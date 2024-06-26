//
// Created by xtx on 24-2-21.
//

#include "main.h"
/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <esp_log.h>
#include <stdio.h>
#include <inttypes.h>
#include <soc/clk_tree_defs.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"


#define BLINK_GPIO_UP 5
#define BLINK_GPIO_DOWN 6
#define TAG "app_main"
#define LED_STRIP_LED_NUMBERS 63

void app_main(void) {
	//初始化上层灯
	led_strip_config_t strip_config_up = {
		.strip_gpio_num = BLINK_GPIO_UP, // The GPIO that connected to the LED strip's data line
		.max_leds = 63, // The number of LEDs in the strip,
		.led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
		.led_model = LED_MODEL_WS2812, // LED strip model
		.flags.invert_out = false,
		// whether to invert the output signal (useful when your hardware has a level inverter)
	};

	led_strip_rmt_config_t rmt_config_up = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
		.rmt_channel = 0,
#else
		.clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
		.resolution_hz = 10 * 1000 * 1000, // 10MHz
		.flags.with_dma = true, // whether to enable the DMA feature
#endif
	};
	ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config_up, &rmt_config_up, &led_strip_up));

	//初始化下层灯
	led_strip_config_t strip_config_down = {
		.strip_gpio_num = BLINK_GPIO_DOWN, // The GPIO that connected to the LED strip's data line
		.max_leds = 63, // The number of LEDs in the strip,
		.led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
		.led_model = LED_MODEL_WS2812, // LED strip model
		.flags.invert_out = false,
		// whether to invert the output signal (useful when your hardware has a level inverter)
	};

	led_strip_rmt_config_t rmt_config_down = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
		.rmt_channel = 0,
#else
		.clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
		.resolution_hz = 10 * 1000 * 1000, // 10MHz
		.flags.with_dma = true, // whether to enable the DMA feature
#endif
	};
	ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config_down, &rmt_config_down, &led_strip_down));

	ESP_LOGI(TAG, "Start blinking LED strip");
	bool led_on_off= true;
	while (1) {
		if (led_on_off) {
			/* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
			for (int i = 0; i < LED_STRIP_LED_NUMBERS; i++) {
				ESP_ERROR_CHECK(led_strip_set_pixel(led_strip_down, i, 5, 5, 5));
			}
			/* Refresh the strip to send data */
			ESP_ERROR_CHECK(led_strip_refresh(led_strip_down));
			ESP_LOGI(TAG, "LED ON!");
		} else {
			/* Set all LED off to clear all pixels */
			ESP_ERROR_CHECK(led_strip_clear(led_strip_down));
			ESP_LOGI(TAG, "LED OFF!");
		}

		led_on_off = !led_on_off;
		vTaskDelay(pdMS_TO_TICKS(1500));
	}
}
