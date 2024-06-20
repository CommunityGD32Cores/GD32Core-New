/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "timer.h"

#if defined(GD32F1x0) || defined(GD32F3x0) || defined(GD32E50X) || defined(GD32EPRT)
#define TIMER5_IRQ_Name TIMER5_DAC_IRQn
#else
#define TIMER5_IRQ_Name TIMER5_IRQn
#endif 

#if defined(GD32E23x) || defined(GD32F1x0) || defined(GD32F3x0) || defined(GD32E50X)
#define TIMER0_IRQ_Name TIMER0_Channel_IRQn
#else
#define TIMER0_IRQ_Name TIMER0_IRQn
#endif

#if defined(GD32F30x)
#if defined(GD32F30X_HD)
#define  TIMER0_Update_IRQ_Name TIMER0_UP_IRQn
#else
#define  TIMER0_Update_IRQ_Name TIMER0_UP_TIMER9_IRQn
#endif
#elif defined(GD32E23x) || defined(GD32F3x0) || defined(GD32F1x0)
#define  TIMER0_Update_IRQ_Name TIMER0_BRK_UP_TRG_COM_IRQn
#elif defined(GD32E50X)
#if defined(GD32E50X_HD) || defined(GD32EPRT)
#define  TIMER0_Update_IRQ_Name TIMER0_UP_IRQn
#define NO_TIMER_9
#else
#define  TIMER0_Update_IRQ_Name TIMER0_UP_TIMER9_IRQn
#define TIMER9_IRQ_NAME TIMER0_UP_TIMER9_IRQn
#endif
#elif defined(GD32F10x)
// GD32F10X_XD has these, but requires work to add
#define NO_TIMER_8
#define NO_TIMER_9
#define NO_TIMER_10
#define NO_TIMER_11
#define NO_TIMER_12
#define NO_TIMER_13
#if !defined(GD32F10X_XD)
#define TIMER0_Update_IRQ_Name TIMER0_UP_IRQn
#else
#define TIMER0_Update_IRQ_Name TIMER0_UP_TIMER9_IRQn
#endif
#endif

#if defined(GD32E50X)
#define TIMER7_IRQ_NAME TIMER7_Channel_IRQn
#if defined(GD32E50X_XD) || defined(GD32E50X_CL) || defined(GD32E508)
#define TIMER7_UP_IRQ_NAME TIMER7_UP_TIMER12_IRQn
#define TIMER10_IRQ_NAME TIMER0_TRG_CMT_TIMER10_IRQn
#define TIMER11_IRQ_NAME TIMER7_BRK_TIMER11_IRQn
#define TIMER12_IRQ_NAME TIMER7_UP_TIMER12_IRQn
#define TIMER13_IRQ_NAME TIMER7_TRG_CMT_TIMER13_IRQn
#else 
#define TIMER7_UP_IRQ_NAME TIMER7_UP_IRQn
#define NO_TIMER_10
#define NO_TIMER_11
#define NO_TIMER_12
#define NO_TIMER_13
#endif
#elif defined(GD32F10x)
#if !defined(GD32F10X_XD)
#define TIMER7_IRQ_NAME TIMER7_UP_IRQn
#define TIMER7_UP_IRQ_NAME TIMER7_UP_IRQn
#else
#define TIMER7_IRQ_NAME TIMER7_UP_TIMER12_IRQn
#define TIMER7_UP_IRQ_NAME TIMER7_UP_TIMER12_IRQn
#endif
#else
#define TIMER7_IRQ_NAME TIMER7_IRQn
#define TIMER7_UP_IRQ_NAME TIMER7_IRQn
#endif
/* availablility of timers despite their macros being defined. */
/* e.g., for a GD32F303CC, the macro TIMER8 is defined, although */
/* TIMER8_IRQn is not defined since the timer is not actually available */ 
/* we have to work around this. */
#if defined(GD32F30x)
#if !defined(GD32F30X_HD)
#define HAS_TIMER_8
#define HAS_TIMER_9
#define HAS_TIMER_9
#define HAS_TIMER_10
#define HAS_TIMER_11
#define HAS_TIMER_12
#define HAS_TIMER_13
#endif
#else
#ifndef NO_TIMER_8
#define HAS_TIMER_8
#endif
#ifndef NO_TIMER_9
#define HAS_TIMER_9
#endif
#ifndef NO_TIMER_10
#define HAS_TIMER_10
#endif
#ifndef NO_TIMER_11
#define HAS_TIMER_11
#endif
#ifndef NO_TIMER_12
#define HAS_TIMER_12
#endif
#ifndef NO_TIMER_13
#define HAS_TIMER_13
#endif
#endif

#if defined(GD32F3x0) && !defined(GD32F350)
#define NO_TIMER_5
#endif

extern timerhandle_t timerHandle;
extern pwmhandle_t pwmHandle;

timerhandle_t timerHandle = {
    .init                      = Timer_init,
    .start                     = Timer_start,
    .stop                      = Timer_stop,
    .setPeriodTime             = Timer_setPeriodTime,
    .refresh                   = Timer_rfresh,
    .enableUpdateIT            = Timer_enableUpdateIT,
    .enableCaptureIT           = Timer_enableCaptureIT,
    .disableUpdateIT           = Timer_disableUpdateIT,
    .disableCaptureIT          = Timer_disableCaptureIT,
    .interruptHandle           = Timer_updateHandle,
    .captureInterruptHandle[0] = Timer_captureHandle,
    .captureInterruptHandle[1] = Timer_captureHandle,
    .captureInterruptHandle[2] = Timer_captureHandle,
    .captureInterruptHandle[3] = Timer_captureHandle
};

