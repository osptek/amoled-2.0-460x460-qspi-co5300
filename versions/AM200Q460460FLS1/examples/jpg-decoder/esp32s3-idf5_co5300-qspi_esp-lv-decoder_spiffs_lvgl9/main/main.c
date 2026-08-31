/*
 * SPDX-FileCopyrightText: Copyright 2026 OSPTEK
 * SPDX-License-Identifier: CC-BY-4.0
 *
 * https://github.com/osptek
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

#include "driver/i2c_master.h"
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_cst820.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/spi_master.h"
#include "esp_lcd_co5300.h"
#include "co5300_init_cmds.h"

#include "esp_lv_adapter.h"
#include "lvgl.h"
#include "esp_lv_fs.h"
#include "esp_mmap_assets.h"
#include "mmap_generate_material.h"

static const char *TAG = "LVGL_CO5300_CST820_QSPI";

// ==================== 引脚定义（请根据你的硬件修改） ====================
#define EXAMPLE_PIN_NUM_LCD_CS      GPIO_NUM_9
#define EXAMPLE_PIN_NUM_LCD_SCK     GPIO_NUM_12
#define EXAMPLE_PIN_NUM_LCD_D0      GPIO_NUM_13   // QSPI D0
#define EXAMPLE_PIN_NUM_LCD_D1      GPIO_NUM_14   // QSPI D1
#define EXAMPLE_PIN_NUM_LCD_D2      GPIO_NUM_15   // QSPI D2
#define EXAMPLE_PIN_NUM_LCD_D3      GPIO_NUM_16   // QSPI D3
#define EXAMPLE_PIN_NUM_LCD_RST     GPIO_NUM_10
#define EXAMPLE_PIN_NUM_LCD_TE      GPIO_NUM_11

#define EXAMPLE_TOUCH_I2C_SCL       GPIO_NUM_40
#define EXAMPLE_TOUCH_I2C_SDA       GPIO_NUM_39
#define EXAMPLE_TOUCH_RST           GPIO_NUM_41
#define EXAMPLE_TOUCH_INT           GPIO_NUM_42

/* Touch settings */
#define EXAMPLE_TOUCH_I2C_NUM       I2C_NUM_0
#define EXAMPLE_TOUCH_I2C_CLK_HZ    (400 * 1000)

// ==================== 显示参数 ====================
#define EXAMPLE_LCD_HOST            SPI2_HOST
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ  (40 * 1000 * 1000)

#define EXAMPLE_LCD_H_RES           460
#define EXAMPLE_LCD_V_RES           460    
#define EXAMPLE_LCD_X_GAP           10
#define EXAMPLE_LCD_Y_GAP           0

// 挂载 SJPG 资产分区并注册 LVGL 文件系统
static esp_err_t mount_sjpg_assets(void)
{
    static mmap_assets_handle_t assets_handle = NULL;
    static esp_lv_fs_handle_t   fs_handle = NULL;

    // 如果已经挂载过，直接返回成功
    if (assets_handle != NULL) {
        ESP_LOGW(TAG, "SJPG Assets 已经挂载");
        return ESP_OK;
    }

    ESP_LOGI(TAG, "正在挂载 SJPG Assets...");

    // ==================== 1. 创建 mmap assets ====================
    const mmap_assets_config_t assets_cfg = {
        .partition_label = "material",  // ← 修改为你的分区 label
        .max_files       = MMAP_MATERIAL_FILES,   // 构建时自动生成的宏
        .checksum        = MMAP_MATERIAL_CHECKSUM,
        .flags = {
            .mmap_enable = true,
        },
    };

    ESP_ERROR_CHECK(mmap_assets_new(&assets_cfg, &assets_handle));

    // ==================== 2. 挂载为 LVGL 文件系统 ====================
    const fs_cfg_t fs_cfg = {
        .fs_letter = 'A',  // 盘符 A:
        .fs_assets = assets_handle,
        .fs_nums   = MMAP_MATERIAL_FILES,
    };

    ESP_ERROR_CHECK(esp_lv_adapter_fs_mount(&fs_cfg, &fs_handle));

    ESP_LOGI(TAG, "SJPG Assets 挂载成功！使用盘符 A:");
    return ESP_OK;
}

/**
 * @brief 启动 SJPG 图片播放器（演示使用 mmap_assets 加载 SJPG）
 * 
 * @param disp 当前显示器句柄
 */
