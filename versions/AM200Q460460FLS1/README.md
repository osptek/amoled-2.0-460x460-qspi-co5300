<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 2.0″ AMOLED 460×460（CO5300 · QSPI）</h1>

<p align="center"><b>AMOLED 模组 · QSPI · CO5300</b></p>

<p align="center"><a href="./README_EN.md">English</a> | 简体中文 · <a href="../../README.md">规格族索引</a></p>

<p align="center">
  <img alt="Size: 2.0 inch" src="https://img.shields.io/badge/Size-2.0%22-3498DB?style=flat-square" />
  <img alt="Resolution: 460x460" src="https://img.shields.io/badge/Resolution-460%C3%97460-8E44AD?style=flat-square" />
  <img alt="Interface: QSPI" src="https://img.shields.io/badge/Interface-QSPI-27AE60?style=flat-square" />
  <img alt="Driver: CO5300" src="https://img.shields.io/badge/Driver-CO5300-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 2.0 寸 460×460 AMOLED QSPI 模组（CO5300）宣传图" src="./images/product.png" width="640" /></p>

## 目录

- [产品简介](#产品简介)
- [规格参数](#规格参数)
- [示例工程](#示例工程)
- [仓库结构](#仓库结构)
- [相关资料](#相关资料)
- [购买链接](#购买链接)
- [技术支持](#技术支持)

---

## 产品简介

OSPTEK **2.0 寸 460×460 AMOLED** 是一款 **QSPI** 接口显示模组，驱动为 **CO5300**。适合手持终端、穿戴与小型 HMI 等场景。

规格标识（仓库名）：`2.0-amoled-460x460-qspi-co5300`

当前模组版本：**AM200Q460460FLS1**。电气与外形细节以 [`docs/AM200Q460460FLS1.pdf`](./docs/AM200Q460460FLS1.pdf) 为准。

## 规格参数

| 项目 | 规格 |
| ---- | ---- |
| 尺寸 | 2.0 英寸 |
| 类型 | AMOLED（16.7M LTPS） |
| 分辨率 | 460×460 |
| 接口 | QSPI |
| 驱动 IC | CO5300（CO5300AF-16） |
| 触摸驱动 | 电容触摸（I2C；规格书未标明具体 IC） |

> 完整外形尺寸、FPC 定义、供电与时序以产品规格书 / 驱动手册为准。

## 示例工程

| 说明 | 路径 |
| ---- | ---- |
| ESP32-S3 · CO5300 QSPI + LVGL8 | [`examples/esp32s3-idf5_co5300-qspi_lvgl8/`](./examples/esp32s3-idf5_co5300-qspi_lvgl8/) |
| ESP32-S3 · CO5300 QSPI + esp-lvgl-adapter / LVGL8 | [`examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8/`](./examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8/) |
| ESP32-S3 · CO5300 QSPI + esp-lvgl-adapter / LVGL9 | [`examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9/`](./examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9/) |
| ESP32-S3 · LVGL8 + TE 防撕裂 | [`examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8_amoled-with-te/`](./examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8_amoled-with-te/) |
| ESP32-S3 · LVGL9 + TE 防撕裂 | [`examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9_amoled-with-te/`](./examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9_amoled-with-te/) |
| ESP32-S3 · EAF 开机动画 | [`examples/eaf/esp32s3-idf5_co5300-qspi_eaf-boot-animation_spiffs_lvgl9/`](./examples/eaf/esp32s3-idf5_co5300-qspi_eaf-boot-animation_spiffs_lvgl9/) |
| ESP32-S3 · EAF 播放 | [`examples/eaf/esp32s3-idf5_co5300-qspi_esp-lv-eaf-player_spiffs_lvgl9/`](./examples/eaf/esp32s3-idf5_co5300-qspi_esp-lv-eaf-player_spiffs_lvgl9/) |
| ESP32-S3 · FreeType 字体 | [`examples/freetype/esp32s3-idf5_co5300-qspi_esp-lv-freetype_spiffs_lvgl9/`](./examples/freetype/esp32s3-idf5_co5300-qspi_esp-lv-freetype_spiffs_lvgl9/) |
| ESP32-S3 · JPG 解码 | [`examples/jpg-decoder/esp32s3-idf5_co5300-qspi_esp-lv-decoder_spiffs_lvgl9/`](./examples/jpg-decoder/esp32s3-idf5_co5300-qspi_esp-lv-decoder_spiffs_lvgl9/) |
| ESP32-S3 · Lottie 播放 | [`examples/lottie/esp32s3-idf5_co5300-qspi_esp-lv-lottie-player_spiffs_lvgl9/`](./examples/lottie/esp32s3-idf5_co5300-qspi_esp-lv-lottie-player_spiffs_lvgl9/) |

## 仓库结构

```text
2.0-amoled-460x460-qspi-co5300/                                # 仓库根（导航见 ../../README.md）
└── versions/
    └── AM200Q460460FLS1/                                      # 本料号完整资料
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## 相关资料

### 本产品资料

| 资料 | 链接 |
| ---- | ---- |
| 产品规格书 | [`docs/AM200Q460460FLS1.pdf`](./docs/AM200Q460460FLS1.pdf) |
| 2.0 寸 AMOLED 转接板（V2.0） | [`docs/PCB-2.0寸AMOLED屏转接板V2.0.pdf`](./docs/PCB-2.0%E5%AF%B8AMOLED%E5%B1%8F%E8%BD%AC%E6%8E%A5%E6%9D%BFV2.0.pdf) |
| 转接板连接器（OK-14F024-04） | [`docs/OK-14F024-04.pdf`](./docs/OK-14F024-04.pdf) |

### 示例工程

- [ESP32-S3 CO5300 QSPI + LVGL8](./examples/esp32s3-idf5_co5300-qspi_lvgl8/)
- [ESP32-S3 CO5300 QSPI + LVGL8（adapter）](./examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8/)
- [ESP32-S3 CO5300 QSPI + LVGL9（adapter）](./examples/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9/)
- [ESP32-S3 LVGL8 + TE](./examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl8_amoled-with-te/)
- [ESP32-S3 LVGL9 + TE](./examples/with-te/esp32s3-idf5_co5300-qspi_esp-lvgl-adapter_lvgl9_amoled-with-te/)
- [ESP32-S3 EAF 开机动画](./examples/eaf/esp32s3-idf5_co5300-qspi_eaf-boot-animation_spiffs_lvgl9/)
- [ESP32-S3 EAF 播放](./examples/eaf/esp32s3-idf5_co5300-qspi_esp-lv-eaf-player_spiffs_lvgl9/)
- [ESP32-S3 FreeType](./examples/freetype/esp32s3-idf5_co5300-qspi_esp-lv-freetype_spiffs_lvgl9/)
- [ESP32-S3 JPG 解码](./examples/jpg-decoder/esp32s3-idf5_co5300-qspi_esp-lv-decoder_spiffs_lvgl9/)
- [ESP32-S3 Lottie](./examples/lottie/esp32s3-idf5_co5300-qspi_esp-lv-lottie-player_spiffs_lvgl9/)

## 购买链接

<p align="center">
  <a href="https://shop110742373.taobao.com/"><img alt="淘宝官方店铺" src="https://img.shields.io/badge/淘宝-官方店铺-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="速卖通官方店铺" src="https://img.shields.io/badge/速卖通-官方店铺-E62E04?style=for-the-badge&logo=aliexpress&logoColor=white" /></a>
</p>

**国内（淘宝）**

- 店铺：[鱼鹰光电工厂店](https://shop110742373.taobao.com/)

**海外（AliExpress）**

- 店铺：[OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

## 技术支持

- 技术支持 / 产品咨询：<luyu@osptek.com>
- QQ 技术交流群：**985881096**
- 公司官网：<https://osptek.com/>
- 有任何问题，都可以在本仓库 Issues 中提问

---

<p align="center"><sub>© 2026 OSPTEK 鱼鹰光电 · 本仓库资料采用 CC BY 4.0 许可</sub></p>
