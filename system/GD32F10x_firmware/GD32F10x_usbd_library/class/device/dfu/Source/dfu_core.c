/*!
    \file    dfu_core.c
    \brief   USB DFU device class core functions

    \version 2020-07-17, V3.0.0, firmware for GD32F10x
    \version 2022-06-30, V3.1.0, firmware for GD32F10x
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#include "dfu_core.h"
#include "systick.h"
#include "dfu_mem.h"
#include <string.h>

#define USBD_VID                     0x28E9U
#define USBD_PID                     0x0189U

/* local function prototypes ('static') */
static uint8_t dfu_init        (usb_dev *udev, uint8_t config_index);
static uint8_t dfu_deinit      (usb_dev *udev, uint8_t config_index);
static uint8_t dfu_req_handler (usb_dev *udev, usb_req *req);
static uint8_t dfu_ctlx_in     (usb_dev *udev);

/* DFU requests management functions */
static void dfu_detach     (usb_dev *udev, usb_req *req);
static void dfu_dnload     (usb_dev *udev, usb_req *req);
static void dfu_upload     (usb_dev *udev, usb_req *req);
static void dfu_getstatus  (usb_dev *udev, usb_req *req);
static void dfu_clrstatus  (usb_dev *udev, usb_req *req);
static void dfu_getstate   (usb_dev *udev, usb_req *req);
static void dfu_abort      (usb_dev *udev, usb_req *req);
static void string_to_unicode (uint8_t *str, uint16_t *pbuf);

static void dfu_mode_leave            (usb_dev *udev);
static uint8_t dfu_getstatus_complete (usb_dev *udev);

extern dfu_mem_prop dfu_inter_flash_cb;
extern dfu_mem_prop dfu_nor_flash_cb;
extern dfu_mem_prop dfu_nand_flash_cb;

static void (*dfu_request_process[])(usb_dev *udev, usb_req *req) = 
{
    [DFU_DETACH]    = dfu_detach,
    [DFU_DNLOAD]    = dfu_dnload,
    [DFU_UPLOAD]    = dfu_upload,
    [DFU_GETSTATUS] = dfu_getstatus,
    [DFU_CLRSTATUS] = dfu_clrstatus,
    [DFU_GETSTATE]  = dfu_getstate,
    [DFU_ABORT]     = dfu_abort
};

/* note:it should use the c99 standard when compiling the below codes */
/* USB standard device descriptor */
usb_desc_dev dfu_dev_desc =
{
    .header = 
     {
         .bLength          = USB_DEV_DESC_LEN, 
         .bDescriptorType  = USB_DESCTYPE_DEV
     },
    .bcdUSB                = 0x0200U,
    .bDeviceClass          = 0x00U,
    .bDeviceSubClass       = 0x00U,
    .bDeviceProtocol       = 0x00U,
    .bMaxPacketSize0       = USBD_EP0_MAX_SIZE,
    .idVendor              = USBD_VID,
    .idProduct             = USBD_PID,
    .bcdDevice             = 0x0100U,
    .iManufacturer         = STR_IDX_MFC,
    .iProduct              = STR_IDX_PRODUCT,
    .iSerialNumber         = STR_IDX_SERIAL,
    .bNumberConfigurations = USBD_CFG_MAX_NUM
};

