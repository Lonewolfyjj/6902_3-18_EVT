/******************************************************************************
 *
 *  Copyright (C) 2014 Google, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <stddef.h>
#include <stdlib.h>
#include "osi/types.h"

char *osi_strdup(const char *str);

void *osi_malloc_func(size_t size);
void *osi_calloc_func(size_t size);
void osi_free_func(void *ptr);

#ifdef CONFIG_BLUEDROID_MEM_DEBUG

void osi_mem_dbg_init(void);
void osi_mem_dbg_record(void *p, int size, const char *func, int line);
void osi_mem_dbg_clean(void *p, const char *func, int line);
void osi_mem_dbg_show(void);

#ifdef CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST
#define osi_malloc(size)                                \
({                                                      \
    void *p;                                            \
     p = rt_malloc((size));                                  \
    osi_mem_dbg_record(p, size, __func__, __LINE__);    \
    (void *)p;                                          \
})

#define osi_calloc(size)                                \
({                                                      \
    void *p;                                        \
    p = rt_calloc(1, (size));                              \
    osi_mem_dbg_record(p, size, __func__, __LINE__);    \
    (void *)p;                                          \
})

#else

#define osi_malloc(size)                                \
({                                                      \
    void *p;                                            \
    p = rt_malloc((size));                                 \
    osi_mem_dbg_record(p, size, __func__, __LINE__);    \
    (void *)p;                                          \
})

#define osi_calloc(size)                               \
({                                                    \
    void *p;                                           \
    p = rt_calloc(1, (size));                              \
    osi_mem_dbg_record(p, size, __func__, __LINE__);    \
    (void *)p;                                          \
})

#endif /* #ifdef CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST */

#define osi_free(ptr)                                   \
do {                                                    \
    void *tmp_point = (void *)(ptr);                    \
    osi_mem_dbg_clean(tmp_point, __func__, __LINE__);   \
    rt_free(tmp_point);                                    \
} while (0)

#else

#ifdef CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST
    #define osi_malloc(size)                  rt_malloc((size))//heap_caps_malloc_prefer(size, 2, MALLOC_CAP_DEFAULT|MALLOC_CAP_SPIRAM, MALLOC_CAP_DEFAULT|MALLOC_CAP_INTERNAL)
    #define osi_calloc(size)                 rt_calloc(1, (size))// heap_caps_calloc_prefer(1, size, 2, MALLOC_CAP_DEFAULT|MALLOC_CAP_SPIRAM, MALLOC_CAP_DEFAULT|MALLOC_CAP_INTERNAL)
#else
    #define osi_malloc(size)                  rt_malloc((size))
    #define osi_calloc(size)                  rt_calloc(1, (size))
#endif /* #ifdef CONFIG_BT_ALLOCATION_FROM_SPIRAM_FIRST */
#define osi_free(p)                       rt_free((p))

#endif /* CONFIG_BLUEDROID_MEM_DEBUG */

#define FREE_AND_RESET(a)   \
do {                        \
    if (a) {                \
        osi_free(a);        \
        a = NULL;           \
    }                       \
}while (0)


#endif /* _ALLOCATOR_H_ */
