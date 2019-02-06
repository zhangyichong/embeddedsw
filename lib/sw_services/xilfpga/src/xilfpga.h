/******************************************************************************
 * Copyright (C) 2018-2019 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 *
 *
 ******************************************************************************/
/*****************************************************************************/
/**
 *
 * @file xilfpga.h
 * @addtogroup xfpga_apis XilFPGA APIs
 * @{
 *
 * Xilfpga Error format:
 * Lower level Errors + Interface specific Errors + Xilfpga top layer Errors
 *----------------------------------------------------------------------------
 * Lower level Errors | Interface Specific Errors | Xilfpga top layer Errors
 * (other libarier    |     (PCAP/CFI Interface)  |
 * or drivers         |                           |
 * Used by xilfpga)   |                           |
 * ----------------------------------------------------------------------------
 * 31 - 16 bits       |     15 - 8 bits           |         7 - 0 bits
 * ----------------------------------------------------------------------------
 * Xilfpga Top Layer:
 *	The functionality exist in this layers is completely Interface agnostic.
 * It provides a unique interface to load the Bitstream  across multiple
 * platforms.(ie; ZynqMP and Versel)
 *
 * Interface Specific layer:
 * 	This layer is responsible for providing the interface specific related
 * errors.
 *	-->In Case of ZynqMp, it provides the errors related to PCAP Interface.
 *	-->In Case of Versel, it provides the errors related to CFI Interface.
 *
 * Xilfpga lower layer:
 *	This layer is responsible for providing the Error related  to the lower
 * level drivers used by Interface layer.
 *
 * @{
 * @cond xilfpga_internal
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- ---- -------- -------------------------------------------------------
 * 4.2   Nava  08/06/16 Refactor the xilfpga library to support
 *                      different PL programming Interfaces.
 * 4.2   adk   11/07/18 Added support for readback of PL configuration data.
 * 4.2   Nava  16/08/18 Modified the PL data handling Logic to support
 *                      different PL programming interfaces.
 * 4.2   adk   28/08/18 Fixed misra-c required standard violations.
 * 4.2   Nava  15/09/18 Fixed global function call-backs issue.
 * 5.0   Nava  11/05/18 Added full bitstream loading support for versal Platform.
 * 5.0   Div   21/01/19 Fixed misra-c required standard violations.
 * </pre>
 *
 * @note
 *
 ******************************************************************************/
#ifndef XILFPGA_H
#define XILFPGA_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "xil_io.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xfpga_config.h"
#if defined(PLATFORM_ZYNQMP) || (PSU_PMU)
#include "xilfpga_pcap.h"
#include "xsecure.h"
#else
#include "xilfpga_cfi.h"
#endif

/**************************** Type Definitions *******************************/
/**
 * @XFpga_ValidateBitstream:	validate the Bitstream header before
 *				programming the PL
 * @xilfpga_PreConfig:		prepare the FPGA to receive confuration data
 * @xilfpga_WriteToPl:		write count bytes of configuration data to
 *				the FPGA
 * @xilfpga_PostConfig:		set FPGA to operating state after writing
 *				is done
 * @XFpga_GetInterfaceStatus:	Provides the STATUS of PL programming interface
 * @Xfpga_GetConfigReg:		Returns the value of the specified configuration
 *				register
 * @XFpga_GetConfigData:	Provides the FPGA readback data.
 * @PLInfo:			Which is used to store the secure image data.
 * @WriteInfoPtr:		Pointer to XFpga_Write structure.
 * @XFpga_Read:  		Pointer to XFpga_Read structure.
 */
typedef struct XFpgatag{
	u32 (*XFpga_ValidateBitstream)(struct XFpgatag *InstancePtr);
	u32 (*XFpga_PreConfig)(const struct XFpgatag *InstancePtr);
	u32 (*XFpga_WriteToPl)(struct XFpgatag *InstancePtr);
	u32 (*XFpga_PostConfig)(const struct XFpgatag *InstancePtr);
	u32 (*XFpga_GetInterfaceStatus)(void);
	u32 (*XFpga_GetConfigReg)(const struct XFpgatag *InstancePtr);
	u32 (*XFpga_GetConfigData)(const struct XFpgatag *InstancePtr);
	void (* XFpga_GlobSeqWriteReg)(struct XFpgatag *InstancePtr, u32 Mask, u32 Val);
	XFpga_Info	PLInfo;
	XFpga_Write	*WriteInfoPtr;
	XFpga_Read	*ReadInfoPtr;
}XFpga;
/************************** Variable Definitions *****************************/
/***************** Macros (Inline Functions) Definitions *********************/
#define XFPGA_SUCCESS			(0x0U)
#define XFPGA_FAILURE			(0x1U)
#define XFPGA_VALIDATE_ERROR		(0x2U)
#define XFPGA_PRE_CONFIG_ERROR		(0x3U)
#define XFPGA_WRITE_BITSTREAM_ERROR	(0x4U)
#define XFPGA_POST_CONFIG_ERROR		(0x5U)
#define XFPGA_OPS_NOT_IMPLEMENTED	(0x6U)