pwmhandle_t pwmHandle = {
    .init            = PWM_init,
    .start           = PWM_start,
    .stop            = PWM_stop,
    .setPeriodCycle  = PWM_setPeriodCycle,
    .enablePWMIT     = PWM_enablePWMIT,
    .disablePWMIT    = PWM_disablePWMIT,
    .interruptHandle = PWM_irqHandle,
    .writeCycleValue = PWM_writeCyclevalue
};

/*!
    \brief      get timer index
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     timer index
*/
uint32_t getTimerIndex(uint32_t instance)
{
    uint32_t index = 0xFF;
    switch (instance) {
#if defined(TIMER0)
        case TIMER0:
            index = 0;
            break;
#endif
#if defined(TIMER1)
        case TIMER1:
            index = 1;
            break;
#endif
#if defined(TIMER2)
        case TIMER2:
            index = 2;
            break;
#endif
#if defined(TIMER3)
        case TIMER3:
            index = 3;
            break;
#endif
#if defined(TIMER4)
        case TIMER4:
            index = 4;
            break;
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
        case TIMER5:
            index = 5;
            break;
#endif
#if defined(TIMER6)
        case TIMER6:
            index = 6;
            break;
#endif
#if defined(TIMER7)
        case TIMER7:
            index = 7;
            break;
#endif
#if defined(TIMER8)
        case TIMER8:
            index = 8;
            break;
#endif
#if defined(TIMER9)
        case TIMER9:
            index = 9;
            break;
#endif
#if defined(TIMER10)
        case TIMER10:
            index = 10;
            break;
#endif
#if defined(TIMER11)
        case TIMER11:
            index = 11;
            break;
#endif
#if defined(TIMER12)
        case TIMER12:
            index = 12;
            break;
#endif
#if defined(TIMER13)
        case TIMER13:
            index = 13;
            break;
#endif
#if defined(TIMER14)
        case TIMER14:
            index = 14;
            break;
#endif
#if defined(TIMER15)
        case TIMER15:
            index = 15;
            break;
#endif
#if defined(TIMER16)
        case TIMER16:
            index = 16;
            break;
#endif
    }
    return index;
}

/*!
    \brief      get timer device from pinname
    \param[in]  instance: PWMx(x=0..11)
    \param[out] nonex
    \retval     pwmDevice_t obj(TIMERx and TIMER_CH_x)
*/
pwmDevice_t getTimerDeviceFromPinname(PinName instance)
{
    uint32_t pinfunction;
    pwmDevice_t pwmDevice;
    pwmDevice.timer = pinmap_peripheral(instance, PinMap_PWM);
    pinfunction = pinmap_function(instance, PinMap_PWM);
    pwmDevice.channel = GD_PIN_CHANNEL_GET(pinfunction);
    return pwmDevice;
}

/*!
    \brief      get pwm index
    \param[in]  instance: pwmDevice_t obj
    \param[out] none
    \retval     pwm index
*/
uint32_t getPWMIndex(pwmDevice_t instance)
{
    uint8_t timer_index = 0xFF;
    uint8_t channel_index = 0xFF;
    timer_index = getTimerIndex(instance.timer);
    switch (instance.channel) {
        case TIMER_CH_0:
            channel_index = 0;
            break;
        case TIMER_CH_1:
            channel_index = 1;
            break;
        case TIMER_CH_2:
            channel_index = 2;
            break;
        case TIMER_CH_3:
            channel_index = 3;
            break;
        default:
            break;
    }
    return (timer_index * 4 + channel_index);
}

/*!
    \brief      enable timer clock
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void timer_clock_enable(uint32_t instance)
{
    uint32_t temp = 0;
    switch (instance) {
#if defined(TIMER0)
        case TIMER0:
            temp = RCU_TIMER0;
            break;
#endif
#if defined(TIMER1)
        case TIMER1:
            temp = RCU_TIMER1;
            break;
#endif
#if defined(TIMER2)
        case TIMER2:
            temp = RCU_TIMER2;
            break;
#endif
#if defined(TIMER3)
        case TIMER3:
            temp = RCU_TIMER3;
            break;
#endif
#if defined(TIMER4)
        case TIMER4:
            temp = RCU_TIMER4;
            break;
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
        case TIMER5:
            temp = RCU_TIMER5;
            break;
#endif
#if defined(TIMER6)
        case TIMER6:
            temp = RCU_TIMER6;
            break;
#endif
#if defined(TIMER7)
        case TIMER7:
            temp = RCU_TIMER7;
            break;
#endif
#if defined(TIMER8)&& defined(HAS_TIMER_8) /* ToDO: Fix this so for non-F30x series that also have TIMER8 *and* RCU_TIMER8 */
        case TIMER8:
            temp = RCU_TIMER8;
            break;
#endif
#if defined(TIMER9)&& defined(HAS_TIMER_9)
        case TIMER9:
            temp = RCU_TIMER9;
            break;
#endif
#if defined(TIMER10)&& defined(HAS_TIMER_10)
        case TIMER10:
            temp = RCU_TIMER10;
            break;
