//#pragma once
#ifndef __PAGE_STYLE_BIT_H__
#define __PAGE_STYLE_BIT_H__

#include "stdio.h"
#include "rtdbg.h"

typedef struct _hl_page_style_bit
{
    uint32_t page_b_two_in_one : 1;
    uint32_t page_bar : 1;
    uint32_t page_barset : 1;
    uint32_t page_charge : 1;
    uint32_t page_date : 1;
    uint32_t page_lineout : 1;
    uint32_t page_main : 1;
    uint32_t page_menu : 1;
    uint32_t page_monitorset : 1;
    uint32_t page_moreone : 1;
    uint32_t page_pair : 1;
    uint32_t page_poweroff : 1;
    uint32_t page_recodeprotect : 1;
    uint32_t page_s_two_in_one : 1;
    uint32_t page_soundeffect : 1;
    uint32_t page_storage : 1;
    uint32_t page_three_in_one : 1;
    uint32_t page_top : 1;
    uint32_t page_two_in_one : 1;
    uint32_t page_txgain : 1;
    uint32_t page_upgrade : 1;
    uint32_t page_verson : 1;
    uint32_t page_arc : 1;
    uint32_t page_holding : 1;
    uint32_t page_lock : 1;
} hl_page_style_bit;

// 风格创建标记，为1表示已经创建，为0表示未创建
hl_page_style_bit page_style_bit ;
// void page_test(void);

void hl_page_style_bit_init(void);

#endif