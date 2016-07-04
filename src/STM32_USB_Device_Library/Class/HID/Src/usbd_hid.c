/**
  ******************************************************************************
  * @file    usbd_hid.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid.h"
#include "usbd_desc_hid.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 




/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_HID_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_HID_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_HID_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_HID_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_HID_GetDeviceQualifierDesc (uint16_t *length);
static uint8_t  USBD_HID_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_HID_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);
/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef  USBD_HID = 
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  USBD_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_HID_GetCfgDesc,
  USBD_HID_GetCfgDesc, 
  USBD_HID_GetCfgDesc,
  USBD_HID_GetDeviceQualifierDesc,
};
#if 1
#if 1
#define USBShort(ui16Value)     (ui16Value & 0xff), (ui16Value >> 8)
#define USB_EP_DESC_OUT                 0x00
#define USB_EP_DESC_IN                  0x80
#define USBEPToIndex(x)         ((x) >> 4)
#define USB_EP_ATTR_BULK                0x02
#define DATA_IN_EP_MAX_SIZE     64
#define DATA_OUT_EP_MAX_SIZE    64

__ALIGN_BEGIN uint8_t USBD_HID_CfgDesc[170] __ALIGN_END =
{	
	//
	// Configuration descriptor header.
	//
	9,                          // Size of the configuration descriptor.
	USB_DESC_TYPE_CONFIGURATION,    // Type of this descriptor.
	USBShort(170),               // The total size of this full structure.
	7,                          // The number of interfaces in this
								// configuration.
	1,                          // The unique value for this configuration.
	5,                          // The string identifier that describes this
								// configuration.
	0xC0,     // Bus Powered, Self Powered, remote wake up.
	250,                        // The maximum power in 2mA increments.
	//
    // Vendor-specific Interface Descriptor.
    //
    9,                              // Size of the interface descriptor.
    4,            // Type of this descriptor.
    0,                              // The index for this interface.
    0,                              // The alternate setting for this
                                    // interface.
    2,                              // The number of endpoints used by this
                                    // interface.
    0xff,        // The interface class
    0,                              // The interface sub-class.
    0,                              // The interface protocol for the sub-class
                                    // specified above.
    4,                              // The string index for this interface.

    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x01,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.
	//
	// Endpoint Descriptor
	//
	7,								 // The size of the endpoint descriptor.
	USB_DESC_TYPE_ENDPOINT, 			 // Descriptor type is an endpoint.
	USB_EP_DESC_OUT | 0x01,
	USB_EP_ATTR_BULK,				 // Endpoint is a bulk endpoint.
	USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
	0,								 // The polling interval for this endpoint.    
	//
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	1,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x02,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x02,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
    //
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	2,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x03,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x03,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
    //
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	3,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x04,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x04,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
    //
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	4,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x05,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x05,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
    //
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	5,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x06,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x06,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
    //
	// Vendor-specific Interface Descriptor.
	//
	9,								// Size of the interface descriptor.
	4,			  // Type of this descriptor.
	6,								// The index for this interface.
	0,								// The alternate setting for this
									// interface.
	2,								// The number of endpoints used by this
									// interface.
	0xff,		 // The interface class
	0,								// The interface sub-class.
	0,								// The interface protocol for the sub-class
									// specified above.
	4,								// The string index for this interface.
    
    //
    // Endpoint Descriptor
    //
    7,                              // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,             // Descriptor type is an endpoint.
    USB_EP_DESC_IN | 0x07,
    USB_EP_ATTR_BULK,               // Endpoint is a bulk endpoint.
    USBShort(DATA_IN_EP_MAX_SIZE),  // The maximum packet size.
    0,                              // The polling interval for this endpoint.

    //
    // Endpoint Descriptor
    //
    7,                               // The size of the endpoint descriptor.
    USB_DESC_TYPE_ENDPOINT,              // Descriptor type is an endpoint.
    USB_EP_DESC_OUT | 0x07,
    USB_EP_ATTR_BULK,                // Endpoint is a bulk endpoint.
    USBShort(DATA_OUT_EP_MAX_SIZE),  // The maximum packet size.
    0,                               // The polling interval for this endpoint.
};

#else
__ALIGN_BEGIN uint8_t USBD_HID_CfgDesc[144] __ALIGN_END =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
  144,                /* wTotalLength:no of returned bytes */
  0x00,
  0x02,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0x32,   /* MaxPower 0 mA */
  
  /*---------------------------------------------------------------------------*/
  
  /*Interface Descriptor */
  0x09,   /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: One endpoints used */
  0x02,   /* bInterfaceClass: Communication Interface Class */
  0x02,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: */
  
  /*Header Functional Descriptor*/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Func Desc */
  0x10,   /* bcdCDC: spec release number */
  0x01,
  
  /*Call Management Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x01,   /* bDataInterface: 1 */
  
  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */
  
  /*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */
  0x01,   /* bSlaveInterface0: Data Class Interface */
  
  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x02,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(64),     /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                           /* bInterval: */ 
  /*Endpoint 2 Descriptor*/
  0x07,                           /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
  0x82,                     /* bEndpointAddress */
  0x02,                           /* bmAttributes: Interrupt */
  LOBYTE(64),     /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,
  /*---------------------------------------------------------------------------*/
  
  /*Data class interface descriptor*/
  0x09,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
  0x01,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x0c,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */
  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x01,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x81,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                               /* bInterval: ignore for Bulk transfer */
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x03,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x83,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x04,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x84,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x05,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x85,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x06,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x86,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,  
  /*Endpoint OUT Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x07,                        /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,                              /* bInterval: ignore for Bulk transfer */
  
  /*Endpoint IN Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
  0x87,                         /* bEndpointAddress */
  0x02,                              /* bmAttributes: Bulk */
  LOBYTE(64),  /* wMaxPacketSize: */
  HIBYTE(64),
  0x00,  
} ;
#endif
#else
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ]  __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xE0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  
  /************** Descriptor of Joystick Mouse interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x02,         /*bInterfaceClass: HID*/
  0x02,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Mouse endpoint ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
  
  HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x02,          /*bmAttributes: Interrupt endpoint*/
  LOBYTE(64),	/* wMaxPacketSize: */
  HIBYTE(64),
  0,          /*bInterval: Polling Interval (10 ms)*/
  /* 34 */
} ;
#endif
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]  __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE]  __ALIGN_END =
{
  0x05,   0x01,
  0x09,   0x02,
  0xA1,   0x01,
  0x09,   0x01,
  
  0xA1,   0x00,
  0x05,   0x09,
  0x19,   0x01,
  0x29,   0x03,
  
  0x15,   0x00,
  0x25,   0x01,
  0x95,   0x03,
  0x75,   0x01,
  
  0x81,   0x02,
  0x95,   0x01,
  0x75,   0x05,
  0x81,   0x01,
  
  0x05,   0x01,
  0x09,   0x30,
  0x09,   0x31,
  0x09,   0x38,
  
  0x15,   0x81,
  0x25,   0x7F,
  0x75,   0x08,
  0x95,   0x03,
  
  0x81,   0x06,
  0xC0,   0x09,
  0x3c,   0x05,
  0xff,   0x09,
  
  0x01,   0x15,
  0x00,   0x25,
  0x01,   0x75,
  0x01,   0x95,
  
  0x02,   0xb1,
  0x22,   0x75,
  0x06,   0x95,
  0x01,   0xb1,
  
  0x01,   0xc0
}; 