#endif
#if defined(TIMER11)&& defined(HAS_TIMER_11)
        case TIMER11:
            temp = RCU_TIMER11;
            break;
#endif
#if defined(TIMER12)&& defined(HAS_TIMER_12)
        case TIMER12:
            temp = RCU_TIMER12;
            break;
#endif
#if defined(TIMER13)&& defined(HAS_TIMER_13)
        case TIMER13:
            temp = RCU_TIMER13;
            break;
#endif
#if defined(TIMER14)
        case TIMER14:
            temp = RCU_TIMER14;
            break;
#endif
#if defined(TIMER15)
        case TIMER15:
            temp = RCU_TIMER15;
            break;
#endif
#if defined(TIMER16)
        case TIMER16:
            temp = RCU_TIMER16;
            break;
#endif
        default:
            break;
    }
    rcu_periph_clock_enable(temp);
}

/*!
    \brief      disable timer clock
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void timer_clock_disable(uint32_t instance)
{
    uint32_t temp = 0;
    switch (instance) {
#if defined(TIMER0)
        case TIMER0:
            temp = RCU_TIMER0;
            break;
#endif
#if defined(TIMER1)
        case TIMER1:
            temp = RCU_TIMER1;
            break;
#endif
#if defined(TIMER2)
        case TIMER2:
            temp = RCU_TIMER2;
            break;
#endif
#if defined(TIMER3)
        case TIMER3:
            temp = RCU_TIMER3;
            break;
#endif
#if defined(TIMER4)
        case TIMER4:
            temp = RCU_TIMER4;
            break;
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
        case TIMER5:
            temp = RCU_TIMER5;
            break;
#endif
#if defined(TIMER6)
        case TIMER6:
            temp = RCU_TIMER6;
            break;
#endif
#if defined(TIMER7)
        case TIMER7:
            temp = RCU_TIMER7;
            break;
#endif
#if defined(TIMER8)&& defined(HAS_TIMER_8) /* ToDO: Fix this so for non-F30x series that also have TIMER8 *and* RCU_TIMER8 */
        case TIMER8:
            temp = RCU_TIMER8;
            break;
#endif
#if defined(TIMER9)&& defined(HAS_TIMER_9)
        case TIMER9:
            temp = RCU_TIMER9;
            break;
#endif
#if defined(TIMER10)&& defined(HAS_TIMER_10) 
        case TIMER10:
            temp = RCU_TIMER10;
            break;
#endif
#if defined(TIMER11)&& defined(HAS_TIMER_11)
        case TIMER11:
            temp = RCU_TIMER11;
            break;
#endif
#if defined(TIMER12)&& defined(HAS_TIMER_12)
        case TIMER12:
            temp = RCU_TIMER12;
            break;
#endif
#if defined(TIMER13)&& defined(HAS_TIMER_13)
        case TIMER13:
            temp = RCU_TIMER13;
            break;
#endif
#if defined(TIMER14)
        case TIMER14:
            temp = RCU_TIMER14;
            break;
#endif
#if defined(TIMER15)
        case TIMER15:
            temp = RCU_TIMER15;
            break;
#endif
#if defined(TIMER16)
        case TIMER16:
            temp = RCU_TIMER16;
            break;
#endif
        default:
            break;
    }
    rcu_periph_clock_disable(temp);
}

/*!
    \brief      initialize timer
    \param[in]  instance: TIMERx(x=0..13)
    \param[in]  timerPeriod: period and format
    \param[out] none
    \retval     none
*/
void Timer_init(uint32_t instance, timerPeriod_t *timerPeriod)
{
    timer_parameter_struct timer_initpara;

#if defined(GD32E23x)
    /* no subprio */
    nvic_irq_enable(getTimerUpIrq(instance), 2);
    nvic_irq_enable(getTimerCCIrq(instance), 2);
#else
    nvic_irq_enable(getTimerUpIrq(instance), 2, 2);
    nvic_irq_enable(getTimerCCIrq(instance), 2, 2);
#endif
    timer_clock_enable(instance);
    timer_deinit(instance);
    switch (timerPeriod->format) {
        case FORMAT_US:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 1000000 - 1;
            timer_initpara.period = timerPeriod->time - 1;
            timer_initpara.repetitioncounter = 0;
            break;
        case FORMAT_MS:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 10000 - 1;
            timer_initpara.period = timerPeriod->time * 10 - 1;
            timer_initpara.repetitioncounter = 0;
            break;
        case FORMAT_S:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 10000 - 1;
            timer_initpara.period = timerPeriod->time * 40 - 1 ;
            timer_initpara.repetitioncounter = 250;
            break;
        default:
            break;
    }
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(instance, &timer_initpara);
}

