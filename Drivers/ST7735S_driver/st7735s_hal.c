#include "st7735s_hal.h"



uint16_t frame[FRAME_SIZE];


/**
 * @brief Send a command to the ST7735S chip.
 * 
 * @param[in] hspi SPI device hspi of the display.
 * @param[in] command 8-bit command (see ST7735S datasheet p.104)
 */
static void send_command(SPI_HandleTypeDef *hspi, uint8_t command)
{   
    // Check if proper data size is configured
    if ((hspi->Instance->CR2 & SPI_CR2_DS_Msk) != SPI_DATASIZE_8BIT) {
        __HAL_SPI_DISABLE(hspi);
        SET_BIT(hspi->Instance->CR2, SPI_DATASIZE_8BIT & SPI_CR2_DS_Msk);
        __HAL_SPI_ENABLE(hspi);
    }
    // Enable command mode
    HAL_GPIO_WritePin(GPIOA, PIN_LCD_RS, GPIO_PIN_RESET);
    // Transmit command
    ERROR_CHECK(HAL_SPI_Transmit(hspi, &command, 1, 100));
}


/**
 * @brief Send a packet of 8-bit data to the ST7735S chip.
 * 
 * @param[in] hspi SPI device hspi of the display.
 * @param[in] data Pointer to the 8-bit data to be sent.
 * @param[in] len Amount of data in byte.
 * 
 * @note This function is useful to send command arguments to the ST7735S
 * driver, or 8-bit color format data. It is recommended to make use of the
 * send_words() function if sending 16-bit color format data.
 */
static void send_data(SPI_HandleTypeDef *hspi, uint8_t *data,
                      const uint16_t len)
{
    // Check if proper data size is configured
    if ((hspi->Instance->CR2 & SPI_CR2_DS_Msk) != SPI_DATASIZE_8BIT) {
        __HAL_SPI_DISABLE(hspi);
        SET_BIT(hspi->Instance->CR2, SPI_DATASIZE_8BIT & SPI_CR2_DS_Msk);
        __HAL_SPI_ENABLE(hspi);
    }
    // Enable data mode
    HAL_GPIO_WritePin(GPIOA, PIN_LCD_RS, GPIO_PIN_SET);
    // Transmit command
    ERROR_CHECK(HAL_SPI_Transmit(hspi, data, len, 100));
}


void st7735s_init_tft(SPI_HandleTypeDef *hspi)
{
    // Configure GPIOs
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef tft_gpio_init = {
        .Pin = PIN_LCD_RST | PIN_LCD_RS,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_LOW,
    };
    HAL_GPIO_Init(GPIOA, &tft_gpio_init);

    uint8_t parameter;
    // Hardware reset
    HAL_GPIO_WritePin(GPIOA, PIN_LCD_RST, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, PIN_LCD_RST, GPIO_PIN_SET);
    HAL_Delay(120);

    // Reset software
    send_command(hspi, SWRESET);
    HAL_Delay(120);

    // Sleep out
    send_command(hspi, SLPOUT);
    HAL_Delay(250);

    // Pixel format 16-bit
    send_command(hspi, COLMOD);
    parameter = LCD_COLOR_FORMAT;
    send_data(hspi, &parameter, sizeof(parameter));

    // Frame rate control
    send_command(hspi, FRMCTR1);
    uint8_t param_FRMCTRL1[3] = {LCD_RTNA, LCD_FPA, LCD_BPA};
    send_data(hspi, param_FRMCTRL1, sizeof(param_FRMCTRL1));

    // Memory data access control
    send_command(hspi, MADCTL);
    parameter = ((LCD_MY << 7) | (LCD_MX << 6) | (LCD_MV << 5) |
                (LCD_ML << 4) | (LCD_RGB << 3) | (LCD_MH << 2));
    send_data(hspi, &parameter, sizeof(parameter));

    // Display inversion off
    send_command(hspi, INVOFF);

    // Power control 1 - Default applied
    send_command(hspi, PWCTR1);
    uint8_t param_PWCTR1[3] = {0xA8, 0x08, 0x84};
    send_data(hspi, param_PWCTR1, sizeof(param_PWCTR1));

    // Power control 2 - Default applied
    send_command(hspi, PWCTR2);
    parameter = 0xC0;
    send_data(hspi, &parameter, sizeof(parameter));

    // VCOM control - Default applied
    send_command(hspi, VMCTR1);
    parameter = 0x05;
    send_data(hspi, &parameter, sizeof(parameter));

    // Display function control
    send_command(hspi, INVCTR);
    parameter = 0x00;
    send_data(hspi, &parameter, sizeof(parameter));

    // Gamma curve
    send_command(hspi, GAMSET);
    parameter = LCD_GAMMA; 
    send_data(hspi, &parameter, sizeof(parameter));

    // Normal display mode ON
    send_command(hspi, NORON);
    
    // Switch display on
    send_command(hspi, DISPON);

    // Set all columns
    send_command(hspi, CASET);
    uint8_t columns[4] = {0x00, 0x00, 0x00, 0x7F};
    send_data(hspi, columns, sizeof(columns));

    // Set all rows
    send_command(hspi, RASET);
    uint8_t rows[4] = {0x00, 0x00, 0x00, 0x9F};
    send_data(hspi, rows, sizeof(rows));
}


void st7735s_push_frame(SPI_HandleTypeDef *hspi)
{
    send_command(hspi, RAMWR);

    if ((hspi->Instance->CR2 & SPI_CR2_DS_Msk) != SPI_DATASIZE_16BIT) {
        __HAL_SPI_DISABLE(hspi);
        SET_BIT(hspi->Instance->CR2, SPI_DATASIZE_16BIT & SPI_CR2_DS_Msk);
        __HAL_SPI_ENABLE(hspi);
    }

    HAL_GPIO_WritePin(GPIOA, PIN_LCD_RS, GPIO_PIN_SET);
    ERROR_CHECK(HAL_SPI_Transmit_DMA(hspi, (uint8_t *)frame, sizeof(frame)));
}

