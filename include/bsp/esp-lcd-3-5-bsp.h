#pragma once

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/sdmmc_host.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Pin configuration
 */

// LCD
#define BSP_LCD_SPI_CLK_PIN     17
#define BSP_LCD_SPI_MOSI_PIN    4
#define BSP_LCD_SPI_MISO_PIN    2
#define BSP_LCD_DC_PIN          5
#define BSP_LCD_CS_PIN          19
#define BSP_LCD_RST_PIN         18
#define BSP_LCD_BACKLIGHT_PIN   15
// Touch input
#define BSP_TOUCH_CS_PIN        16
// SD Card
#define BSP_SDCARD_CS_PIN       22
// I2C
#define BSP_LCD_I2C_SCL_PIN     23
#define BSP_LCD_I2C_SDA_PIN     21

// LCD config definitions
#define BSP_LCD_H_RES           (320)
#define BSP_LCD_V_RES           (480)
#define LCD_BUFFER_SIZE         (320 * 30)
#define BSP_LCD_PIXEL_CLOCK_HZ  (40 * 1000 * 1000)
// I2C definitions
#define BSP_LCD_I2C_CLK_SPEED_HZ 100000

// ESP32 Peripherals
#define BSP_LCD_I2C_NUM         I2C_NUM_0
#define BSP_SHARED_SPI_HOST     VSPI_HOST

/**************************************************************************************************
 *
 * I2C interface
 *
 * There are multiple devices connected to I2C periphera
 *
 * After initialization of I2C, use BSP_LCD_I2C_NUM macro when creating I2C devices drivers ie.:
 **************************************************************************************************/

/**
 * @brief Init I2C driver
 *
 */
esp_err_t bsp_lcd_i2c_init(void);

/**
 * @brief Deinit I2C driver and free its resources
 *
 */
esp_err_t bsp_lcd_i2c_deinit(void);

/**************************************************************************************************
 *
 * SD Card interface
 *
 * Utilize the shared SPI SD card on the LCD Display
 *
 **************************************************************************************************/

/**
 * @brief Mount SD card to virtual file system
 * Sets *perr to:
 *      - ESP_OK on success
 *      - ESP_ERR_INVALID_STATE if esp_vfs_fat_sdmmc_mount was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes from SDMMC or SPI drivers, SDMMC protocol, or FATFS drivers
 * @return
 *      - Pointer to mounted card
 */
sdmmc_card_t *bsp_lcd_sdcard_mount(const char *mount_point, esp_err_t *pErr);

/**
 * @brief Unmount SD card from virtual file system
 *
 * @return
 *      - ESP_OK on success
 *      - ESP_ERR_NOT_FOUND if the partition table does not contain FATFS partition with given label
 *      - ESP_ERR_INVALID_STATE if esp_vfs_fat_spiflash_mount was already called
 *      - ESP_ERR_NO_MEM if memory can not be allocated
 *      - ESP_FAIL if partition can not be mounted
 *      - other error codes from wear levelling library, SPI flash driver, or FATFS drivers
 */
esp_err_t bsp_lcd_sdcard_unmount(sdmmc_card_t* card, const char *mount_point);

/**************************************************************************************************
 *
 * LCD interface
 *
 * This display has 3.5 inch and ili9488 display controller.
 * It features 16-bit colors, 320x480 resolution and capacitive touch controller XPT2046.
 *
 * LVGL is used as graphics library. LVGL is NOT thread safe, therefore the user must take LVGL mutex
 * by calling bsp_lcd_lock() before calling and LVGL API (lv_...) and then give the mutex with
 * bsp_lcd_unlock().
 *
 **************************************************************************************************/

/**
 * @brief Initialize display
 *
 * This function initializes SPI, display controller and starts LVGL handling task.
 * Note that the SPI interface for the display-mounted SD card is also initialized.
 *
 * @param Amount of extra stack, in addition  to default stack
 * @return Pointer to LVGL display or NULL when error occured
 */
lv_disp_t *bsp_lcd_start(int extra_stack);

/**
 * @brief Set display brightness
 *
 * Set the brightness of the LCD display as integer percentage
 *
 */
void bsp_lcd_set_brightness(int brightness_percentage);

/**
 * @brief Take LVGL mutex
 *
 * @param timeout_ms Timeout in [ms]. 0 will block indefinitely.
 * @return true  Mutex was taken
 * @return false Mutex was NOT taken
 */
bool bsp_lcd_lock(uint32_t timeout_ms);

/**
 * @brief Give LVGL mutex
 *
 */
void bsp_lcd_unlock(void);


#ifdef __cplusplus
}
#endif