/*!
    \brief      start timer
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void Timer_start(uint32_t instance)
{
    timer_enable(instance);
}

/*!
    \brief      stop timer
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void Timer_stop(uint32_t instance)
{
    timer_disable(instance);
}

/*!
    \brief      set timer period
    \param[in]  instance: TIMERx(x=0..13)
    \param[in]  timerPeriod: time and format
    \param[out] none
    \retval     none
*/
void Timer_setPeriodTime(uint32_t instance, timerPeriod_t *timerPeriod)
{
    uint32_t prescalerfactor = 0;
    uint32_t period_cycle = 0;
    timer_parameter_struct timer_initpara;
    //timer_deinit(instance);
    switch (timerPeriod->format) {
        case FORMAT_TICK:
            timer_initpara.prescaler = TIMER_PSC(instance);
            timer_initpara.period = timerPeriod->time - 1;
            timer_initpara.repetitioncounter  = 0;
            break;
        case FORMAT_US:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 1000000 - 1;
            timer_initpara.period = timerPeriod->time - 1;
            timer_initpara.repetitioncounter = 0;
            break;
        case FORMAT_MS:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 10000 - 1;
            timer_initpara.period = timerPeriod->time * 10 - 1;
            timer_initpara.repetitioncounter = 0;
            break;
        case FORMAT_S:
            timer_initpara.prescaler = getTimerClkFrequency(instance) / 10000 - 1;
            timer_initpara.period = timerPeriod->time * 40 - 1;
            timer_initpara.repetitioncounter = 250;
            break;
        case FORMAT_HZ:
            period_cycle = getTimerClkFrequency(instance) / timerPeriod->time;
            prescalerfactor = (period_cycle / 65536) + 1;
            timer_initpara.prescaler = prescalerfactor - 1;
            timer_initpara.period = period_cycle / prescalerfactor;
            timer_initpara.repetitioncounter  = 0;
            break;
        default:
            break;
    }
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;

    timer_init(instance, &timer_initpara);
}

/*!
    \brief      update some registers to restart counters
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void Timer_rfresh(uint32_t instance)
{
    timer_event_software_generate(instance, TIMER_EVENT_SRC_UPG);
}

/*!
    \brief      enable timer update interrupt
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void Timer_enableUpdateIT(uint32_t instance)
{
    timer_flag_clear(instance, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(instance, TIMER_INT_UP);
}

/*!
    \brief      disable timer update interrupt
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void Timer_disableUpdateIT(uint32_t instance)
{
    timer_flag_clear(instance, TIMER_INT_FLAG_UP);
    timer_interrupt_disable(instance, TIMER_INT_UP);
}

/*!
    \brief      enable timer channel capture interrupt
    \param[in]  instance: TIMERx(x=0..13)
    \param[in]  channel: TIMERx(x=0..3)
    \param[out] none
    \retval     none
*/
void Timer_enableCaptureIT(uint32_t instance, uint8_t channel)
{
    uint32_t interrupt;
    uint32_t interrupt_flag;
    switch (channel) {
        case 0:
            interrupt = TIMER_INT_CH0;
            interrupt_flag = TIMER_INT_FLAG_CH0;
            break;
        case 1:
            interrupt = TIMER_INT_CH1;
            interrupt_flag = TIMER_INT_FLAG_CH1;
            break;
        case 2:
            interrupt = TIMER_INT_CH2;
            interrupt_flag = TIMER_INT_FLAG_CH2;
            break;
        case 3:
            interrupt = TIMER_INT_CH3;
            interrupt_flag = TIMER_INT_FLAG_CH3;
            break;
        default:
            //ToDo: better error handling in case of invalid params
            return;
    }
    timer_interrupt_flag_clear(instance, interrupt_flag);
    timer_interrupt_enable(instance, interrupt);
}

/*!
    \brief      disable timer channel capture interrupt
    \param[in]  instance: TIMERx(x=0..13)
    \param[in]  channel: TIMERx(x=0..3)
    \param[out] none
    \retval     none
*/
void Timer_disableCaptureIT(uint32_t instance, uint8_t channel)
{
    uint32_t interrupt;
    uint32_t interrupt_flag;
    switch (channel) {
        case 0:
            interrupt = TIMER_INT_CH0;
            interrupt_flag = TIMER_INT_FLAG_CH0;
            break;
        case 1:
            interrupt = TIMER_INT_CH1;
            interrupt_flag = TIMER_INT_FLAG_CH1;
            break;
        case 2:
            interrupt = TIMER_INT_CH2;
            interrupt_flag = TIMER_INT_FLAG_CH2;
            break;
        case 3:
            interrupt = TIMER_INT_CH3;
            interrupt_flag = TIMER_INT_FLAG_CH3;
            break;
        default:
            //ToDo: better error handling in case of invalid params
            return;
    }
    timer_interrupt_flag_clear(instance, interrupt_flag);
    timer_interrupt_disable(instance, interrupt);
}
/*!
    \brief      initialize pwm
    \param[in]  pwmDevice: pwm device
    \param[in]  pwmPeriodCycle: pwm period and cycle
    \param[out] none
    \retval     none
*/
void PWM_init(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle)
{
    (void) pwmPeriodCycle; // ToDo remove parameter from API
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    uint32_t periph = pwmDevice->timer;
#if defined(GD32E23x)
    /* no subprio */
    nvic_irq_enable(getTimerCCIrq(periph), 2);
#else 
    nvic_irq_enable(getTimerCCIrq(periph), 2, 2);
#endif
    timer_clock_enable(periph);
#if defined(GD32F30x)
    rcu_periph_clock_enable(RCU_AF);
#elif defined(GD32F3x0) || defined(GD32F1x0) || defined(GD32F4xx)
    rcu_periph_clock_enable(RCU_CFGCMP);
#endif
    /* configure TIMER base function */
    timer_initpara.prescaler = getTimerClkFrequency(periph) / 1000000 - 1 ;
    timer_initpara.period = 9999;
    timer_initpara.clockdivision = 0;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_init(pwmDevice->timer, &timer_initpara);

    /* configure TIMER channel output function */
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_DISABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;

    timer_channel_output_config(pwmDevice->timer, pwmDevice->channel, &timer_ocintpara);
    timer_channel_output_pulse_value_config(pwmDevice->timer, pwmDevice->channel, 4999);
    timer_channel_output_mode_config(pwmDevice->timer, pwmDevice->channel, TIMER_OC_MODE_PWM0);
    timer_auto_reload_shadow_enable(pwmDevice->timer);
    timer_channel_output_fast_config(pwmDevice->timer, pwmDevice->channel, TIMER_OC_FAST_DISABLE);
    timer_primary_output_config(pwmDevice->timer, ENABLE);

    timer_enable(periph);
    timer_channel_output_state_config(pwmDevice->timer, pwmDevice->channel, TIMER_CCX_DISABLE);
}