/* USB device configuration descriptor */
usb_dfu_desc_config_set dfu_config_desc = 
{
    .config = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_config), 
             .bDescriptorType = USB_DESCTYPE_CONFIG 
         },
        .wTotalLength         = sizeof(usb_dfu_desc_config_set),
        .bNumInterfaces       = 0x01U,
        .bConfigurationValue  = 0x01U,
        .iConfiguration       = 0x00U,
        .bmAttributes         = 0x80U,
        .bMaxPower            = 0x32U
    },

    .dfu_itf0 = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x00U,
        .bNumEndpoints        = 0x00U,
        .bInterfaceClass      = USB_DFU_CLASS,
        .bInterfaceSubClass   = USB_DFU_SUBCLASS_UPGRADE,
        .bInterfaceProtocol   = USB_DFU_PROTOCL_DFU,
        .iInterface           = STR_IDX_ALT_ITF0
    },

    .dfu_itf1 = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x01U,
        .bNumEndpoints        = 0x00U,
        .bInterfaceClass      = USB_DFU_CLASS,
        .bInterfaceSubClass   = USB_DFU_SUBCLASS_UPGRADE,
        .bInterfaceProtocol   = USB_DFU_PROTOCL_DFU,
        .iInterface           = STR_IDX_ALT_ITF1
    },

    .dfu_itf2 = 
    {
        .header = 
         {
             .bLength         = sizeof(usb_desc_itf), 
             .bDescriptorType = USB_DESCTYPE_ITF 
         },
        .bInterfaceNumber     = 0x00U,
        .bAlternateSetting    = 0x02U,
        .bNumEndpoints        = 0x00U,
        .bInterfaceClass      = USB_DFU_CLASS,
        .bInterfaceSubClass   = USB_DFU_SUBCLASS_UPGRADE,
        .bInterfaceProtocol   = USB_DFU_PROTOCL_DFU,
        .iInterface           = STR_IDX_ALT_ITF2
    },

    .dfu_func = 
    {
        .header = 
         {
            .bLength          = sizeof(usb_desc_dfu_func), 
            .bDescriptorType  = DFU_DESC_TYPE 
         },
        .bmAttributes         = USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD | USB_DFU_WILL_DETACH,
        .wDetachTimeOut       = 0x00FFU,
        .wTransferSize        = TRANSFER_SIZE,
        .bcdDFUVersion        = 0x0110U,
    },
};

/* USB language ID descriptor */
static usb_desc_LANGID usbd_language_id_desc = 
{
    .header = 
    {
         .bLength         = sizeof(usb_desc_LANGID), 
         .bDescriptorType = USB_DESCTYPE_STR
    },
    .wLANGID              = ENG_LANGID
};

/* USB manufacture string */
static usb_desc_str manufacturer_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(10U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'i', 'g', 'a', 'D', 'e', 'v', 'i', 'c', 'e'}
};

/* USB product string */
static usb_desc_str product_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(12U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', '-', 'U', 'S', 'B', '_', 'D', 'F', 'U'}
};

/* USB serial string */
static usb_desc_str serial_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     }
};

/* USB configure string */
static usb_desc_str config_string = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(15U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
    .unicode_string = {'G', 'D', '3', '2', ' ', 'U', 'S', 'B', ' ', 'C', 'O', 'N', 'F', 'I', 'G'}
};

/* alternate interface 0 string */
static usb_desc_str interface_string0 = 
{
    .header = 
     {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
     },
};
/* alternate interface 1 string */
static usb_desc_str interface_string1 = 
{
    .header = 
    {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
    },
};

/* alternate interface 2 string */
static usb_desc_str interface_string2 = 
{
    .header = 
    {
         .bLength         = USB_STRING_LEN(2U), 
         .bDescriptorType = USB_DESCTYPE_STR,
    },
};

uint8_t* usbd_dfu_strings[] = 
{
    [STR_IDX_LANGID]  = (uint8_t *)&usbd_language_id_desc,
    [STR_IDX_MFC]     = (uint8_t *)&manufacturer_string,
    [STR_IDX_PRODUCT] = (uint8_t *)&product_string,
    [STR_IDX_SERIAL]  = (uint8_t *)&serial_string,
    [STR_IDX_CONFIG]  = (uint8_t *)&config_string,
    [STR_IDX_ALT_ITF0] = (uint8_t *)&interface_string0,
    [STR_IDX_ALT_ITF1] = (uint8_t *)&interface_string1,
    [STR_IDX_ALT_ITF2] = (uint8_t *)&interface_string2
};

usb_desc dfu_desc = {
    .dev_desc    = (uint8_t *)&dfu_dev_desc,
    .config_desc = (uint8_t *)&dfu_config_desc,
    .strings     = usbd_dfu_strings
};

