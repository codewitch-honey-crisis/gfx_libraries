#pragma once
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
#define LCD_BKL_HIGH false
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_ROTATION 1

#include <Arduino.h>
#include <stdio.h>
#include <ili9341.hpp>

#include <tft_io.hpp>

// generated with fontgen
#include "telegrama.hpp"

using namespace arduino;
using bus_type = tft_spi_ex<LCD_HOST,
                            PIN_NUM_CS,
                            PIN_NUM_MOSI,
                            PIN_NUM_MISO,
                            PIN_NUM_CLK,
                            SPI_MODE0,
                            PIN_NUM_MISO<0
#ifdef OPTIMIZE_DMA
                            ,(LCD_WIDTH*LCD_HEIGHT)*2+8
#endif
                            >;
using lcd_type = ili9341<PIN_NUM_DC,
                        PIN_NUM_RST,
                        PIN_NUM_BKL,
                        bus_type,
                        LCD_ROTATION,
                        LCD_BKL_HIGH,
                        LCD_WRITE_SPEED_PERCENT,
                        LCD_READ_SPEED_PERCENT>;
