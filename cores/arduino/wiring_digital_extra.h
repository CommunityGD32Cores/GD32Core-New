#ifndef _WIRING_DIGITAL_EXTRA_H
#define _WIRING_DIGITAL_EXTRA_H

#include "gd32/PinConfigured.h"
#include "gd32/PinNames.h"

#ifdef __cplusplus
extern "C" {
#endif

void digitalToggle(pin_size_t ulPin);


#if defined(GD32F30x) || defined(GD32F10x)|| defined(GD32E50X)
/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*****************************************************************************/
typedef enum afio_debug_cfg {
    /** Full Serial Wire and JTAG debug */
#ifdef GD32F10x    
    AFIO_DEBUG_FULL_SWJ          = ((uint32_t)0x00300000U | (AFIO_PCF0_SWJ_CFG >> 16)),
#else
    AFIO_DEBUG_FULL_SWJ          = ((uint32_t)0x00300000U | (PCF0_SWJ_CFG(0) >> 16)),
#endif
    /** Full Serial Wire and JTAG, but no NJTRST. */
    AFIO_DEBUG_FULL_SWJ_NO_NJRST = GPIO_SWJ_NONJTRST_REMAP,
    /** Serial Wire debug only (JTAG-DP disabled, SW-DP enabled) */
    AFIO_DEBUG_SW_ONLY           = GPIO_SWJ_SWDPENABLE_REMAP,
    /** No debug; all JTAG and SW pins are free for use as GPIOs. */
    AFIO_DEBUG_NONE              = GPIO_SWJ_DISABLE_REMAP,
} afio_debug_cfg;

static inline void afio_cfg_debug_ports(afio_debug_cfg config) {
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config((uint32_t)config, ENABLE);
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_DIGITAL_EXTRA_H */