/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx)
{
  uint8_t ret = 0;
  
  /* Open EP IN */
  
  USBD_LL_OpenEP(pdev,
                 0x81,
                 USBD_EP_TYPE_BULK,
                 64);  
  USBD_LL_OpenEP(pdev,
                 0x82,
                 USBD_EP_TYPE_BULK,
                 64);
  
  USBD_LL_OpenEP(pdev,
                 0x83,
                 USBD_EP_TYPE_BULK,
                 64);  
  USBD_LL_OpenEP(pdev,
                 0x84,
                 USBD_EP_TYPE_BULK,
                 64);  
  USBD_LL_OpenEP(pdev,
                 0x85,
                 USBD_EP_TYPE_BULK,
                 64);  
  USBD_LL_OpenEP(pdev,
                 0x86,
                 USBD_EP_TYPE_BULK,
                 64);  
  USBD_LL_OpenEP(pdev,
                 0x87,
                 USBD_EP_TYPE_BULK,
                 64);  

  pdev->pClassData = USBD_malloc(sizeof (USBD_HID_HandleTypeDef));
  
  if(pdev->pClassData == NULL)
  {
    ret = 1; 
  }
  else
  {
    ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[0] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[1] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[2] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[3] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[4] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[5] = HID_IDLE;
	((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[6] = HID_IDLE;
  }
  return ret;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{
  /* Close HID EPs */
  USBD_LL_CloseEP(pdev,
                  HID_EPIN_ADDR);
  
  /* FRee allocated memory */
  if(pdev->pClassData != NULL)
  {
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  } 
  
  return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_HID_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
  uint16_t len = 0;
  uint8_t  *pbuf = NULL;
  USBD_HID_HandleTypeDef     *hhid = (USBD_HID_HandleTypeDef*) pdev->pClassData;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
      
      
    case HID_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;
      
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hhid->Protocol,
                        1);    
      break;
      
    case HID_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;
      
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hhid->IdleState,
                        1);        
      break;      
      
    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( req->wValue >> 8 == HID_REPORT_DESC)
      {
        len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
        pbuf = HID_MOUSE_ReportDesc;
      }
      else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
      {
        pbuf = USBD_HID_Desc;   
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData (pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&hhid->AltSetting,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      hhid->AltSetting = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport 
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport     (USBD_HandleTypeDef  *pdev, 
								 uint8_t addr,
                                 uint8_t *report,
                                 uint16_t len)
{
  USBD_HID_HandleTypeDef     *hhid = (USBD_HID_HandleTypeDef*)pdev->pClassData;
  
  if (pdev->dev_state == USBD_STATE_CONFIGURED )
  {
  	while(hhid->state[addr-0x81] == HID_BUSY)
    //if(hhid->state[addr-0x81] == HID_IDLE)
    //{
      hhid->state[addr-0x81] = HID_BUSY;
      USBD_LL_Transmit (pdev, 
                        addr,                                      
                        report,
                        len);
    //}
	//else
	//	return USBD_BUSY;
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_GetPollingInterval 
  *         return polling interval from endpoint descriptor
  * @param  pdev: device instance
  * @retval polling interval
  */
uint32_t USBD_HID_GetPollingInterval (USBD_HandleTypeDef *pdev)
{
  uint32_t polling_interval = 0;

  /* HIGH-speed endpoints */
  if(pdev->dev_speed == USBD_SPEED_HIGH)
  {
   /* Sets the data transfer polling interval for high speed transfers. 
    Values between 1..16 are allowed. Values correspond to interval 
    of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
    polling_interval = (((1 <<(HID_HS_BINTERVAL - 1)))/8);
  }
  else   /* LOW and FULL-speed endpoints */
  {
    /* Sets the data transfer polling interval for low and full 
    speed transfers */
    polling_interval =  HID_FS_BINTERVAL;
  }
  
  return ((uint32_t)(polling_interval));
}

/**
  * @brief  USBD_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}


/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_HID_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
  
  /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
  //printf("HID epnum %d IN\n",epnum);
  ((USBD_HID_HandleTypeDef *)pdev->pClassData)->state[epnum-1] = HID_IDLE;
  return USBD_OK;
}

/**
  * @brief  USBD_CDC_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_HID_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{      
  USBD_HID_HandleTypeDef   *hcdc = (USBD_HID_HandleTypeDef*) pdev->pClassData;
  
  /* Get the received data length */
  int RxLength = USBD_LL_GetRxDataSize (pdev, epnum);
  printf("%d Got %d bytes\n",epnum,RxLength);
  /* USB data will be immediately processed, this allow next USB traffic being 
  NAKed till the end of the application Xfer */
  //if(pdev->pClassData != NULL)
  //{
    //((USBD_CDC_ItfTypeDef *)pdev->pUserData)->Receive(hcdc->RxBuffer, &hcdc->RxLength);

   // return USBD_OK;
  //}
  //else
  //{
  //  return USBD_FAIL;
 // }
 USBD_LL_PrepareReceive(pdev,
                             epnum,
                             hcdc->RxBuffer[epnum-1],
                             64);
 return USBD_OK;
}

uint8_t  USBD_HID_SetRxBuffer  (USBD_HandleTypeDef   *pdev,
                                   uint8_t  *pbuff,
                                   uint8_t ep)
{
  USBD_HID_HandleTypeDef   *hcdc = (USBD_HID_HandleTypeDef*) pdev->pClassData;
  
  hcdc->RxBuffer[ep] = pbuff;
  
  return USBD_OK;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
static uint8_t  *USBD_HID_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