/*!
    \brief      start pwm output
    \param[in]  pwmDevice: pwm device
    \param[out] none
    \retval     none
*/
void PWM_start(pwmDevice_t *pwmDevice)
{
    timer_channel_output_state_config(pwmDevice->timer, pwmDevice->channel, TIMER_CCX_ENABLE);
}

/*!
    \brief      stop pwm output
    \param[in]  pwmDevice: pwm device
    \param[out] none
    \retval     none
*/
void PWM_stop(pwmDevice_t *pwmDevice)
{
    timer_channel_output_state_config(pwmDevice->timer, pwmDevice->channel, TIMER_CCX_DISABLE);
}

/*!
    \brief      set pwm period and cycle
    \param[in]  pwmDevice: pwm device
    \param[in]  pwmPeriodCycle: pwm period and cycle
    \param[out] none
    \retval     none
*/
void PWM_setPeriodCycle(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle)
{
    uint32_t ccvalue = 0;
    uint32_t prescalerfactor = 0;
    uint32_t period_cycle = 0;
    timer_parameter_struct timer_initpara;
    switch (pwmPeriodCycle->format) {
        case FORMAT_US:
            timer_initpara.prescaler = getTimerClkFrequency(pwmDevice->timer) / 1000000 - 1;
            timer_initpara.period = pwmPeriodCycle->period - 1;
            timer_initpara.repetitioncounter = 0;
            ccvalue = pwmPeriodCycle->cycle;
            break;
        case FORMAT_MS:
            timer_initpara.prescaler = getTimerClkFrequency(pwmDevice->timer) / 10000 - 1;
            timer_initpara.period = pwmPeriodCycle->period * 10 - 1;
            timer_initpara.repetitioncounter = 0;
            ccvalue = pwmPeriodCycle->cycle * 10;
            break;
        case FORMAT_S:
            timer_initpara.prescaler = getTimerClkFrequency(pwmDevice->timer) / 10000 - 1;
            timer_initpara.period = pwmPeriodCycle->period * 40;
            timer_initpara.repetitioncounter = 250;
            ccvalue = pwmPeriodCycle->cycle * 40;
            break;
        case FORMAT_HZ:
            period_cycle = getTimerClkFrequency(pwmDevice->timer) / pwmPeriodCycle->period;
            prescalerfactor = (period_cycle / 65536) + 1;
            timer_initpara.prescaler = prescalerfactor - 1;
            timer_initpara.period = period_cycle / prescalerfactor;
            timer_initpara.repetitioncounter  = 0;
            ccvalue = pwmPeriodCycle->cycle * (timer_initpara.period + 1) / 100.0 ;
            break;
        default:
            break;
    }
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(pwmDevice->timer, &timer_initpara);
    timer_channel_output_pulse_value_config(pwmDevice->timer, pwmDevice->channel, ccvalue - 1);
}

/*!
    \brief      set pwm  cycle
    \param[in]  pwmDevice: pwm device
    \param[in]  pwmPeriodCycle: pwm period and cycle
    \param[out] none
    \retval     none
*/
void PWM_writeCyclevalue(pwmDevice_t *pwmDevice, pwmPeriodCycle_t *pwmPeriodCycle)
{
    uint32_t value ;

    switch (pwmPeriodCycle->format) {
        case FORMAT_US:
            value = pwmPeriodCycle->cycle;
            break;
        case FORMAT_MS:
            value = pwmPeriodCycle->cycle * 10;
            break;
        case FORMAT_S:
            value = pwmPeriodCycle->cycle * 40;
            break;
        case FORMAT_HZ:
            value = pwmPeriodCycle->cycle / 100.0 * (TIMER_CAR(pwmDevice->timer) + 1);
            break;
        default:
            //ToDo: better error handling in case of invalid params
            return;
    }
    timer_channel_output_pulse_value_config(pwmDevice->timer, pwmDevice->channel, value - 1);
}

/*!
    \brief      enable pwm interrupt
    \param[in]  pwmDevice: pwm device
    \param[out] none
    \retval     none
*/
void PWM_enablePWMIT(pwmDevice_t *pwmDevice)
{
    uint32_t interrupt;
    uint32_t interrupt_flag;
    uint16_t ch = pwmDevice->channel;
    switch (ch) {
        case 0:
            interrupt = TIMER_INT_CH0;
            interrupt_flag = TIMER_INT_FLAG_CH0;
            break;
        case 1:
            interrupt = TIMER_INT_CH1;
            interrupt_flag = TIMER_INT_FLAG_CH1;
            break;
        case 2:
            interrupt = TIMER_INT_CH2;
            interrupt_flag = TIMER_INT_FLAG_CH2;
            break;
        case 3:
            interrupt = TIMER_INT_CH3;
            interrupt_flag = TIMER_INT_FLAG_CH3;
            break;
        default:
            //ToDo: better error handling in case of invalid params
            return;
    }
    timer_interrupt_flag_clear(pwmDevice->timer, interrupt_flag);
    timer_interrupt_enable(pwmDevice->timer, interrupt);
}