static void start_sjpg_player(lv_display_t *disp)
{
    static bool assets_mounted = false;

    if (!assets_mounted) {
        ESP_LOGI(TAG, "正在初始化 SJPG 资源...");
        
        // 挂载资源（只执行一次）
        ESP_ERROR_CHECK(mount_sjpg_assets());
        assets_mounted = true;
    }

    ESP_LOGI(TAG, "启动 SJPG Player...");

    // ==================== 获取当前活动屏幕并设置为白色背景 ====================
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    if (scr) {
        lv_obj_remove_style_all(scr);                    // 清除原有样式
        lv_obj_set_style_bg_color(scr, lv_color_white(), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_STATE_DEFAULT);
        lv_obj_set_size(scr, LV_PCT(100), LV_PCT(100));
    }

    // ==================== 创建图片控件 ====================
    lv_obj_t *img = lv_image_create(scr);   // 使用已设置好背景的屏幕
    
    // 设置图片位置和大小
    lv_obj_center(img);
    lv_obj_set_size(img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    
    // 可选：添加边框或样式
    lv_obj_set_style_border_width(img, 2, 0);
    lv_obj_set_style_border_color(img, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_radius(img, 8, 0);

    // ==================== 加载 SJPG 图片 ====================
    lv_img_set_src(img, "A:logo_1.sjpg");           // ← 修改为你实际的文件名

    ESP_LOGI(TAG, "SJPG Player 启动完成，图片已显示");
}

void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-S3 + CO5300 (QSPI) + CST820 + LVGL9 开始初始化...");

    // 1. QSPI 总线初始化
    spi_bus_config_t bus_config = {
        .sclk_io_num = EXAMPLE_PIN_NUM_LCD_SCK,
        .data0_io_num = EXAMPLE_PIN_NUM_LCD_D0,
        .data1_io_num = EXAMPLE_PIN_NUM_LCD_D1,
        .data2_io_num = EXAMPLE_PIN_NUM_LCD_D2,
        .data3_io_num = EXAMPLE_PIN_NUM_LCD_D3,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * 2,
        .flags = SPICOMMON_BUSFLAG_QUAD,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(EXAMPLE_LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));

    // 2. LCD Panel IO (QSPI)
    esp_lcd_panel_io_spi_config_t io_config = {
        .cs_gpio_num = EXAMPLE_PIN_NUM_LCD_CS,
        .dc_gpio_num = -1,
        .spi_mode = 0,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 1,
        .lcd_cmd_bits = 32,
        .lcd_param_bits = 8,
        .flags = {
            .quad_mode = true,
        },
    };
    esp_lcd_panel_io_handle_t panel_io = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_HOST, &io_config, &panel_io));

    // 3. CO5300 Panel 初始化
    co5300_vendor_config_t vendor_config = {
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = lcd_init_cmds_size,
        .flags = {
            .use_qspi_interface = 1,
        },
    };
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_endian = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
        .vendor_config = &vendor_config,
    };

    esp_lcd_panel_handle_t panel_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_co5300(panel_io, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_set_gap(panel_handle, EXAMPLE_LCD_X_GAP, EXAMPLE_LCD_Y_GAP));
    // 在打开屏幕或背光之前，用户可以将预定义的图案刷新到屏幕上
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_co5300_set_brightness(panel_handle, 80));

    ESP_LOGI(TAG, "CO5300 QSPI 显示初始化完成 (%dx%d)", EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);

    // 4. CST820 触摸初始化（I2C）
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = EXAMPLE_TOUCH_I2C_NUM,
        .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
        .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t i2c_bus;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus));

    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST820_CONFIG();
    tp_io_config.scl_speed_hz = EXAMPLE_TOUCH_I2C_CLK_HZ;

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &tp_io_config, &tp_io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = EXAMPLE_TOUCH_RST,
        .int_gpio_num = EXAMPLE_TOUCH_INT,
        .levels = {.reset = 0, .interrupt = 0},
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_touch_handle_t touch_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst820(tp_io_handle, &tp_cfg, &touch_handle));

    ESP_LOGI(TAG, "CST820 触控初始化完成");

    // 5. esp_lvgl_adapter 初始化
    esp_lv_adapter_config_t adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG();
    adapter_cfg.stack_in_psram = true; // 尝试将任务栈放入PSRAM
    ESP_ERROR_CHECK(esp_lv_adapter_init(&adapter_cfg));

    // 注册显示器（QSPI 使用 SPI 宏）
    esp_lv_adapter_display_config_t disp_cfg =
        ESP_LV_ADAPTER_DISPLAY_SPI_WITH_PSRAM_TE_DEFAULT_CONFIG(
            panel_handle,
            panel_io,
            EXAMPLE_LCD_H_RES,
            EXAMPLE_LCD_V_RES,
            ESP_LV_ADAPTER_ROTATE_0,
            EXAMPLE_PIN_NUM_LCD_TE,
            EXAMPLE_LCD_PIXEL_CLOCK_HZ,
            4,
            16);

    lv_display_t *disp = esp_lv_adapter_register_display(&disp_cfg);
    assert(disp != NULL);

    // 设置背景颜色为黑色
    lv_obj_t *default_scr = lv_disp_get_scr_act(disp);     // 获取当前活动屏幕
    if (default_scr) {
        lv_obj_remove_style_all(default_scr);               // 清除默认样式
        lv_obj_set_style_bg_color(default_scr, lv_color_black(), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(default_scr, LV_OPA_COVER, LV_STATE_DEFAULT);
        lv_obj_set_size(default_scr, LV_PCT(100), LV_PCT(100));
    }

    // 注册触摸输入设备
    esp_lv_adapter_touch_config_t touch_cfg = ESP_LV_ADAPTER_TOUCH_DEFAULT_CONFIG(disp, touch_handle);
    lv_indev_t *indev = esp_lv_adapter_register_touch(&touch_cfg);
    assert(indev != NULL);

    ESP_ERROR_CHECK(esp_lv_adapter_start());
    ESP_LOGI(TAG, "esp_lvgl_adapter 启动成功！");

    // 6. 创建 LVGL UI（必须在锁内操作）
    if (esp_lv_adapter_lock(portMAX_DELAY) == ESP_OK) {

        // 启动 SJPG 播放器
        start_sjpg_player(disp);

        esp_lv_adapter_unlock();
    }

    ESP_LOGI(TAG, "LVGL UI 创建完成，系统正常运行...");
}
