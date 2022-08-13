/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pl330.c
  * @author  sugar zhang
  * @version V0.1
  * @date    13-Jun-2019
  * @brief   pl330 driver
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <rthw.h>
#include "drv_pm.h"

#ifdef RT_USING_DMA_PL330

#include "dma.h"
#include "drv_clock.h"

#include "hal_base.h"
#include "hal_bsp.h"

#define WORK_QUEUE_STACK_SIZE       512
#define WORK_QUEUE_PRIORITY         0

static uint8_t s_pl330_index = 0;

typedef enum
{
    DMA_STATE_RUNNING,
    DMA_STATE_STOP,
} dma_state_t;

struct rt_dma_pl330
{
    struct rt_dma_device dma;
    struct rt_workqueue *isr_workqueue;
    struct rt_work isr_work[PL330_CHANNELS_PER_DEV];
    struct rt_mutex chan_lock;
#if defined(RT_USING_PMU)
    struct pd *pd;
#endif
    struct HAL_PL330_DEV *pl330; /* hal layer */
};

struct rt_pl330_chan
{
    struct rt_dma_chan chan;
    dma_state_t state;

    struct PL330_CHAN *pchan; /* hal layer */
};

static inline struct rt_dma_pl330 *to_pl330(struct rt_dma_device *dma)
{
    return rt_container_of(dma, struct rt_dma_pl330, dma);
}

static inline struct rt_pl330_chan *to_pchan(struct rt_dma_chan *chan)
{
    return rt_container_of(chan, struct rt_pl330_chan, chan);
}

static int pl330_start(struct rt_dma_chan *chan)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);
#if defined(RT_USING_PMU)
    struct rt_dma_pl330 *rt_pl330 = to_pl330(chan->device);
#endif
    int ret;

    if (rt_pchan->state == DMA_STATE_RUNNING)
        return RT_EOK;

#if defined(RT_USING_PMU)
    if (rt_pl330->pd)
        pd_on(rt_pl330->pd);
#endif
    pm_runtime_request(PM_RUNTIME_ID_DMA);

    ret = HAL_PL330_Start(rt_pchan->pchan);
    if (!ret)
        rt_pchan->state = DMA_STATE_RUNNING;

    return ret;
}

static int pl330_stop(struct rt_dma_chan *chan)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);
#if defined(RT_USING_PMU)
    struct rt_dma_pl330 *rt_pl330 = to_pl330(chan->device);
#endif
    int ret;

    if (rt_pchan->state == DMA_STATE_STOP)
        return RT_EOK;

    ret = HAL_PL330_Stop(rt_pchan->pchan);

    pm_runtime_release(PM_RUNTIME_ID_DMA);

#if defined(RT_USING_PMU)
    if (rt_pl330->pd)
        pd_off(rt_pl330->pd);
#endif

    if (!ret)
        rt_pchan->state = DMA_STATE_STOP;

    return ret;
}

static struct rt_dma_chan *pl330_request_channel(struct rt_dma_device *dma, uint16_t id)
{
    struct rt_dma_pl330 *rt_pl330 = to_pl330(dma);
    struct rt_pl330_chan *rt_pchan;
    struct PL330_CHAN *pchan = NULL;
    void *buf;

    rt_mutex_take(&rt_pl330->chan_lock, RT_WAITING_FOREVER);
    pchan = HAL_PL330_RequestChannel(rt_pl330->pl330, id);
    rt_mutex_release(&rt_pl330->chan_lock);
    if (!pchan)
        return RT_NULL;

    buf = rt_dma_malloc(PL330_CHAN_BUF_LEN);
    if (!buf)
        goto err;

    HAL_PL330_SetMcBuf(pchan, buf);

    rt_pchan = rt_calloc(1, sizeof(*rt_pchan));
    if (!rt_pchan)
        goto err_buf;

    rt_pchan->pchan = pchan;
    rt_pchan->chan.device = dma;
    rt_pchan->state = DMA_STATE_STOP;

    return &rt_pchan->chan;

err_buf:
    rt_dma_free(buf);
err:
    HAL_PL330_ReleaseChannel(pchan);

    return RT_NULL;
}

static int pl330_release_channel(struct rt_dma_chan *chan)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);
    struct rt_dma_pl330 *rt_pl330 = to_pl330(chan->device);
    HAL_Status hRet;
    void *buf;

    buf = HAL_PL330_GetMcBuf(rt_pchan->pchan);
    if (buf)
        rt_dma_free(buf);

    rt_mutex_take(&rt_pl330->chan_lock, RT_WAITING_FOREVER);
    hRet = HAL_PL330_ReleaseChannel(rt_pchan->pchan);
    rt_mutex_release(&rt_pl330->chan_lock);
    HAL_ASSERT(hRet);

    rt_free(rt_pchan);

    return hRet;
}

static int pl330_config(struct rt_dma_chan *chan, struct dma_slave_config *config)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);
    struct DMA_SLAVE_CONFIG sConfig;

    sConfig.direction = config->direction;
    sConfig.srcAddr = config->src_addr;
    sConfig.dstAddr = config->dst_addr;
    sConfig.srcAddrWidth = config->src_addr_width;
    sConfig.dstAddrWidth = config->dst_addr_width;
    sConfig.srcMaxBurst = config->src_maxburst;
    sConfig.dstMaxBurst = config->dst_maxburst;
    sConfig.srcInterlaceSize = config->src_interlace_size;
    sConfig.dstInterlaceSize = config->dst_interlace_size;

    return HAL_PL330_Config(rt_pchan->pchan, &sConfig);
}

