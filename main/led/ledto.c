#include "ledto.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_OUTPUT_IO_0   5
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_0)

void led_init(void)
{
    gpio_config_t io_conf;
    // 禁用中断
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // 设置为输出模式
    io_conf.mode = GPIO_MODE_OUTPUT;
    // 选择要配置的引脚
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    // 禁用下拉模式
    io_conf.pull_down_en = 0;
    // 禁用上拉模式
    io_conf.pull_up_en = 0;
    // 调用gpio_config函数进行配置
    gpio_config(&io_conf);
}
void led_turn(int level){
    gpio_set_level(GPIO_OUTPUT_IO_0, level);
}