/*!
    \brief      disable pwm interrupt
    \param[in]  pwmDevice: pwm device
    \param[out] none
    \retval     none
*/
void PWM_disablePWMIT(pwmDevice_t *pwmDevice)
{
    uint32_t interrupt;
    uint32_t interrupt_flag;
    uint16_t ch = pwmDevice->channel;
    switch (ch) {
        case 0:
            interrupt = TIMER_INT_CH0;
            interrupt_flag = TIMER_INT_FLAG_CH0;
            break;
        case 1:
            interrupt = TIMER_INT_CH1;
            interrupt_flag = TIMER_INT_FLAG_CH1;
            break;
        case 2:
            interrupt = TIMER_INT_CH2;
            interrupt_flag = TIMER_INT_FLAG_CH2;
            break;
        case 3:
            interrupt = TIMER_INT_CH3;
            interrupt_flag = TIMER_INT_FLAG_CH3;
            break;
        default:
            //ToDo: better error handling in case of invalid params
            return;
    }
    timer_interrupt_flag_clear(pwmDevice->timer, interrupt_flag);
    timer_interrupt_disable(pwmDevice->timer, interrupt);
}

/*!
    \brief      get timer clock frequency
    \param[in]  instance: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
uint32_t getTimerClkFrequency(uint32_t instance)
{
    rcu_clock_freq_enum timerclkSrc =  NC;
    uint32_t APBx_PSC = 0;
    uint32_t clk_src = 0;

    if (instance != (uint32_t) NC) {
        switch ((uint32_t)instance) {
#if defined(TIMER0)
            case (uint32_t)TIMER0:
#endif
#if defined(TIMER7)
            case (uint32_t)TIMER7:
#endif
#if defined(TIMER8)
            case (uint32_t)TIMER8:
#endif
#if defined(TIMER9)
            case (uint32_t)TIMER9:
#endif
#if defined(TIMER10)
            case (uint32_t)TIMER10:
#endif
                timerclkSrc = CK_APB2;
                APBx_PSC = (RCU_CFG0 & RCU_CFG0_APB2PSC) >> 11;
                break;
#if defined(TIMER1)
            case (uint32_t)TIMER1:
#endif
#if defined(TIMER2)
            case (uint32_t)TIMER2:
#endif
#if defined(TIMER3)
            case (uint32_t)TIMER3:
#endif
#if defined(TIMER4)
            case (uint32_t)TIMER4:
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
            case (uint32_t)TIMER5:
#endif
#if defined(TIMER6)
            case (uint32_t)TIMER6:
#endif
#if defined(TIMER11)
            case (uint32_t)TIMER11:
#endif
#if defined(TIMER12)
            case (uint32_t)TIMER12:
#endif
#if defined(TIMER13)
            case (uint32_t)TIMER13:
#endif
#if defined(TIMER14)
            case (uint32_t)TIMER14:
#endif
#if defined(TIMER15)
            case (uint32_t)TIMER15:
#endif
#if defined(TIMER16)
            case (uint32_t)TIMER16:
#endif
                timerclkSrc = CK_APB1;
                APBx_PSC = (RCU_CFG0 & RCU_CFG0_APB1PSC) >> 8;
                break;
            default:
                break;
        }
    }
    if (0 != (APBx_PSC & 0x04)) {
        clk_src = 2 * rcu_clock_freq_get(timerclkSrc);
    } else {
        clk_src =  rcu_clock_freq_get(timerclkSrc);
    }
    return clk_src;
}

/*!
    \brief      get timer update IRQn
    \param[in]  tim: TIMERx(x=0..13)
    \param[out] none
    \retval     timer update IRQn
*/
IRQn_Type getTimerUpIrq(uint32_t tim)
{
    IRQn_Type IRQn = NonMaskableInt_IRQn;

    if (tim != (uint32_t)NC) {
        switch ((uint32_t)tim) {
#if defined(TIMER0)
            case (uint32_t)TIMER0:
                //differing update interrupt for Timer0 on some devices
                IRQn = TIMER0_Update_IRQ_Name;
                break;
#endif
#if defined(TIMER1)
            case (uint32_t)TIMER1:
                IRQn = TIMER1_IRQn;
                break;
#endif
#if defined(TIMER2)
            case (uint32_t)TIMER2:
                IRQn = TIMER2_IRQn;
                break;
#endif
#if defined(TIMER3)
            case (uint32_t)TIMER3:
                IRQn = TIMER3_IRQn;
                break;
#endif
#if defined(TIMER4)
            case (uint32_t)TIMER4:
                IRQn = TIMER4_IRQn;
                break;
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
            case (uint32_t)TIMER5:
                IRQn = TIMER5_IRQ_Name;
                break;
#endif
#if defined(TIMER6)
            case (uint32_t)TIMER6:
                IRQn = TIMER6_IRQn;
                break;
#endif
#if defined(TIMER7)
            case (uint32_t)TIMER7:
                IRQn = TIMER7_UP_IRQ_NAME;
                break;
#endif
#if defined(TIMER8) && defined(HAS_TIMER_8)
            case (uint32_t)TIMER8:
                IRQn = TIMER8_IRQn;
                break;
#endif
#if defined(TIMER9) && defined(HAS_TIMER_9)
            case (uint32_t)TIMER9:
                IRQn = TIMER9_IRQn;
                break;
#endif
#if defined(TIMER10) && defined(HAS_TIMER_10)
            case (uint32_t)TIMER10:
            #ifdef TIMER10_IRQ_NAME //TODO: repeat this for other timers...
                IRQn = TIMER10_IRQ_NAME;
            #else
                IRQn = TIMER10_IRQn;
            #endif
                break;
#endif
#if defined(TIMER11) && defined(HAS_TIMER_11)
            case (uint32_t)TIMER11:
                IRQn = TIMER11_IRQn;
                break;
#endif
#if defined(TIMER12) && defined(HAS_TIMER_12)
            case (uint32_t)TIMER12:
                IRQn = TIMER12_IRQn;
                break;
#endif
#if defined(TIMER13) && defined(HAS_TIMER_13)
            case (uint32_t)TIMER13:
                IRQn = TIMER13_IRQn;
                break;
#endif
#if defined(TIMER14)
            case (uint32_t)TIMER14:
                IRQn = TIMER14_IRQn;
                break;
#endif
#if defined(TIMER15)
            case (uint32_t)TIMER15:
                IRQn = TIMER15_IRQn;
                break;
#endif
#if defined(TIMER16)
            case (uint32_t)TIMER16:
                IRQn = TIMER16_IRQn;
                break;
#endif
            default:
                break;
        }
    }
    return IRQn;
}