usb_class dfu_class = {
    .init            = dfu_init,
    .deinit          = dfu_deinit,
    .req_process     = dfu_req_handler,
    .ctlx_in         = dfu_ctlx_in
};

/*!
    \brief      initialize the USB DFU device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_init (usb_dev *udev, uint8_t config_index)
{
    static usbd_dfu_handler dfu_handler;

    /* unlock the internal flash */
    dfu_mem_init();

    systick_config();

    memset((void *)&dfu_handler, 0, sizeof(usbd_dfu_handler));

    dfu_handler.base_addr = APP_LOADED_ADDR;
    dfu_handler.manifest_state = MANIFEST_COMPLETE;
    dfu_handler.bState = STATE_DFU_IDLE;
    dfu_handler.bStatus = STATUS_OK;

    udev->class_data[USBD_DFU_INTERFACE] = (void *)&dfu_handler;

    /* create interface string */
    string_to_unicode((uint8_t *)dfu_inter_flash_cb.pstr_desc, (uint16_t *)udev->desc->strings[STR_IDX_ALT_ITF0]);
    string_to_unicode((uint8_t *)dfu_nor_flash_cb.pstr_desc, (uint16_t *)udev->desc->strings[STR_IDX_ALT_ITF1]);
    string_to_unicode((uint8_t *)dfu_nand_flash_cb.pstr_desc, (uint16_t *)udev->desc->strings[STR_IDX_ALT_ITF2]);
    return USBD_OK;
}

/*!
    \brief      deinitialize the USB DFU device
    \param[in]  udev: pointer to USB device instance
    \param[in]  config_index: configuration index
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_deinit (usb_dev *udev, uint8_t config_index)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    /* restore device default state */
    memset(udev->class_data[USBD_DFU_INTERFACE], 0, sizeof(usbd_dfu_handler));

    dfu->bState = STATE_DFU_IDLE;
    dfu->bStatus = STATUS_OK;

    /* lock the internal flash */
    dfu_mem_deinit();

    return USBD_OK;
}

/*!
    \brief      handle the USB DFU class-specific requests
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: device class-specific request
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_req_handler (usb_dev *udev, usb_req *req)
{
    if (req->bRequest < DFU_REQ_MAX) {
        dfu_request_process[req->bRequest](udev, req);
    } else {
        return USBD_FAIL;
    }

    return USBD_OK;
}

/*!
    \brief      handle data stage
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     USB device operation status
*/
static uint8_t dfu_ctlx_in (usb_dev *udev)
{
    dfu_getstatus_complete(udev);

    return USBD_OK;
}

/*!
    \brief      handle data in stage in control endpoint 0
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     USB device operation status
  */
static uint8_t dfu_getstatus_complete (usb_dev *udev)
{
    uint32_t addr;

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    if (STATE_DFU_DNBUSY == dfu->bState) {
        /* decode the special command */
        if (0U == dfu->block_num) {
            if (1U == dfu->data_len){
                if (GET_COMMANDS == dfu->buf[0]) {
                    /* no operation */
                }
            } else if (5U == dfu->data_len) {
                if (SET_ADDRESS_POINTER == dfu->buf[0]) {
                    /* set flash operation address */
                    dfu->base_addr = *(uint32_t *)(dfu->buf + 1U);
                } else if (ERASE == dfu->buf[0]) {
                    dfu->base_addr = *(uint32_t *)(dfu->buf + 1U);

                    dfu_mem_erase(dfu->base_addr);
                } else {
                    /* no operation */
                }
            } else {
                /* no operation */
            }
        } else if (dfu->block_num > 1U) {  /* regular download command */
            /* decode the required address */
            addr = (dfu->block_num - 2U) * TRANSFER_SIZE + dfu->base_addr;

            dfu_mem_write (dfu->buf, addr, dfu->data_len);

            dfu->block_num = 0U;
        } else {
             /* no operation */
        }

        dfu->data_len = 0U;

        /* update the device state and poll timeout */
        dfu->bState = STATE_DFU_DNLOAD_SYNC;

        return USBD_OK;
    } else if (STATE_DFU_MANIFEST == dfu->bState) {  /* manifestation in progress */
        /* start leaving DFU mode */
        dfu_mode_leave(udev);
    } else {
        /* no operation */
    }

    return USBD_OK;
}

