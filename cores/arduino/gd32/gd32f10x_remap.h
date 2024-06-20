    __REMAP_NAME__(SPI0_REMAP) /*!< SPI0 remapping */
    __REMAP_NAME__(I2C0_REMAP) /*!< I2C0 remapping */
    __REMAP_NAME__(USART0_REMAP) /*!< USART0 remapping */
    __REMAP_NAME__(USART1_REMAP) /*!< USART1 remapping */
    __REMAP_NAME__(USART2_PARTIAL_REMAP) /*!< USART2 partial remapping */
    __REMAP_NAME__(USART2_FULL_REMAP) /*!< USART2 full remapping */
    __REMAP_NAME__(TIMER0_PARTIAL_REMAP) /*!< TIMER0 partial remapping */
    __REMAP_NAME__(TIMER0_FULL_REMAP) /*!< TIMER0 full remapping */
    __REMAP_NAME__(TIMER1_PARTIAL_REMAP0) /*!< TIMER1 partial remapping */
    __REMAP_NAME__(TIMER1_PARTIAL_REMAP1) /*!< TIMER1 partial remapping */
    __REMAP_NAME__(TIMER1_FULL_REMAP) /*!< TIMER1 full remapping */
    __REMAP_NAME__(TIMER2_PARTIAL_REMAP) /*!< TIMER2 partial remapping */
    __REMAP_NAME__(TIMER2_FULL_REMAP) /*!< TIMER2 full remapping */
    __REMAP_NAME__(TIMER3_REMAP) /*!< TIMER3 remapping */
    __REMAP_NAME__(PD01_REMAP) /*!< PD01 remapping */
#if (defined(GD32F10X_MD) || defined(GD32F10X_HD) || defined(GD32F10X_XD))
    __REMAP_NAME__(CAN_PARTIAL_REMAP) /*!< CAN partial remapping(only for GD32F10X_MD devices), GD32F10X_HD devices and GD32F10X_XD devices) */
    __REMAP_NAME__(CAN_FULL_REMAP) /*!< CAN full remapping(only for GD32F10X_MD devices), GD32F10X_HD devices and GD32F10X_XD devices) */
#endif /* GD32F10X_MD||GD32F10X_HD||GD32F10X_XD */
#if (defined(GD32F10X_CL) || defined(GD32F10X_HD) || defined(GD32F10X_XD))
    __REMAP_NAME__(SPI2_REMAP) /*!< SPI2 remapping(only for GD32F10X_CL devices) */
#endif /* GD32F10X_CL||GD32F10X_HD */
#if (defined(GD32F10X_CL) || defined(GD32F10X_HD))
    __REMAP_NAME__(TIMER4CH3_IREMAP) /*!< TIMER4 channel3 internal remapping(only for GD32F10X_CL devices and GD32F10X_HD devices) */
#endif /* GD32F10X_CL||GD32F10X_HD */
#if (defined(GD32F10X_MD) || defined(GD32F10X_HD) || defined(GD32F10X_XD))
    __REMAP_NAME__(ADC0_ETRGRT_REMAP) /*!< ADC0 external trigger routine conversion remapping(only for GD32F10X_MD devices), GD32F10X_HD devices and GD32F10X_XD devices) */
    __REMAP_NAME__(ADC1_ETRGRT_REMAP) /*!< ADC1 external trigger routine conversion remapping(only for GD32F10X_MD devices), GD32F10X_HD devices and GD32F10X_XD devices) */
#endif /* GD32F10X_MD||GD32F10X_HD||GD32F10X_XD */
    __REMAP_NAME__(SWJ_NONJTRST_REMAP) /*!< full SWJ(JTAG-DP + SW-DP)),but without NJTRST */
    __REMAP_NAME__(SWJ_SWDPENABLE_REMAP) /*!< JTAG-DP disabled and SW-DP enabled */
    __REMAP_NAME__(SWJ_DISABLE_REMAP) /*!< JTAG-DP disabled and SW-DP disabled */
#ifdef GD32F10X_CL
    __REMAP_NAME__(CAN0_PARTIAL_REMAP) /*!< CAN0 partial remapping(only for GD32F10X_CL devices) */
    __REMAP_NAME__(CAN0_FULL_REMAP) /*!< CAN0 full remapping(only for GD32F10X_CL devices) */
    __REMAP_NAME__(ENET_REMAP) /*!< ENET remapping(only for GD32F10X_CL devices) */
    __REMAP_NAME__(CAN1_REMAP) /*!< CAN1 remapping(only for GD32F10X_CL devices) */
    __REMAP_NAME__(TIMER1ITI1_REMAP) /*!< TIMER1 internal trigger 1 remapping(only for GD32F10X_CL devices) */
    __REMAP_NAME__(PTP_PPS_REMAP) /*!< ethernet PTP PPS remapping(only for GD32F10X_CL devices) */
#endif /* GD32F10X_CL */
#ifdef GD32F10X_XD
    __REMAP_NAME__(TIMER8_REMAP) /*!< TIMER8 remapping */
    __REMAP_NAME__(TIMER9_REMAP) /*!< TIMER9 remapping */
    __REMAP_NAME__(TIMER10_REMAP) /*!< TIMER10 remapping */
    __REMAP_NAME__(TIMER12_REMAP) /*!< TIMER12 remapping */
    __REMAP_NAME__(TIMER13_REMAP) /*!< TIMER13 remapping */
    __REMAP_NAME__(EXMC_NADV_REMAP) /*!< EXMC_NADV connect/disconnect */
#endif /* GD32F10X_XD */