/*!
    \brief      get timer capture/compare IRQn
    \param[in]  tim: TIMERx(x=0..13)
    \param[out] none
    \retval     timer capture/compare IRQn
*/
IRQn_Type getTimerCCIrq(uint32_t tim)
{
    IRQn_Type IRQn = NonMaskableInt_IRQn;

    if (tim != (uint32_t)NC) {
        switch ((uint32_t)tim) {
#if defined(TIMER0)
            case (uint32_t)TIMER0:
                IRQn = TIMER0_Channel_IRQn;
                break;
#endif
#if defined(TIMER1)
            case (uint32_t)TIMER1:
                IRQn = TIMER1_IRQn;
                break;
#endif
#if defined(TIMER2)
            case (uint32_t)TIMER2:
                IRQn = TIMER2_IRQn;
                break;
#endif
#if defined(TIMER3)
            case (uint32_t)TIMER3:
                IRQn = TIMER3_IRQn;
                break;
#endif
#if defined(TIMER4)
            case (uint32_t)TIMER4:
                IRQn = TIMER4_IRQn;
                break;
#endif
#if defined(TIMER5) && !defined(NO_TIMER_5)
            case (uint32_t)TIMER5:
                IRQn = TIMER5_IRQ_Name;
                break;
#endif
#if defined(TIMER6)
            case (uint32_t)TIMER6:
                IRQn = TIMER6_IRQn;
                break;
#endif
#if defined(TIMER7)
            case (uint32_t)TIMER7:
                IRQn = TIMER7_IRQ_NAME;
                break;
#endif
#if defined(TIMER8) && defined(HAS_TIMER_8)
            case (uint32_t)TIMER8:
                IRQn = TIMER8_IRQn;
                break;
#endif
#if defined(TIMER9) && defined(HAS_TIMER_9)
            case (uint32_t)TIMER9:
                IRQn = TIMER9_IRQn;
                break;
#endif
#if defined(TIMER10) && defined(HAS_TIMER_10)
            case (uint32_t)TIMER10:
                IRQn = TIMER10_IRQn;
                break;
#endif
#if defined(TIMER11) && defined(HAS_TIMER_11)
            case (uint32_t)TIMER11:
                IRQn = TIMER11_IRQn;
                break;
#endif
#if defined(TIMER12) && defined(HAS_TIMER_12)
            case (uint32_t)TIMER12:
                IRQn = TIMER12_IRQn;
                break;
#endif
#if defined(TIMER13) && defined(HAS_TIMER_13)
            case (uint32_t)TIMER13:
                IRQn = TIMER13_IRQn;
                break;
#endif
#if defined(TIMER14)
            case (uint32_t)TIMER14:
                IRQn = TIMER14_IRQn;
                break;
#endif
#if defined(TIMER15)
            case (uint32_t)TIMER15:
                IRQn = TIMER15_IRQn;
                break;
#endif
#if defined(TIMER16)
            case (uint32_t)TIMER16:
                IRQn = TIMER16_IRQn;
                break;
#endif
            default:
                break;
        }
    }
    return IRQn;
}