static int pl330_prep_dma_cyclic(struct rt_dma_chan *chan, uint32_t dmaAddr,
                                 uint32_t len, uint32_t periodLen,
                                 dma_direction_t direction,
                                 dma_callback callback, void *cparam)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);

    return HAL_PL330_PrepDmaCyclic(rt_pchan->pchan, dmaAddr, len, periodLen,
                                   direction, callback, cparam);
}

static int pl330_prep_dma_single(struct rt_dma_chan *chan, uint32_t dmaAddr,
                                 uint32_t len,
                                 dma_direction_t direction,
                                 dma_callback callback, void *cparam)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);

    return HAL_PL330_PrepDmaSingle(rt_pchan->pchan, dmaAddr, len, direction,
                                   callback, cparam);
}

static int pl330_prep_dma_memcpy(struct rt_dma_chan *chan, uint32_t dst,
                                 uint32_t src, uint32_t len,
                                 dma_callback callback, void *cparam)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);

    return HAL_PL330_PrepDmaMemcpy(rt_pchan->pchan, dst, src, len,
                                   callback, cparam);
}

static int pl330_get_position(struct rt_dma_chan *chan)
{
    struct rt_pl330_chan *rt_pchan = to_pchan(chan);

    return HAL_PL330_GetPosition(rt_pchan->pchan);
}

static void pl330_work(struct rt_work *work, void *work_data)
{
    const struct PL330_DESC *desc = (const struct PL330_DESC *)work_data;

    RT_ASSERT(desc);

    if (desc->callback)
        desc->callback(desc->cparam);
}

static void pl330_isr(struct HAL_PL330_DEV *pl330)
{
    struct rt_dma_pl330 *rt_pl330 = pl330->priv;
    rt_uint32_t irqstatus, i;

    rt_interrupt_enter();

    irqstatus = HAL_PL330_IrqHandler(pl330);

    for (i = 0; i < PL330_CHANNELS_PER_DEV; i++)
    {
        if (irqstatus & (1 << i))
            rt_workqueue_dowork(rt_pl330->isr_workqueue, &rt_pl330->isr_work[i]);
    }

    rt_interrupt_leave();
}

static rt_err_t pl330_init(struct HAL_PL330_DEV *pl330,
                           rt_isr_handler_t handler)
{
    struct rt_dma_pl330 *rt_pl330;
    struct rt_dma_device *dma;
    HAL_Status hal_ret;
    char name[RT_NAME_MAX];
    int i;

    rt_pl330 = rt_calloc(1, sizeof(*rt_pl330));
    RT_ASSERT(rt_pl330);

    rt_pl330->pl330 = pl330;
    pl330->priv = rt_pl330;

#if defined(RT_USING_PMU)
    rt_pl330->pd = get_pd_from_id(pl330->pd);

    if (rt_pl330->pd)
        pd_on(rt_pl330->pd);
#endif

    hal_ret = HAL_PL330_Init(pl330);
    RT_ASSERT(hal_ret == HAL_OK);

#if defined(RT_USING_PMU)
    if (rt_pl330->pd)
        pd_off(rt_pl330->pd);
#endif

    rt_snprintf(name, RT_NAME_MAX, "pl330-%d", s_pl330_index++);
    rt_pl330->isr_workqueue = rt_workqueue_create(name, WORK_QUEUE_STACK_SIZE,
                              WORK_QUEUE_PRIORITY);
    RT_ASSERT(rt_pl330->isr_workqueue);

    for (i = 0; i < PL330_CHANNELS_PER_DEV; i++)
        rt_work_init(&rt_pl330->isr_work[i], pl330_work,
                     (void *)HAL_PL330_GetDesc(&pl330->chans[i]));

    rt_mutex_init(&rt_pl330->chan_lock, name, RT_IPC_FLAG_FIFO);

    dma = &rt_pl330->dma;

    dma->base = (uint32_t)pl330->pReg;
    dma->start = pl330_start;
    dma->stop = pl330_stop;
    dma->request_channel = pl330_request_channel;
    dma->release_channel = pl330_release_channel;
    dma->config = pl330_config;
    dma->prep_dma_memcpy = pl330_prep_dma_memcpy;
    dma->prep_dma_cyclic = pl330_prep_dma_cyclic;
    dma->prep_dma_single = pl330_prep_dma_single;
    dma->get_position = pl330_get_position;

    for (i = 0; i < PL330_NR_IRQS; i++)
    {
        rt_hw_interrupt_install(pl330->irq[i], handler, RT_NULL, RT_NULL);
        rt_hw_interrupt_umask(pl330->irq[i]);
    }

    return rt_hw_dma_register(&rt_pl330->dma);
}

static void pl330_0_isr(int irq, void *param)
{
    pl330_isr(&g_pl330Dev);
}

int rt_hw_pl330_init(void)
{
    rt_err_t ret;

    /* init dmac0 */
    ret = pl330_init(&g_pl330Dev, pl330_0_isr);
    RT_ASSERT(ret == RT_EOK);

    return 0;
}

INIT_PREV_EXPORT(rt_hw_pl330_init);
#endif