/*!
    \brief      handle the DFU_DETACH request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none.
*/
static void dfu_detach(usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_SYNC:
    case STATE_DFU_DNLOAD_IDLE:
    case STATE_DFU_MANIFEST_SYNC:
    case STATE_DFU_UPLOAD_IDLE:
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
        dfu->iString = 0U; /* iString */

        dfu->block_num = 0U;
        dfu->data_len = 0U;
        break;
 
    default:
        break;
    }

    /* check the detach capability in the DFU functional descriptor */
    if (dfu_config_desc.dfu_func.wDetachTimeOut & DFU_DETACH_MASK) {
        usbd_disconnect(udev);

        usbd_connect(udev);
    } else {
        /* wait for the period of time specified in detach request */
        delay_1ms(4U);
    }
}

/*!
    \brief      handle the DFU_DNLOAD request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_dnload(usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->transc_out[0];

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_IDLE:
        if (req->wLength > 0U) {
            /* update the global length and block number */
            dfu->block_num = req->wValue;
            dfu->data_len = req->wLength;

            dfu->bState = STATE_DFU_DNLOAD_SYNC;

            transc->xfer_len = dfu->data_len;
            transc->xfer_buf = dfu->buf;
            transc->xfer_count = 0U;
        } else {
            dfu->manifest_state = MANIFEST_IN_PROGRESS;
            dfu->bState = STATE_DFU_MANIFEST_SYNC;
        }
        break;

    default:
        break;
    }
}

/*!
    \brief      handles the DFU_UUPLOAD request.
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_upload (usb_dev *udev, usb_req *req)
{
    uint8_t *phy_addr = NULL;
    uint32_t addr = 0U;

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    if(req->wLength <= 0U) {
        dfu->bState = STATE_DFU_IDLE;

        return;
    }

    usb_transc *transc = &udev->transc_in[0];

    switch (dfu->bState) {
    case STATE_DFU_IDLE:
    case STATE_DFU_UPLOAD_IDLE:
        /* update the global length and block number */
        dfu->block_num = req->wValue;
        dfu->data_len = req->wLength;

        /* DFU get command */
        if (0U == dfu->block_num) {
            /* update the state machine */
            dfu->bState = (dfu->data_len > 3U) ? STATE_DFU_IDLE : STATE_DFU_UPLOAD_IDLE;

            /* store the values of all supported commands */
            dfu->buf[0] = GET_COMMANDS;
            dfu->buf[1] = SET_ADDRESS_POINTER;
            dfu->buf[2] = ERASE;

            /* send the status data over EP0 */
            transc->xfer_buf = &(dfu->buf[0]);
            transc->xfer_len = 3U;
        } else if (dfu->block_num > 1U) {
            dfu->bState = STATE_DFU_UPLOAD_IDLE;

            /* change is accelerated */
            addr = (dfu->block_num - 2U) * TRANSFER_SIZE + dfu->base_addr;

            /* return the physical address where data are stored */
          //  phy_addr = (uint8_t *)(addr);

            /* return the physical address where data are stored */
            phy_addr = dfu_mem_read (dfu->buf, addr, dfu->data_len);

            /* send the status data over EP0 */
            transc->xfer_buf = phy_addr;
            transc->xfer_len = dfu->data_len;
        } else {
            dfu->bState = STATUS_ERR_STALLEDPKT;
        }
        break;

    default:
        dfu->data_len = 0U;
        dfu->block_num = 0U;
        break;
    }
}

