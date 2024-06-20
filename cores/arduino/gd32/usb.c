#ifdef USBCON
#include "usb.h"

#include "usbd_lld_int.h"

usb_dev usbd;

static void rcu_config()
{
    uint32_t system_clock = rcu_clock_freq_get(CK_SYS);

    /* disable USB APB1 clock (bootloader may have left it running) */
    rcu_periph_clock_disable(RCU_USBD);

#ifdef RCC_AHBPeriph_GPIO_PULLUP
    /* enable USB pull-up pin clock */
    rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);
#endif

    if (48000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1);
    } else if (72000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1_5);
    } else if (96000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2);
    } else if (120000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);
    } else {
        /* TODO: panic if the clock doesn’t match assertions. */
    }

    /* enable USB APB1 clock */
    rcu_periph_clock_enable(RCU_USBD);
}

static void gpio_config()
{
#ifdef USB_PULLUP
#ifndef USB_PULLUP_PIN_MODE
#define USB_PULLUP_PIN_MODE GPIO_MODE_OUT_PP
#endif
    /* configure usb pull-up pin */
    gpio_init(USB_PULLUP, USB_PULLUP_PIN_MODE, GPIO_OSPEED_50MHZ, USB_PULLUP_PIN);
#endif
}

static void nvic_config()
{
    /* 2 bits for preemption priority, 2 bits for subpriority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    /* enable the USB low priority interrupt */
    nvic_irq_enable((uint8_t)USBD_LP_CAN0_RX0_IRQn, 2U, 0U);

    /* enable the USB low priority interrupt */
    nvic_irq_enable((uint8_t)USBD_HP_CAN0_TX_IRQn, 1U, 0U);

    /* enable the USB wakeup interrupt */
    nvic_irq_enable((uint8_t)USBD_WKUP_IRQn, 1U, 0U);
}

void usb_init(usb_desc* desc, usb_class* class_core)
{
    rcu_config();
    gpio_config();

    // Wait a bit after GPIO setup so the host can register a disconnect from the port.
    volatile int i = 0;
    for (;i<5000;){i++;}

    usbd_init(&usbd, desc, class_core);
}

void usb_connect()
{
    nvic_config();
    usbd_connect(&usbd);
    while (usbd.cur_status != USBD_CONFIGURED) {}
}

void usb_enable_interrupts()
{
    /* enable all interrupts mask bits */
    USBD_CTL |= CTL_STIE | CTL_WKUPIE | CTL_SPSIE | CTL_SOFIE | CTL_ESOFIE | CTL_RSTIE;
}

void usb_disable_interrupts()
{
    /* enable all interrupts mask bits */
    USBD_CTL &= ~(CTL_STIE | CTL_WKUPIE | CTL_SPSIE | CTL_SOFIE | CTL_ESOFIE | CTL_RSTIE);
}

void usb_disconnect()
{
    usbd_disconnect(&usbd);
}

void USBD_HP_CAN0_TX_IRQHandler()
{
    usbd_isr();
}

void USBD_LP_CAN0_RX0_IRQHandler()
{
    usbd_isr();
}

void USBD_WKUP_IRQHandler()
{
    exti_interrupt_flag_clear(EXTI_18);
}
#endif
