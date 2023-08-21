/**
 * @file OV7670_register_control.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for register control commands of the 0V7670 camera module.
 * @date 2023-08-10
 * 
 * @note I'm creating those macros as I develop and use the driver. Hence, some
 * commands may be missing. Feel free to look at the register map in the OV7670
 * datasheet to find the right register and bits to apply for a given setting.
 */

#ifndef __OV7670_REGISTER_CONTROL__
#define __OV7670_REGISTER_CONTROL__

#ifdef __cplusplus
extern "C" {
#endif

#include "OV7670_config.h"


/* Registers ******************************************************************/

/**
 * @brief Register adresses
 */
#define ADDR_GAIN               (0x00U)
#define ADDR_BLUE               (0x01U)
#define ADDR_RED                (0x02U)
#define ADDR_VREF               (0x03U)
#define ADDR_COM1               (0x04U)
#define ADDR_BAVE               (0x05U)
#define ADDR_GbAVE              (0x06U)
#define ADDR_AECHH              (0x07U)
#define ADDR_RAVE               (0x08U)
#define ADDR_COM2               (0x09U)
#define ADDR_PID                (0x0AU)
#define ADDR_VER                (0x0BU)
#define ADDR_COM3               (0x0CU)
#define ADDR_COM4               (0x0DU)
#define ADDR_COM5               (0x0EU)
#define ADDR_COM6               (0x0FU)
#define ADDR_AECH               (0x10U)
#define ADDR_CLKRC              (0x11U)
#define ADDR_COM7               (0x12U)
#define ADDR_COM8               (0x13U)
#define ADDR_COM9               (0x14U)
#define ADDR_COM10              (0x15U)
// 0x16                         Reserved
#define ADDR_HSTART             (0x17U)
#define ADDR_HSTOP              (0x18U)
#define ADDR_VSTRT              (0x19U)
#define ADDR_VSTOP              (0x1AU)
#define ADDR_PSHFT              (0x1BU)
#define ADDR_MIDH               (0x1CU)
#define ADDR_MIDL               (0x1DU)
#define ADDR_MVFP               (0x1EU)
// 0x1F                         Reserved
#define ADDR_ADCCTR0            (0x20U)
// 0x21-0x23                    Reserved
#define ADDR_AEW                (0x24U)
#define ADDR_AEB                (0x25U)
#define ADDR_VPT                (0x26U)
#define ADDR_BBIAS              (0x27U)
#define ADDR_GbBIAS             (0x28U)
// 0x29                         Reserved
#define ADDR_EXHCH              (0x2AU)
#define ADDR_EXHCL              (0x2BU)
#define ADDR_RBIAS              (0x2CU)
#define ADDR_ADVFL              (0x2DU)
#define ADDR_ADVFH              (0x2EU)
#define ADDR_YAVE               (0x2FU)
#define ADDR_HSYST              (0x30U)
#define ADDR_HSYEN              (0x31U)
#define ADDR_HREF               (0x32U)
#define ADDR_CHLF               (0x33U)
#define ADDR_ARBLM              (0x34U)
// 0x35-0x36                    Reserved
#define ADDR_ADC                (0x37U)   // ADC control
#define ADDR_ACOM               (0x38U)
#define ADDR_OFON               (0x39U)
#define ADDR_TSLB               (0x3AU)
#define ADDR_COM11              (0x3BU)
#define ADDR_COM12              (0x3CU)
#define ADDR_COM13              (0x3DU)
#define ADDR_COM14              (0x3EU)
#define ADDR_EDGE               (0x3FU)
#define ADDR_COM15              (0x40U)
#define ADDR_COM16              (0x41U)
#define ADDR_COM17              (0x42U)
#define ADDR_AWBC1              (0x43U)
#define ADDR_AWBC2              (0x44U)
#define ADDR_AWBC3              (0x45U)
#define ADDR_AWBC4              (0x46U)
#define ADDR_AWBC5              (0x47U)
#define ADDR_AWBC6              (0x48U)
// 0x49-0x4A                    Reserved
#define ADDR_REG4B              (0x4BU)
#define ADDR_DNSTH              (0x4CU)
#define ADDR_DM_POS             (0x4DU)
// 0x4E                         Reserved
#define ADDR_MTX1               (0x4FU)
#define ADDR_MTX2               (0x50U)
#define ADDR_MTX3               (0x51U)
#define ADDR_MTX4               (0x52U)
#define ADDR_MTX5               (0x53U)
#define ADDR_MTX6               (0x54U)
#define ADDR_BRIGHT             (0x55U)
#define ADDR_CONTRAST           (0x56U)
#define ADDR_CONTRASCENTER      (0x57U)
#define ADDR_MTXS               (0x58U)
#define ADDR_AWBC7              (0x59U)
#define ADDR_AWBC8              (0x5AU)
#define ADDR_AWBC9              (0x5BU)
#define ADDR_AWBC10             (0x5CU)
#define ADDR_AWBC11             (0x5DU)
#define ADDR_AWBC12             (0x5EU)
#define ADDR_B_LMT              (0x5FU)
#define ADDR_R_LMT              (0x60U)
#define ADDR_G_LMT              (0x61U)
#define ADDR_LCC1               (0x62U)
#define ADDR_LCC2               (0x63U)
#define ADDR_LCC3               (0x64U)
#define ADDR_LCC4               (0x65U)
#define ADDR_LCC5               (0x66U)
#define ADDR_MANU               (0x67U)
#define ADDR_MANV               (0x68U)
#define ADDR_GFIX               (0x69U)
#define ADDR_GGAIN              (0x6AU)
#define ADDR_DBLV               (0x6BU)
#define ADDR_AWBCTR3            (0x6CU)
#define ADDR_AWBCTR2            (0x6DU)
#define ADDR_AWBCTR1            (0x6EU)
#define ADDR_AWBCTR0            (0x6FU)
#define ADDR_SCALING_XSC        (0x70U)
#define ADDR_SCALING_YSC        (0x71U)
#define ADDR_SCALING_DCWCTR     (0x72U)
#define ADDR_SCALING_PCLK_DIV   (0x73U)
#define ADDR_REG74              (0x74U)
#define ADDR_REG75              (0x75U)
#define ADDR_REG76              (0x76U)
#define ADDR_REG77              (0x77U)
// 0x78-0x79                    Reserved
#define ADDR_SLOP               (0x7AU)
#define ADDR_GAM1               (0x7BU)
#define ADDR_GAM2               (0x7CU)
#define ADDR_GAM3               (0x7DU)
#define ADDR_GAM4               (0x7EU)
#define ADDR_GAM5               (0x7FU)
#define ADDR_GAM6               (0x80U)
#define ADDR_GAM7               (0x81U)
#define ADDR_GAM8               (0x82U)
#define ADDR_GAM9               (0x83U)
#define ADDR_GAM10              (0x84U)
#define ADDR_GAM11              (0x85U)
#define ADDR_GAM12              (0x86U)
#define ADDR_GAM13              (0x87U)
#define ADDR_GAM14              (0x88U)
#define ADDR_GAM15              (0x89U)
// 0x8A-0x91                    Reserved
#define ADDR_DM_LNL             (0x92U)
#define ADDR_DM_LNH             (0x93U)
#define ADDR_LCC6               (0x94U)
#define ADDR_LCC7               (0x95U)
// 0x96-0x9C                    Reserved
#define ADDR_BD50ST             (0x9DU)
#define ADDR_BD60ST             (0x9EU)
#define ADDR_HRL                (0x9FU)
#define ADDR_LRL                (0xA0U)
#define ADDR_DSPC3              (0xA1U)
#define ADDR_SCALING_PCLK_DELAY (0xA2U)
// 0xA3                         Reserved
#define ADDR_NT_CTRL            (0xA4U)
#define ADDR_AECGMAX            (0xA5U)
#define ADDR_LPH                (0xA6U)
#define ADDR_UPL                (0xA7U)
#define ADDR_TPL                (0xA8U)
#define ADDR_TPH                (0xA9U)
#define ADDR_NALG               (0xAAU)
// 0xAB                         Reserved
#define ADDR_STROPT             (0xACU)
#define ADDR_STR_R              (0xADU)
#define ADDR_STR_G              (0xAEU)
#define ADDR_STR_B              (0xAFU)
// 0xB0                         Reserved
#define ADDR_ABLC1              (0xB1U)
// 0xB2                         Reserved
#define ADDR_THL_ST             (0xB4U)
// 0xB4                         Reserved
#define ADDR_THL_DLT            (0xB5U)
// 0xB6-BD                      Reserved
#define ADDR_ADCHB              (0xBEU)
#define ADDR_ADCHR              (0xBFU)
#define ADDR_ADCHGb             (0xC0U)
#define ADDR_ADCHGr             (0xC1U)
// 0xC2-0xC8                    Reserved
#define ADDR_SATCTR             (0xC9U)

/**
 * @brief Register content
*/
#define RESET                   (1U<<7)     // Reset setting
#define STANDBY                 (1U<<4)     // Standby mode setting
#define CCIR656                 (1U<<6)     // CCIR656 format
#define HFLIP                   (1U<<5)     // Horizontal mirror
#define VFLIP                   (1U<<4)     // Vertical mirror


 /* Function prototypes ********************************************************/

 /**
  * @brief Set the gain //TODO: better descr pls
  * 
  * @param hword Gain value between 0-1023
  */
void OV7670_set_gain(uint16_t hword);
void OV7670_set_blue_gain(uint8_t byte);
void OV7670_set_red_gain(uint8_t byte);
void OV7670_enable_CCIR656_format(void);
void OV7670_disable_CCIR656_format(void);

/**
 * @brief Reset the camera module via SCCB
 * @note Another way to reset the module is to use the RST pin.
 */
void OV7670_reset_camera(void);
void OV7670_sleep_camera(void);
void OV7670_enable_hflip(void);
void OV7670_disable_hflip(void);
void OV7670_enable_vflip(void);
void OV7670_disable_vflip(void);

/**
 * @brief Internal clock PLL division factor.
 * 
 * @param plldiv Division factor
 * @note Division factor allowed between 2 and 128
 */
void OV7670_set_INTCLK_div(uint8_t plldiv);

/**
 * @brief Internal clock PLL multiplication factor.
 * 
 * @param pllmul Multiplication factor (4, 6, or 8)
 * @note * Factor allowed: 4, 6, 8
 * @note * Input of 1 disables the PLL multiplication factor.
 * @warning Forbidden inputs lead to a program abort.
 */
void OV7670_set_INTCLK_mul(uint8_t pllmul);

#ifdef __cplusplus
}
#endif

#endif // __OV7670_REGISTER_CONTROL__