// build/rorc_registers.h: RORC Register Definitions
//
// Note:
// This file was automatically generated from:
// src/packages/registers.vhd
// Repository is: git repository
// Repository Revision: 1fc8ea9929828cc0949e885eb6a712355737e2b1
// Repository Status: clean
// Build Timestamp: Tue Jan 16 16:14:13 CET 2018
// Build System: hutter@eda02
#define RORC_C_BUILD_REPO 1
#define RORC_C_BUILD_DATE 1516115653
#define RORC_C_BUILD_REVISION 1fc8ea9929828cc0949e885eb6a712355737e2b1
#define RORC_C_BUILD_STATUS_CLEAN 1
#define RORC_C_BUILD_HOST 65646130320000000000000000000000
#define RORC_C_BUILD_USER 68757474657200000000000000000000
#define RORC_C_HARDWARE_VERSION 26
#define RORC_CHANNEL_OFFSET 0x00008000
#define RORC_DMA_CMP_SEL 13
#define RORC_C_LINK_SEL 5
#define RORC_REG_HARDWARE_INFO 0
#define RORC_REG_BUILD_FLAGS 1
#define RORC_REG_N_CHANNELS 2
#define RORC_REG_SC_REQ_CANCELED 3
#define RORC_REG_DMA_TX_TIMEOUT 4
#define RORC_REG_ILLEGAL_REQ 5
#define RORC_REG_MULTIDWREAD 6
#define RORC_REG_PCIE_CTRL 7
#define RORC_REG_SYS_PERF_INT 8
#define RORC_REG_MC_CNT_CFG 9
#define RORC_REG_APP_CFG 11
#define RORC_REG_APP_STS 12
#define RORC_REG_BUILD_DATE_L 13
#define RORC_REG_BUILD_DATE_H 14
#define RORC_REG_BUILD_REV_0 15
#define RORC_REG_BUILD_REV_1 16
#define RORC_REG_BUILD_REV_2 17
#define RORC_REG_BUILD_REV_3 18
#define RORC_REG_BUILD_REV_4 19
#define RORC_REG_BUILD_HOST_0 20
#define RORC_REG_BUILD_HOST_1 21
#define RORC_REG_BUILD_HOST_2 22
#define RORC_REG_BUILD_HOST_3 23
#define RORC_REG_BUILD_USER_0 24
#define RORC_REG_BUILD_USER_1 25
#define RORC_REG_BUILD_USER_2 26
#define RORC_REG_BUILD_USER_3 27
#define RORC_REG_PERF_PCI_NRDY 28
#define RORC_REG_PERF_PCI_TRANS 29
#define RORC_REG_PERF_PCI_MAX_NRDY 30
#define RORC_REG_EBDM_N_SG_CONFIG 0
#define RORC_REG_EBDM_BUFFER_SIZE_L 1
#define RORC_REG_EBDM_BUFFER_SIZE_H 2
#define RORC_REG_RBDM_N_SG_CONFIG 3
#define RORC_REG_RBDM_BUFFER_SIZE_L 4
#define RORC_REG_RBDM_BUFFER_SIZE_H 5
#define RORC_REG_EBDM_SW_READ_POINTER_L 6
#define RORC_REG_EBDM_SW_READ_POINTER_H 7
#define RORC_REG_RBDM_SW_READ_POINTER_L 8
#define RORC_REG_RBDM_SW_READ_POINTER_H 9
#define RORC_REG_DMA_CTRL 10
#define RORC_REG_PERF_N_EVENTS 11
#define RORC_REG_EBDM_FPGA_WRITE_POINTER_L 12
#define RORC_REG_EBDM_FPGA_WRITE_POINTER_H 13
#define RORC_REG_RBDM_FPGA_WRITE_POINTER_L 14
#define RORC_REG_RBDM_FPGA_WRITE_POINTER_H 15
#define RORC_REG_SGENTRY_ADDR_LOW 16
#define RORC_REG_SGENTRY_ADDR_HIGH 17
#define RORC_REG_SGENTRY_LEN 18
#define RORC_REG_SGENTRY_CTRL 19
#define RORC_REG_PERF_DMA_STALL 20
#define RORC_REG_MISC_CFG 21
#define RORC_REG_MISC_STS 22
#define RORC_REG_EBDM_OFFSET_L 23
#define RORC_REG_EBDM_OFFSET_H 24
#define RORC_REG_DESC_CNT_L 25
#define RORC_REG_DESC_CNT_H 26
#define RORC_REG_PERF_INTERVAL 27
#define RORC_REG_PERF_EBUF_STALL 28
#define RORC_REG_PERF_RBUF_STALL 29
#define RORC_REG_GTX_DATAPATH_CFG 0
#define RORC_REG_GTX_LINK_STS 1
#define RORC_REG_GTX_PERF_INTERVAL 2
#define RORC_REG_GTX_PERF_PKT_AFULL 3
#define RORC_REG_LINK_FLIM_HW_INFO 0
#define RORC_REG_FLIM_BUILD_FLAGS 1
#define RORC_REG_FLIM_BUILD_DATE_L 2
#define RORC_REG_FLIM_BUILD_DATE_H 3
#define RORC_REG_FLIM_BUILD_REV_0 4
#define RORC_REG_FLIM_BUILD_REV_1 5
#define RORC_REG_FLIM_BUILD_REV_2 6
#define RORC_REG_FLIM_BUILD_REV_3 7
#define RORC_REG_FLIM_BUILD_REV_4 8
#define RORC_REG_FLIM_BUILD_HOST_0 9
#define RORC_REG_FLIM_BUILD_HOST_1 10
#define RORC_REG_FLIM_BUILD_HOST_2 11
#define RORC_REG_FLIM_BUILD_HOST_3 12
#define RORC_REG_FLIM_BUILD_USER_0 13
#define RORC_REG_FLIM_BUILD_USER_1 14
#define RORC_REG_FLIM_BUILD_USER_2 15
#define RORC_REG_FLIM_BUILD_USER_3 16
#define RORC_REG_LINK_FLIM_CFG 17
#define RORC_REG_LINK_FLIM_STS 18
#define RORC_REG_LINK_MC_INDEX_L 19
#define RORC_REG_LINK_MC_INDEX_H 20
#define RORC_REG_LINK_MC_TIME_L 21
#define RORC_REG_LINK_MC_TIME_H 22
#define RORC_REG_LINK_MC_PGEN_CFG 23
#define RORC_REG_LINK_MC_PGEN_MC_SIZE 24
#define RORC_REG_LINK_MC_PGEN_IDS 25
#define RORC_REG_LINK_MC_PGEN_MC_PENDING 26
#define RORC_REG_LINK_MAX_MC_WORDS 27
#define RORC_REG_LINK_TEST 31
