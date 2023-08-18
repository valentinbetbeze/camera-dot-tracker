/**
 * @file OV7670_register_control.c
 * @author valentin betbeze (valentin.betbeze@gmail.com)
 * @brief Source file for register control functions
 * @date 2023-08-10
 */

#include "OV7670_regctrl.h"

#define I2C_TIMEOUT         (25)            // ms timeout


/**
 * @brief OV7670 Camera module base adresses.
 */
#define ADDR_WRITE              (0x42)
#define ADDR_READ               (0x43)


/* Register control ***********************************************************/

/**
 * @brief Driver's I2C communication handle variable
 */
I2C_HandleTypeDef OV7670_hi2c;

/**
 * @brief Read the content of a register (1 byte).
 * 
 * @param reg Register to read
 * @param data Buffer to store the value of the register
 * 
 * @note Read operation uses I2C in polling mode.
 */
static void OV7670_read_register(uint8_t reg, uint8_t *data)
{
    assert_param(data != NULL);
    HAL_CHECK(HAL_I2C_Master_Transmit(&OV7670_hi2c, ADDR_WRITE,
                                      &reg, 1, I2C_TIMEOUT));
    HAL_CHECK(HAL_I2C_Master_Receive(&OV7670_hi2c, ADDR_READ,
                                     data, 1, I2C_TIMEOUT));
}

/**
 * @brief Write a byte onto a given register.
 * 
 * @param reg Register to update
 * @param data Value to write
 */
static void OV7670_write_register(uint8_t reg, uint8_t data)
{
    HAL_CHECK(HAL_I2C_Mem_Write(&OV7670_hi2c, ADDR_WRITE,
                                reg, 1, &data, 1, I2C_TIMEOUT));
}

/**
 * @brief Enable a parameter without affecting the rest of the register.
 * 
 * @param reg Register containing the parameter
 * @param data Parameter to enable
 */
static void OV7670_enable_parameter(uint8_t reg, uint8_t data)
{
    // Get current register value
    uint8_t regdata = 0;
    OV7670_read_register(reg, &regdata);
    // Update register's content
    regdata |= data;
    // Send new register value
    OV7670_write_register(reg, regdata);
}

/**
 * @brief Disable a parameter without affecting the rest of the register.
 * 
 * @param reg Register containing the parameter
 * @param data Parameter to disable
 */
static void OV7670_disable_parameter(uint8_t reg, uint8_t data)
{
    // Get current register value
    uint8_t regdata = 0;
    OV7670_read_register(reg, &regdata);
    // Update register's content
    regdata &= ~data;
    // Send new register value
    OV7670_write_register(reg, regdata);
}

// Gain between 0 and 1023
void OV7670_set_gain(uint16_t word)
{
    OV7670_write_register(ADDR_GAIN, word & 0xFFU);
    // data buffer for read operation
    uint8_t rd;
    OV7670_read_register(ADDR_VREF, &rd);
    rd = ((word>>8) & 0x1U) ? rd | (1U<<6) : rd & ~(1U<<6);
    rd = ((word>>9) & 0x1U) ? rd | (1U<<7) : rd & ~(1U<<7);
    OV7670_write_register(ADDR_VREF, rd);
}

void OV7670_set_blue_gain(uint8_t byte)
{
    OV7670_write_register(ADDR_BLUE, byte);
}

void OV7670_set_red_gain(uint8_t byte)
{
    OV7670_write_register(ADDR_RED, byte);
}

void OV7670_enable_CCIR656_format(void)
{
    OV7670_enable_parameter(ADDR_COM1, CCIR656);
}

void OV7670_disable_CCIR656_format(void)
{
    OV7670_disable_parameter(ADDR_COM1, CCIR656);
}

void OV7670_reset_camera(void)
{
    OV7670_write_register(ADDR_COM7, RESET);
    HAL_Delay(1);
}

void OV7670_sleep_camera(void)
{
    OV7670_write_register(ADDR_COM2, STANDBY);
}

void OV7670_enable_hflip(void)
{
    OV7670_enable_parameter(ADDR_MVFP, HFLIP);
}

void OV7670_disable_hflip(void)
{
    OV7670_disable_parameter(ADDR_MVFP, HFLIP);
}

void OV7670_enable_vflip(void)
{
    OV7670_enable_parameter(ADDR_MVFP, VFLIP);
}

void OV7670_disable_vflip(void)
{
    OV7670_disable_parameter(ADDR_MVFP, VFLIP);
}

void OV7670_set_INTCLK_div(uint8_t plldiv)
{
    assert_param(plldiv < 128);
    if (plldiv < 2) {
        OV7670_disable_parameter(ADDR_CLKRC, (1U<<6));
    }
    OV7670_write_register(ADDR_CLKRC, (plldiv/2)-1);
}

void OV7670_set_INTCLK_mul(uint8_t pllmul)
{
    uint8_t regdata = 0;
    OV7670_read_register(ADDR_DBLV, &regdata);

    switch (pllmul) {
        case 1:
            regdata &= ~(1U<<6);
            regdata &= ~(1U<<7);
            break;
        case 4:
            regdata |= (1U<<6);
            regdata &= ~(1U<<7);
            break;
        case 6:
            regdata &= ~(1U<<6);
            regdata |= (1U<<7);
            break;
        case 8:
            regdata |= (1U<<7) | (1U<<6);
            break;
        default:
            assert_param(0);
            return;
    }
    OV7670_write_register(ADDR_DBLV, regdata);
}

