#pragma once
// ensure the following is configured for your setup
#if defined(PARALLEL8)
#define PIN_NUM_BKL  -1
#define PIN_NUM_CS   33  // Chip select control pin (library pulls permanently low
#define PIN_NUM_DC   22  // (RS) Data Command control pin - must use a pin in the range 0-31
#define PIN_NUM_RST  32  // Reset pin, toggles on startup
#define PIN_NUM_WR    21  // Write strobe control pin - must use a pin in the range 0-31
#define PIN_NUM_RD    15  // Read strobe control pin
#define PIN_NUM_D0   2  // Must use pins in the range 0-31 for the data bus
#define PIN_NUM_D1   13  // so a single register write sets/clears all bits.
#define PIN_NUM_D2   26  // Pins can be randomly assigned, this does not affect
#define PIN_NUM_D3   25  // TFT screen update performance.
#define PIN_NUM_D4   27
#define PIN_NUM_D5   12
#define PIN_NUM_D6   14
#define PIN_NUM_D7   4
#define LCD_WRITE_SPEED_PERCENT 100 // 40MHz
#define LCD_READ_SPEED_PERCENT 100 // 20MHz

#elif defined(ESP_WROVER_KIT)
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
#define LCD_ROTATION 0
#define LCD_WRITE_SPEED_PERCENT 400 // 40MHz
#define LCD_READ_SPEED_PERCENT 200 // 20MHz
#elif defined(I2C)
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
#if defined(ILI9341) || defined(RA8875)
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
#elif defined(WS5IN65F) || defined(WS4IN2GSC)
#define LCD_READ_SPEED_PERCENT 100 // 10MHz
#else
#define LCD_READ_SPEED_PERCENT 200 // 20MHz
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
#ifndef PIN_NUM_BKL
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
#define LCD_ROTATION 3
#endif
#define LCD_COLOR
#elif defined(RA8875)
#include <ra8875.hpp>
#define PIN_NUM_BKL 15
#define LCD_WIDTH 480
#define LCD_HEIGHT 800
#define LCD_ROTATION 1
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
#elif defined(WS4IN2GSC)
#include <waveshare4in2gsc.hpp>
#define PIN_NUM_WAIT 15
#define LCD_WIDTH 400
#define LCD_HEIGHT 300
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
#ifdef PARALLEL8
using bus_type = tft_p8<PIN_NUM_CS,
                        PIN_NUM_WR,
                        PIN_NUM_RD,
                        PIN_NUM_D0,
                        PIN_NUM_D1,
                        PIN_NUM_D2,
                        PIN_NUM_D3,
                        PIN_NUM_D4,
                        PIN_NUM_D5,
                        PIN_NUM_D6,
                        PIN_NUM_D7>;
#elif I2C
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
// some devices have no SDA read capability, so no read whatsoever.
#if defined(LCD_EPAPER) || defined(SSD1306) || defined(SSD1351) 
                            false
#else
                            PIN_NUM_MISO<0
#endif
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
#if defined(LILYGO_TTGO)
                        true,
#else
                        false,
#endif
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
#elif defined(RA8875)
using lcd_type = ra8875<LCD_WIDTH,
                        LCD_HEIGHT,
                        bus_type,
                        PIN_NUM_RST,
                        -1,
                        LCD_ROTATION,
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
#elif defined(ST7735)
using lcd_type = st7735<PIN_NUM_DC,
                        PIN_NUM_RST,
                        PIN_NUM_BKL,
                        bus_type,
                        st7735_flags::green,
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
#elif defined(WS4IN2GSC)
using lcd_type = waveshare4in2gsc<PIN_NUM_DC,
                                PIN_NUM_RST,
                                PIN_NUM_WAIT,
                                bus_type,
                                LCD_WRITE_SPEED_PERCENT>;
#elif defined(WS5IN65F)
using lcd_type = waveshare5in65f<PIN_NUM_DC,
                                PIN_NUM_RST,
                                PIN_NUM_WAIT,
                                bus_type,
                                16,
                                LCD_WRITE_SPEED_PERCENT>;
#endif