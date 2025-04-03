/*
 * @Author: globalji 1048313827@qq.com
 * @Date: 2025-03-04 11:31:10
 * @LastEditors: globalji 1048313827@qq.com
 * @LastEditTime: 2025-03-05 11:29:37
 * @FilePath: \AFE_I2S_WAKEUP\main\I2S\MY_I2S.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "I2Sto.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define MIC_I2S_PIN_WS   GPIO_NUM_16
#define MIC_I2S_PIN_SCK  GPIO_NUM_15 
#define MIC_I2S_PIN_DIN  GPIO_NUM_17

#define BUFFER_SIZE 256
static i2s_chan_handle_t rx_handle = NULL;


/* I2S初始化 */
void my_i2s_init(void) {
    /* I2S通道配置 */
    i2s_chan_config_t chan_cfg = {
        .id = I2S_NUM_0,
        .role = I2S_ROLE_MASTER,
        .dma_desc_num = 8,
        .dma_frame_num = BUFFER_SIZE,
        .auto_clear = false,
    };
    /* 创建I2S通道 */
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

    /* I2S控制器标准模式配置 */
    i2s_std_config_t std_cfg = {
        .clk_cfg = {
            .sample_rate_hz = 16000,
            .clk_src = I2S_CLK_SRC_DEFAULT,
            .mclk_multiple = I2S_MCLK_MULTIPLE_256,
        },
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_16BIT,
            .slot_mode = I2S_SLOT_MODE_MONO,
            .slot_mask = I2S_STD_SLOT_LEFT,
            .ws_width = 16,
            .ws_pol = false,
            .bit_shift = true,
            .left_align = true,
            .big_endian = false,
            .bit_order_lsb = false,
        },
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = MIC_I2S_PIN_SCK,
            .ws = MIC_I2S_PIN_WS,
            .dout = I2S_GPIO_UNUSED,
            .din = MIC_I2S_PIN_DIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    /* 初始化I2S控制器标准模式 */
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));
    
    /* 启动I2S通道 */
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));
}

/* 读取I2S数据 */
esp_err_t my_i2s_read(int16_t *buffer, int length, size_t *bytes_read) {
    return i2s_channel_read(rx_handle, buffer, length, bytes_read, portMAX_DELAY);
}

/* 写入I2S数据 */