/*!
    \brief      handle the DFU_GETSTATUS request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_getstatus (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->transc_in[0];

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState) {
    case STATE_DFU_DNLOAD_SYNC:
        if (0U != dfu->data_len) {
            dfu->bState = STATE_DFU_DNBUSY;

            if (0U == dfu->block_num) {
                if (ERASE == dfu->buf[0]) {
                    SET_POLLING_TIMEOUT(FLASH_ERASE_TIMEOUT);
                } else {
                    SET_POLLING_TIMEOUT(FLASH_WRITE_TIMEOUT);
                }
            }
        } else {
            dfu->bState = STATE_DFU_DNLOAD_IDLE;
        }
        break;

    case STATE_DFU_MANIFEST_SYNC:
        if (MANIFEST_IN_PROGRESS == dfu->manifest_state) {
            dfu->bState = STATE_DFU_MANIFEST;
            dfu->bwPollTimeout0 = 1U;
        } else if ((MANIFEST_COMPLETE == dfu->manifest_state) && \
            (dfu_config_desc.dfu_func.bmAttributes & 0x04U)){
            dfu->bState = STATE_DFU_IDLE;
            dfu->bwPollTimeout0 = 0U;
        } else {
            /* no operation */
        }
        break;

    default:
        break;
    }

    /* send the status data of DFU interface to host over EP0 */
    transc->xfer_buf = (uint8_t *)&(dfu->bStatus);
    transc->xfer_len = 6U;
}

/*!
    \brief      handle the DFU_CLRSTATUS request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_clrstatus (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    if (STATE_DFU_ERROR == dfu->bState) {
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
    } else {
        /* state error */
        dfu->bStatus = STATUS_ERR_UNKNOWN;
        dfu->bState = STATE_DFU_ERROR;
    }

    dfu->iString = 0U; /* iString: index = 0 */
}

/*!
    \brief      handle the DFU_GETSTATE request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_getstate (usb_dev *udev, usb_req *req)
{
    usb_transc *transc = &udev->transc_in[0];

    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    /* send the current state of the DFU interface to host */
    transc->xfer_buf = &(dfu->bState);
    transc->xfer_len = 1U;
}

/*!
    \brief      handle the DFU_ABORT request
    \param[in]  udev: pointer to USB device instance
    \param[in]  req: DFU class request
    \param[out] none
    \retval     none
*/
static void dfu_abort (usb_dev *udev, usb_req *req)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    switch (dfu->bState){
    case STATE_DFU_IDLE:
    case STATE_DFU_DNLOAD_SYNC:
    case STATE_DFU_DNLOAD_IDLE:
    case STATE_DFU_MANIFEST_SYNC:
    case STATE_DFU_UPLOAD_IDLE:
        dfu->bStatus = STATUS_OK;
        dfu->bState = STATE_DFU_IDLE;
        dfu->iString = 0U; /* iString: index = 0 */

        dfu->block_num = 0U;
        dfu->data_len = 0U;
        break;

    default:
        break;
    }
}

/*!
    \brief      convert string value into unicode char
    \param[in]  str: pointer to plain string
    \param[in]  pbuf: buffer pointer to store unicode char
    \param[out] none
    \retval     none
*/
static void string_to_unicode (uint8_t *str, uint16_t *pbuf)
{
    uint8_t index = 0;

    if (str != NULL) {
        pbuf[index++] = ((strlen((const char *)str) * 2U + 2U) & 0x00FFU) | ((USB_DESCTYPE_STR << 8U) & 0xFF00);

        while (*str != '\0') {
            pbuf[index++] = *str++;
        }
    }
}

/*!
    \brief      leave DFU mode and reset device to jump to user loaded code
    \param[in]  udev: pointer to USB device instance
    \param[out] none
    \retval     none
*/
static void dfu_mode_leave (usb_dev *udev)
{
    usbd_dfu_handler *dfu = (usbd_dfu_handler *)udev->class_data[USBD_DFU_INTERFACE];

    dfu->manifest_state = MANIFEST_COMPLETE;

    if (dfu_config_desc.dfu_func.bmAttributes & 0x04U) {
        dfu->bState = STATE_DFU_MANIFEST_SYNC;
    } else {
        dfu->bState = STATE_DFU_MANIFEST_WAIT_RESET;

        /* lock the internal flash */
        dfu_mem_deinit();

        /* generate system reset to allow jumping to the user code */
        NVIC_SystemReset();
    }
}