/*!
    \brief      timer interrupt handler
    \param[in]  timer: TIMERx(x=0..13)
    \param[out] none
    \retval     none
*/
void timerinterrupthandle(uint32_t timer)
{
    if (timer_interrupt_flag_get(timer, TIMER_INT_FLAG_UP) != RESET) {
        timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_UP);
        if (timerHandle.interruptHandle) {
            timerHandle.interruptHandle(timer);
        }
    } else if (timer_interrupt_flag_get(timer, TIMER_INT_FLAG_CH0) != RESET) {
        timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_CH0);
        if (pwmHandle.interruptHandle) {
            pwmHandle.interruptHandle(timer, TIMER_CH_0);
        }
        if (timerHandle.captureInterruptHandle[0]) {
            timerHandle.captureInterruptHandle[0](timer, 0);
        }
    } else if (timer_interrupt_flag_get(timer, TIMER_INT_FLAG_CH1) != RESET) {
        timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_CH1);
        if (pwmHandle.interruptHandle) {
            pwmHandle.interruptHandle(timer, TIMER_CH_1);
        }
        if (timerHandle.captureInterruptHandle[1]) {
            timerHandle.captureInterruptHandle[1](timer, 1);
        }
    } else if (timer_interrupt_flag_get(timer, TIMER_INT_FLAG_CH2) != RESET) {
        timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_CH2);
        if (pwmHandle.interruptHandle) {
            pwmHandle.interruptHandle(timer, TIMER_CH_2);
        }
        if (timerHandle.captureInterruptHandle[2]) {
            timerHandle.captureInterruptHandle[2](timer, 2);
        }
    } else if (timer_interrupt_flag_get(timer, TIMER_INT_FLAG_CH3) != RESET) {
        timer_interrupt_flag_clear(timer, TIMER_INT_FLAG_CH3);
        if (pwmHandle.interruptHandle) {
            pwmHandle.interruptHandle(timer, TIMER_CH_3);
        }
        if (timerHandle.captureInterruptHandle[3]) {
            timerHandle.captureInterruptHandle[3](timer, 3);
        }
    }
}

#if defined(TIMER0)
void TIMER0_IRQHandler(void)
{
    timerinterrupthandle(TIMER0);
#if defined(TIMER9)
    timerinterrupthandle(TIMER9);
#endif
}

void TIMER0_Channel_IRQHandler(void)
{
    timerinterrupthandle(TIMER0);
}

/* some devices have this. */
void TIMER0_BRK_UP_TRG_COM_IRQHandler(void)
{
    timerinterrupthandle(TIMER0);
}

void TIMER0_UP_TIMER9_IRQHandler(void)
{
    timerinterrupthandle(TIMER0);
#if defined(TIMER9)
    timerinterrupthandle(TIMER9);
#endif
}

void TIMER0_UP_IRQHandler(void)
{
    timerinterrupthandle(TIMER0);
}

#endif /* TIMER0/TIMER9 handler */

#if defined(TIMER1)
void TIMER1_IRQHandler(void)
{
    timerinterrupthandle(TIMER1);
}
#endif /* TIMER1 handler */

#if defined(TIMER2)
void TIMER2_IRQHandler(void)
{
    timerinterrupthandle(TIMER2);
}
#endif /* TIMER2 handler */

#if defined(TIMER3)
void TIMER3_IRQHandler(void)
{
    timerinterrupthandle(TIMER3);
}
#endif /* TIMER3 handler */

#if defined(TIMER4)
void TIMER4_IRQHandler(void)
{
    timerinterrupthandle(TIMER4);
}
#endif /* TIMER4 handler */

#if defined(TIMER5) && !defined(NO_TIMER_5)
void TIMER5_IRQHandler(void)
{
    timerinterrupthandle(TIMER5);
}

/* interrupt handler name for multiple F1x0, E50, F3x0, F4xx,.. chips */
void TIMER5_DAC_IRQHandler(void) {
    timerinterrupthandle(TIMER5);
}
#endif /* TMER5 handler */

#if defined(TIMER6)
void TIMER6_IRQHandler(void)
{
    timerinterrupthandle(TIMER6);
}
#endif /* TIMER6 handler */

#if defined(TIMER7)
void TIMER7_IRQHandler(void)
{
    timerinterrupthandle(TIMER7);
#if defined(TIMER12)
    timerinterrupthandle(TIMER12);
#endif
}

void TIMER7_Channel_IRQHandler(void)
{
    timerinterrupthandle(TIMER7);
}
#endif /* TIMER7/TIMER12 handler */

#if defined(TIMER8)
void TIMER8_IRQHandler(void)
{
    timerinterrupthandle(TIMER8);
}
#endif /* TIMER8 handler */

#if defined(TIMER9) && !defined (TIMER0)
void TIMER9_IRQHandler(void)
{
    timerinterrupthandle(TIMER9);
}
#endif /* TIMER9 handler */

#if defined(TIMER10)
void TIMER10_IRQHandler(void)
{
    timerinterrupthandle(TIMER10);
}
#endif /* TIMER10 handler */

#if defined(TIMER11)
void TIMER11_IRQHandler(void)
{
    timerinterrupthandle(TIMER11);
}
#endif /* TIMER11 handler */

#if defined(TIMER12) && !defined (TIMER7)
void TIMER12_IRQHandler(void)
{
    timerinterrupthandle(TIMER12);
}
#endif /* TIMER12 handler */

#if defined(TIMER13)
void TIMER13_IRQHandler(void)
{
    timerinterrupthandle(TIMER13);
}
#endif /* TIMER13 handler */

#if defined(TIMER14)
void TIMER14_IRQHandler(void)
{
    timerinterrupthandle(TIMER14);
}
#endif

#if defined(TIMER15)
void TIMER15_IRQHandler(void)
{
    timerinterrupthandle(TIMER15);
}
#endif

#if defined(TIMER16)
void TIMER16_IRQHandler(void)
{
    timerinterrupthandle(TIMER16);
}
#endif
