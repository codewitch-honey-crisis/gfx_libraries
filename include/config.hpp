#pragma once
// ensure the following is configured for your setup
#if defined(ESP_WROVER_KIT)
#define LCD_HOST    VSPI
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22
#define LCD_WRITE_SPEED_PERCENT 400 // 40MHz
#define LCD_READ_SPEED_PERCENT 200 // 20MHz
#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BKL 5
#elif defined(LILYGO_TTGO)
#define LCD_WIDTH 135
#define LCD_HEIGHT 240
#define LCD_HOST    VSPI
#define PIN_NUM_MISO -1
#define PIN_NUM_MOSI 19
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5
#define PIN_NUM_DC   16
#define PIN_NUM_RST  23
#define PIN_NUM_BKL 4
#define LCD_ROTATION 1
#define LCD_WRITE_SPEED_PERCENT 400 // 40MHz
#define LCD_READ_SPEED_PERCENT 200 // 20MHz
#else
#ifdef I2C
#define LCD_PORT 0
#define PIN_NUM_SDA 21
#define PIN_NUM_SCL 22
#define PIN_NUM_RST -1
#define PIN_NUM_DC -1
#define LCD_WRITE_SPEED_PERCENT 400 // 400KHz - can sometimes be 800
#else
#define LCD_HOST VSPI
#define PIN_NUM_CS 5
#define PIN_NUM_MOSI 23
#if defined(ILI9341)
#define PIN_NUM_MISO 19
#else
#define PIN_NUM_MISO -1
#endif
#define PIN_NUM_CLK 18
#define PIN_NUM_DC 2
#define PIN_NUM_RST 4
#if defined(ILI9341) || defined(ST7789)
#define LCD_WRITE_SPEED_PERCENT 400 // 40MHz
#else
#define LCD_WRITE_SPEED_PERCENT 200 // 20MHz
#endif
#if defined(ST7735)
// SDA reads are slow
#define LCD_READ_SPEED_PERCENT 40 // 4MHz
#elif defined(WS5IN65F)
#define LCD_READ_SPEED_PERCENT 100 // 10MHz
#else
#define LCD_READ_SPEED_PERCENT 200 // 20MHz
#endif
#endif
#endif

#include <Arduino.h>
#include <stdio.h>
#include <tft_io.hpp>
#if defined(ILI9341)
#include <ili9341.hpp>
#if defined(ESP_WROVER_KIT)
#define LCD_BKL_HIGH false
#else
#define LCD_BKL_HIGH true
#endif
#ifndef ESP_WROVER_KIT
#define PIN_NUM_BKL 15
#endif
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_ROTATION 1
#define LCD_COLOR
#elif defined(ST7789)
#include <st7789.hpp>
#ifndef LILYGO_TTGO
#define PIN_NUM_BKL 15
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_ROTATION 1
#endif
#define LCD_COLOR
#elif defined(ST7735)
#include <st7735.hpp>
#define PIN_NUM_BKL 15
#define LCD_WIDTH 128
#define LCD_HEIGHT 128
#define LCD_ROTATION 1
#define LCD_COLOR
#elif defined(SSD1306)
#include <ssd1306.hpp>
#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_ADDRESS 0x3C
#define LCD_VDC_3_3 true
#elif defined(SSD1351)
#include <ssd1351.hpp>
#define LCD_WIDTH 128
#define LCD_HEIGHT 128
#define LCD_COLOR
#elif defined(WS5IN65F)
#include <waveshare5in65f.hpp>
#define PIN_NUM_WAIT 15
#define LCD_WIDTH 600
#define LCD_HEIGHT 448
#define LCD_COLOR
#define LCD_EPAPER
#endif

#ifdef LCD_COLOR
// generated with bingen
#include "image.h"
#else
// generated with bingen
#include "image3.h"
#define image_jpg_stream image3_jpg_stream
#endif

// generated with fontgen
#include "Maziro.h"
// generated with fontgen
#include "Bm437_ATI_9x16.h"

using namespace arduino;

#ifdef I2C
using bus_type = tft_i2c_ex<LCD_PORT,
                            PIN_NUM_SDA,
                            PIN_NUM_SCL>;
#else
using bus_type = tft_spi_ex<LCD_HOST,
                            PIN_NUM_CS,
                            PIN_NUM_MOSI,
                            PIN_NUM_MISO,
                            PIN_NUM_CLK,
                            SPI_MODE0,
                            false
#ifdef OPTIMIZE_DMA
                            ,(LCD_WIDTH*LCD_HEIGHT)/8+8
#endif
                            >;
#endif

#if defined(ILI9341)
using lcd_type = ili9341<PIN_NUM_DC,
                        PIN_NUM_RST,
                        PIN_NUM_BKL,
                        bus_type,
                        LCD_ROTATION,
                        LCD_BKL_HIGH,
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
#elif defined(ST7789)
using lcd_type = st7789<LCD_WIDTH,
                        LCD_HEIGHT,
                        PIN_NUM_DC,
                        PIN_NUM_RST,
                        PIN_NUM_BKL,
                        bus_type,
                        LCD_ROTATION,
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
#elif defined(ST7735)
using lcd_type = st7735<PIN_NUM_DC,
                        PIN_NUM_RST,
                        PIN_NUM_BKL,
                        bus_type,
                        arduino::st7735_flags::green,
                        LCD_ROTATION,
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
#elif defined(SSD1306)
using lcd_type = ssd1306<LCD_WIDTH,
                        LCD_HEIGHT,
                        bus_type,
                        4,
                        LCD_ADDRESS,
                        LCD_VDC_3_3,
                        LCD_WRITE_SPEED_PERCENT,
                        PIN_NUM_DC,
                        PIN_NUM_RST,
                        true>;
#elif defined(SSD1351)
using lcd_type = ssd1351<PIN_NUM_DC,
                        PIN_NUM_RST,
                        bus_type,
                        LCD_WRITE_SPEED_PERCENT>;
#elif defined(WS5IN65F)
using lcd_type = waveshare5in65f<PIN_NUM_DC,
                                PIN_NUM_RST,
                                PIN_NUM_WAIT,
                                bus_type,
                                LCD_WRITE_SPEED_PERCENT>;
#endif