#define XFPGA_FULLBIT_EN			(0x00000000U)
#define XFPGA_PARTIAL_EN			(0x00000001U)
#define XFPGA_AUTHENTICATION_DDR_EN		(0x00000002U)
#define XFPGA_AUTHENTICATION_OCM_EN		(0x00000004U)
#define XFPGA_ENCRYPTION_USERKEY_EN		(0x00000008U)
#define XFPGA_ENCRYPTION_DEVKEY_EN		(0x00000010U)
#define XFPGA_ONLY_BIN_EN			(0x00000020U)

#define XFPGA_SECURE_FLAGS	(				\
				XFPGA_AUTHENTICATION_DDR_EN	\
				| XFPGA_AUTHENTICATION_OCM_EN	\
				| XFPGA_ENCRYPTION_USERKEY_EN	\
				| XFPGA_ENCRYPTION_DEVKEY_EN	\
				)

#define XFPGA_AUTH_ENC_USERKEY_DDR	(				\
					XFPGA_AUTHENTICATION_DDR_EN	\
					| XFPGA_ENCRYPTION_USERKEY_EN	\
					)

#define XFPGA_AUTH_ENC_DEVKEY_DDR	(				\
					XFPGA_AUTHENTICATION_DDR_EN	\
					| XFPGA_ENCRYPTION_DEVKEY_EN	\
					)

#define XFPGA_AUTH_ENC_USERKEY_OCM	(				\
					XFPGA_AUTHENTICATION_OCM_EN	\
					| XFPGA_ENCRYPTION_USERKEY_EN	\
					)

#define XFPGA_AUTH_ENC_DEVKEY_OCM	(				\
					XFPGA_AUTHENTICATION_OCM_EN	\
					| XFPGA_ENCRYPTION_DEVKEY_EN	\
					)

#define Xfpga_Printf(DebugType, ...) \
	if (DebugType != 0U) \
	{xil_printf (__VA_ARGS__); }

#define XFPGA_ERR_MASK				(0xFFU)
#define XFPGA_ERR_INTERFACE_MASK	(0xFFFFFF00U)
#define XFPGA_UPDATE_ERR(XfpgaErr, InterfaceErr) \
		(((InterfaceErr)&XFPGA_ERR_INTERFACE_MASK) + \
		 ((XfpgaErr)&XFPGA_ERR_MASK))

/** @endcond*/
/************************** Function Prototypes ******************************/
u32 XFpga_Initialize(XFpga *InstancePtr);
#if defined(PLATFORM_ZYNQMP) || (PSU_PMU)
u32 XFpga_PL_BitStream_Load(UINTPTR BitstreamImageAddr,
			    UINTPTR AddrPtr, u32 Flags);
u32 XFpga_PL_Preconfig(const XFpga_Info *PLInfoPtr);
u32 XFpga_PL_WriteToPl(const XFpga_Info *PLInfoPtr);
u32 XFpga_PL_PostConfig(const XFpga_Info *PLInfoPtr);
u32 XFpga_PL_ValidateImage(XFpga_Info *PLInfoPtr);
u32 XFpga_GetPlConfigData(const XFpga_Info *PLInfoPtr);
u32 XFpga_GetPlConfigReg(u32 ConfigReg, UINTPTR Address);
u32 XFpga_InterfaceStatus(void);
#else
u32 XFpga_PL_BitStream_Load(XFpga *InstancePtr,
			    UINTPTR BitstreamImageAddr,
			    UINTPTR AddrPtr_Size, u32 Flags);
u32 XFpga_PL_Preconfig(XFpga *InstancePtr);
u32 XFpga_PL_Write(XFpga *InstancePtr,UINTPTR BitstreamImageAddr,
		   UINTPTR AddrPtr_Size, u32 Flags);
u32 XFpga_PL_PostConfig(XFpga *InstancePtr);
u32 XFpga_PL_ValidateImage(XFpga *InstancePtr,
			   UINTPTR BitstreamImageAddr,
			   UINTPTR AddrPtr_Size, u32 Flags);
u32 XFpga_GetPlConfigData(XFpga *InstancePtr, UINTPTR ReadbackAddr,
			  u32 ConfigReg_NumFrames);
void XFpga_GetDmaPtr(XFpga *InstancePtr, XCsuDma *DmaPtr);
u32 XFpga_GlobSeqWriteReg(XFpga *InstancePtr, u32 Mask, u32 Val);
#endif

#ifdef __cplusplus
}
#endif

#endif  /* XILFPGA_H */
