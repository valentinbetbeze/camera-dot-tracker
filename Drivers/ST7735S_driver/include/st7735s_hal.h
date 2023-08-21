/**
 * @file st7735s_hal.h
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Header file for the HAL functions of the ST7735S driver.
 * Compatible for use with: ESP32-WROOM-32.
 * @date 2023-04-16
 * 
 * @warning Do not modify any value between parenthesis '()'.
 */

#ifndef __ST7735S_HAL_H__
#define __ST7735S_HAL_H__


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "stm32f3xx_hal.h"
#include "main.h"


#define PIN_LCD_RST         GPIO_PIN_6
#define PIN_LCD_RS          GPIO_PIN_9

/*************************************************
 * ST7735S functions (System/Panel)
 *************************************************/
#define NOP                 (0x00)          // No Operation
#define SWRESET             (0x01)          // Software Reset
#define RDDID               (0x04)          //
#define RDDST               (0x09)          //
#define RDDPM               (0x0A)          //
#define RDDMADCTL           (0x0B)          //
#define RDDCOLMOD           (0x0C)          //
#define RDDIM               (0x0D)          //
#define RDDSM               (0x0E)          //
#define RDDSDR              (0x0F)          //
#define SLPIN               (0x10)          //
#define SLPOUT              (0x11)          // Sleep Out
#define PLTON               (0x12)          //
#define NORON               (0x13)          // Normal Mode On
#define INVOFF              (0x20)          // Display Inversion Off
#define INVON               (0x21)          //
#define GAMSET              (0x26)          // Set Gamma Curve
#define DISPOFF             (0x28)          //
#define DISPON              (0x29)          // Display On
#define CASET               (0x2A)          // Column Adress Set
#define RASET               (0x2B)          // Raw Adress Set
#define RAMWR               (0x2C)          //
#define RGBSET              (0x2D)          //
#define RAMRD               (0x2E)          //
#define PTLAR               (0x30)          //
#define SCRLAR              (0x33)          //
#define TEOFF               (0x34)          //
#define TEON                (0x35)          //
#define MADCTL              (0x36)          // Memory Data Access Control
#define VSCSAD              (0x37)          //
#define IDMOFF              (0x38)          //
#define IDMON               (0x39)          //
#define COLMOD              (0x3A)          // Interface Pixel Format
#define RDID1               (0xDA)          //
#define RDID2               (0xDB)          //
#define RDID3               (0xDC)          //

#define FRMCTR1             (0xB1)          // Frame Rate Control (NM/FC)
#define FRMCTR2             (0xB2)          //           
#define FRMCTR3             (0xB3)          //
#define INVCTR              (0xB4)          //
#define PWCTR1              (0xC0)          // Power Control 1
#define PWCTR2              (0xC1)          // Power Control 2
#define PWCTR3              (0xC2)          //
#define PWCTR4              (0xC3)          //
#define PWCTR5              (0xC4)          //
#define VMCTR1              (0xC5)          //
#define VMOFCTR             (0xC7)          //
#define WRID2               (0xD1)          //
#define WRID3               (0xD2)          //
#define NVCTR1              (0xD9)          //
#define NVCTR2              (0xDE)          //
#define NVCTR3              (0xDF)          //
#define GAMCTRP1            (0xE0)          //
#define GAMCTRN1            (0xE1)          //
#define GCV                 (0xFC)          //


/*************************************************
 * Display parameters
 ************************************************/
#define LCD_MEMORY_BASE     0b11            // Display resolution code
#define LCD_COLOR_FORMAT    (0x05)          // 16-bit/pixel            
#define LCD_RTNA            0x00
#define LCD_FPA             0x06
#define LCD_BPA             0x03
#define LCD_MH              0x00
#define LCD_RGB             0x00            // 0x00: RGB; 0x01: BGR    
#define LCD_ML              0x00
#define LCD_MV              0x00
#define LCD_MX              0x00            // X-Mirror                
#define LCD_MY              0x01            // Y-Mirror                
#define LCD_GAMMA           0x08            // Gamma Curve 4           

#if (LCD_MEMORY_BASE == 0b00)
    #define LCD_HEIGHT      (132)           /* pixels */
    #define LCD_WIDTH       (162)           /* pixels */
#elif (LCD_MEMORY_BASE == 0b01)
    #define LCD_HEIGHT      (132)           /* pixels */
    #define LCD_WIDTH       (132)           /* pixels */
#elif (LCD_MEMORY_BASE == 0b11)
    #define LCD_HEIGHT      (128)           /* pixels */
    #define LCD_WIDTH       (160)           /* pixels */ 
#else
    #error "LCD_MEMORY_BASE not recognized. Consult ST7735S datasheet."
#endif
#define LCD_NPIX            (LCD_HEIGHT * LCD_WIDTH)    // Number of pixels on the display
#define LCD_SIZE            hypot(LCD_HEIGHT, LCD_WIDTH)

#define TEXT_PADDING_X      1               // pixels                  
#define TEXT_PADDING_Y      3               // pixels   


/*************************************************
 * Frame dimensions
 ************************************************/

#define FRAME_SIZE          (20480)



/*************************************************
 * Extern variables
 *************************************************/

extern uint16_t frame[FRAME_SIZE];


/*************************************************
 * Prototypes
 *************************************************/

/**
 * @brief Initialize the TFT display.
 * 
 * @param[in] hspi SPI device handle of the display.
 */
void st7735s_init_tft(SPI_HandleTypeDef *hspi);

/**
 * @brief Send the frame to the ST7735S chip via SPI.
 * 
 * @param[in] hspi SPI device handle of the display.
 */
void st7735s_push_frame(SPI_HandleTypeDef *hspi);


#endif // __ST7735S_HAL_H__