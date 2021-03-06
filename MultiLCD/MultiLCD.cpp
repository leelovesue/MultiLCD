/*************************************************************************
* Arduino Text Display Library for Multiple LCDs
* Distributed under GPL v2.0
* Copyright (c) 2013 Stanley Huang <stanleyhuangyc@live.com>
* All rights reserved.
*************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "MultiLCD.h"

const PROGMEM unsigned char digits16x16[][32] = {
{0x00,0xE0,0xF8,0xFC,0xFE,0x1E,0x07,0x07,0x07,0x07,0x1E,0xFE,0xFC,0xF8,0xF0,0x00,0x00,0x07,0x0F,0x3F,0x3F,0x7C,0x70,0x70,0x70,0x70,0x7C,0x3F,0x1F,0x1F,0x07,0x00},/*0*/
{0x00,0x00,0x00,0x06,0x07,0x07,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},/*1*/
{0x00,0x38,0x3C,0x3E,0x3E,0x0F,0x07,0x07,0x07,0xCF,0xFF,0xFE,0xFE,0x38,0x00,0x00,0x00,0x40,0x40,0x60,0x70,0x78,0x7C,0x7E,0x7F,0x77,0x73,0x71,0x70,0x70,0x00,0x00},/*2*/
{0x00,0x18,0x1C,0x1E,0x1E,0x0F,0xC7,0xC7,0xE7,0xFF,0xFE,0xBE,0x9C,0x00,0x00,0x00,0x00,0x0C,0x1C,0x3C,0x3C,0x78,0x70,0x70,0x70,0x79,0x7F,0x3F,0x1F,0x0F,0x00,0x00},/*3*/
{0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x1E,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x06,0x07,0x07,0x07,0x06,0x06,0x06,0x06,0x06,0x7F,0x7F,0x7F,0x7F,0x06,0x06,0x00},/*4*/
{0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0xFF,0xE7,0xE7,0xE7,0xE7,0xC7,0x87,0x00,0x00,0x00,0x00,0x38,0x78,0x71,0x70,0x70,0x70,0x70,0x70,0x39,0x3F,0x3F,0x1F,0x0F,0x00},/*5*/
{0x00,0x80,0xE0,0xF0,0xF8,0xFC,0x7F,0x7F,0x6F,0x67,0xE1,0xE1,0xC0,0x80,0x00,0x00,0x00,0x0F,0x1F,0x3F,0x3F,0x78,0x70,0x70,0x70,0x70,0x78,0x3F,0x3F,0x1F,0x0F,0x00},/*6*/
{0x00,0x07,0x07,0x07,0x07,0x07,0xC7,0xE7,0xF7,0xFF,0x7F,0x3F,0x1F,0x07,0x03,0x01,0x00,0x20,0x38,0x7C,0x7E,0x3F,0x0F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*7*/
{0x00,0x00,0x00,0x1C,0xBE,0xFE,0xFF,0xE7,0xC3,0xC3,0xE7,0xFF,0xFE,0xBE,0x1C,0x00,0x00,0x00,0x0E,0x3F,0x3F,0x7F,0x71,0x60,0x60,0x60,0x71,0x7F,0x3F,0x3F,0x0F,0x00},/*8*/
{0x00,0x78,0xFC,0xFE,0xFE,0x8F,0x07,0x07,0x07,0x07,0x8F,0xFE,0xFE,0xFC,0xF8,0x00,0x00,0x00,0x00,0x01,0x43,0x43,0x73,0x7B,0x7F,0x7F,0x1F,0x0F,0x07,0x03,0x00,0x00},/*9*/
};

const PROGMEM unsigned char digits8x8[][8] = {
{0x3C,0x7E,0x83,0x81,0x81,0x7E,0x3C,0x00},/*"0",0*/
{0x84,0x84,0x82,0xFF,0xFF,0x80,0x80,0x00},/*"1",1*/
{0x84,0xC6,0xE1,0xA1,0xB1,0x9F,0x8E,0x00},/*"2",2*/
{0x42,0xC3,0x81,0x89,0x89,0xFF,0x76,0x00},/*"3",3*/
{0x20,0x38,0x24,0x22,0xFF,0xFF,0x20,0x00},/*"4",4*/
{0x5F,0xDF,0x99,0x89,0x89,0xF9,0x70,0x00},/*"5",5*/
{0x3C,0x7E,0x89,0x89,0x89,0xFB,0x72,0x00},/*"6",6*/
{0x01,0x01,0xE1,0xF9,0x1D,0x07,0x01,0x00},/*"7",7*/
{0x6E,0xFF,0x89,0x89,0x99,0xFF,0x76,0x00},/*"8",8*/
{0x4E,0xDF,0x91,0x91,0x91,0x7F,0x3E,0x00},/*"9",9*/
};

// The 7-bit ASCII character set...
const PROGMEM unsigned char font5x8[][5] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 20 space
  { 0x00, 0x00, 0x5f, 0x00, 0x00 },  // 21 !
  { 0x00, 0x07, 0x00, 0x07, 0x00 },  // 22 "
  { 0x14, 0x7f, 0x14, 0x7f, 0x14 },  // 23 #
  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },  // 24 $
  { 0x23, 0x13, 0x08, 0x64, 0x62 },  // 25 %
  { 0x36, 0x49, 0x55, 0x22, 0x50 },  // 26 &
  { 0x00, 0x05, 0x03, 0x00, 0x00 },  // 27 '
  { 0x00, 0x1c, 0x22, 0x41, 0x00 },  // 28 (
  { 0x00, 0x41, 0x22, 0x1c, 0x00 },  // 29 )
  { 0x14, 0x08, 0x3e, 0x08, 0x14 },  // 2a *
  { 0x08, 0x08, 0x3e, 0x08, 0x08 },  // 2b +
  { 0x00, 0x50, 0x30, 0x00, 0x00 },  // 2c ,
  { 0x08, 0x08, 0x08, 0x08, 0x08 },  // 2d -
  { 0x00, 0x60, 0x60, 0x00, 0x00 },  // 2e .
  { 0x20, 0x10, 0x08, 0x04, 0x02 },  // 2f /
  { 0x3e, 0x51, 0x49, 0x45, 0x3e },  // 30 0
  { 0x00, 0x42, 0x7f, 0x40, 0x00 },  // 31 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 },  // 32 2
  { 0x21, 0x41, 0x45, 0x4b, 0x31 },  // 33 3
  { 0x18, 0x14, 0x12, 0x7f, 0x10 },  // 34 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 },  // 35 5
  { 0x3c, 0x4a, 0x49, 0x49, 0x30 },  // 36 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 },  // 37 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 },  // 38 8
  { 0x06, 0x49, 0x49, 0x29, 0x1e },  // 39 9
  { 0x00, 0x36, 0x36, 0x00, 0x00 },  // 3a :
  { 0x00, 0x56, 0x36, 0x00, 0x00 },  // 3b ;
  { 0x08, 0x14, 0x22, 0x41, 0x00 },  // 3c <
  { 0x14, 0x14, 0x14, 0x14, 0x14 },  // 3d =
  { 0x00, 0x41, 0x22, 0x14, 0x08 },  // 3e >
  { 0x02, 0x01, 0x51, 0x09, 0x06 },  // 3f ?
  { 0x32, 0x49, 0x79, 0x41, 0x3e },  // 40 @
  { 0x7e, 0x11, 0x11, 0x11, 0x7e },  // 41 A
  { 0x7f, 0x49, 0x49, 0x49, 0x36 },  // 42 B
  { 0x3e, 0x41, 0x41, 0x41, 0x22 },  // 43 C
  { 0x7f, 0x41, 0x41, 0x22, 0x1c },  // 44 D
  { 0x7f, 0x49, 0x49, 0x49, 0x41 },  // 45 E
  { 0x7f, 0x09, 0x09, 0x09, 0x01 },  // 46 F
  { 0x3e, 0x41, 0x49, 0x49, 0x7a },  // 47 G
  { 0x7f, 0x08, 0x08, 0x08, 0x7f },  // 48 H
  { 0x00, 0x41, 0x7f, 0x41, 0x00 },  // 49 I
  { 0x20, 0x40, 0x41, 0x3f, 0x01 },  // 4a J
  { 0x7f, 0x08, 0x14, 0x22, 0x41 },  // 4b K
  { 0x7f, 0x40, 0x40, 0x40, 0x40 },  // 4c L
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f },  // 4d M
  { 0x7f, 0x04, 0x08, 0x10, 0x7f },  // 4e N
  { 0x3e, 0x41, 0x41, 0x41, 0x3e },  // 4f O
  { 0x7f, 0x09, 0x09, 0x09, 0x06 },  // 50 P
  { 0x3e, 0x41, 0x51, 0x21, 0x5e },  // 51 Q
  { 0x7f, 0x09, 0x19, 0x29, 0x46 },  // 52 R
  { 0x46, 0x49, 0x49, 0x49, 0x31 },  // 53 S
  { 0x01, 0x01, 0x7f, 0x01, 0x01 },  // 54 T
  { 0x3f, 0x40, 0x40, 0x40, 0x3f },  // 55 U
  { 0x1f, 0x20, 0x40, 0x20, 0x1f },  // 56 V
  { 0x3f, 0x40, 0x38, 0x40, 0x3f },  // 57 W
  { 0x63, 0x14, 0x08, 0x14, 0x63 },  // 58 X
  { 0x07, 0x08, 0x70, 0x08, 0x07 },  // 59 Y
  { 0x61, 0x51, 0x49, 0x45, 0x43 },  // 5a Z
  { 0x00, 0x7f, 0x41, 0x41, 0x00 },  // 5b [
  { 0x02, 0x04, 0x08, 0x10, 0x20 },  // 5c backslash
  { 0x00, 0x41, 0x41, 0x7f, 0x00 },  // 5d ]
  { 0x04, 0x02, 0x01, 0x02, 0x04 },  // 5e ^
  { 0x40, 0x40, 0x40, 0x40, 0x40 },  // 5f _
  { 0x00, 0x01, 0x02, 0x04, 0x00 },  // 60 `
  { 0x20, 0x54, 0x54, 0x54, 0x78 },  // 61 a
  { 0x7f, 0x48, 0x44, 0x44, 0x38 },  // 62 b
  { 0x38, 0x44, 0x44, 0x44, 0x20 },  // 63 c
  { 0x38, 0x44, 0x44, 0x48, 0x7f },  // 64 d
  { 0x38, 0x54, 0x54, 0x54, 0x18 },  // 65 e
  { 0x08, 0x7e, 0x09, 0x01, 0x02 },  // 66 f
  { 0x0c, 0x52, 0x52, 0x52, 0x3e },  // 67 g
  { 0x7f, 0x08, 0x04, 0x04, 0x78 },  // 68 h
  { 0x00, 0x44, 0x7d, 0x40, 0x00 },  // 69 i
  { 0x20, 0x40, 0x44, 0x3d, 0x00 },  // 6a j
  { 0x7f, 0x10, 0x28, 0x44, 0x00 },  // 6b k
  { 0x00, 0x41, 0x7f, 0x40, 0x00 },  // 6c l
  { 0x7c, 0x04, 0x18, 0x04, 0x78 },  // 6d m
  { 0x7c, 0x08, 0x04, 0x04, 0x78 },  // 6e n
  { 0x38, 0x44, 0x44, 0x44, 0x38 },  // 6f o
  { 0x7c, 0x14, 0x14, 0x14, 0x08 },  // 70 p
  { 0x08, 0x14, 0x14, 0x18, 0x7c },  // 71 q
  { 0x7c, 0x08, 0x04, 0x04, 0x08 },  // 72 r
  { 0x48, 0x54, 0x54, 0x54, 0x20 },  // 73 s
  { 0x04, 0x3f, 0x44, 0x40, 0x20 },  // 74 t
  { 0x3c, 0x40, 0x40, 0x20, 0x7c },  // 75 u
  { 0x1c, 0x20, 0x40, 0x20, 0x1c },  // 76 v
  { 0x3c, 0x40, 0x30, 0x40, 0x3c },  // 77 w
  { 0x44, 0x28, 0x10, 0x28, 0x44 },  // 78 x
  { 0x0c, 0x50, 0x50, 0x50, 0x3c },  // 79 y
  { 0x44, 0x64, 0x54, 0x4c, 0x44 },  // 7a z
  { 0x00, 0x08, 0x36, 0x41, 0x00 },  // 7b {
  { 0x00, 0x00, 0x7f, 0x00, 0x00 },  // 7c |
  { 0x00, 0x41, 0x36, 0x08, 0x00 },  // 7d }
  { 0x10, 0x08, 0x08, 0x10, 0x08 },  // 7e ~
  { 0x00, 0x00, 0x00, 0x00, 0x00 }   // 7f
};

void LCD_Common::printInt(unsigned int value, FONT_SIZE size, byte padding)
{
    bool start = false;
    for (unsigned int den = 10000; den; den /= 10) {
        byte v = (byte)(value / den);
        value -= v * den;
        if (v == 0 && !start) {
            if (padding) {
                writeDigit(-1, size);
                padding--;
            }
            continue;
        }
        start = true;
        writeDigit(v, size);
    }
}

void LCD_Common::printLong(unsigned long value, FONT_SIZE size, byte padding)
{
    bool start = false;
    for (unsigned long den = 1000000000; den; den /= 10) {
        byte v = (byte)(value / den);
        value -= v * den;
        if (v == 0 && !start) {
            if (padding) {
                writeDigit(-1, size);
                padding--;
            }
            continue;
        }
        start = true;
        writeDigit(v, size);
    }
}

void LCD_ZTOLED::setCursor(byte column, byte line)
{
    m_column = column << 3;
    m_page = line << 1;
    delay(1);
    ScI2cMxSetLocation(OLED_ADDRESS, m_page, m_column);
}

size_t LCD_ZTOLED::write(uint8_t c)
{
    char s[2] = {c};
    ScI2cMxDisplay8x16Str(OLED_ADDRESS, m_page, m_column, s);
    m_column += 8;
    if (m_column >= 128) {
        m_column = 0;
        m_page += 2;
    }
}

void LCD_ZTOLED::print(const char* s)
{
    ScI2cMxDisplay8x16Str(OLED_ADDRESS, m_page, m_column, s);
    m_column += (strlen(s) << 3);
    m_page += (m_column >> 7) << 1;
    m_column %= 0x7f;
}

void LCD_ZTOLED::writeDigit(byte n, FONT_SIZE size)
{
    if (size == FONT_SIZE_SMALL) {
        unsigned char data[8];
        if (n >= 0 && n <= 9) {
            memcpy_P(data, digits8x8[n], 8);
        } else {
            memset(data, 0, sizeof(data));
        }
        ScI2cMxDisplayDot8x8(OLED_ADDRESS, data);
        m_column += 8;
    } else if (size == FONT_SIZE_MEDIUM) {
        write(n >= 0 && n <= 9 ? '0' + n : ' ');
    } else {
        unsigned char data[32];
        if (n >= 0 && n <= 9) {
            memcpy_P(data, digits16x16[n], 32);
        } else {
            memset(data, 0, sizeof(data));
        }
        ScI2cMxDisplayDot16x16(OLED_ADDRESS, m_page, m_column, data);
        m_column += 16;
    }
}

void LCD_ZTOLED::clear()
{
    ScI2cMxFillArea(OLED_ADDRESS, 0, 7, 0, 127, 0);
    delay(10);
    setCursor(0, 0);
}

void LCD_ZTOLED::begin()
{
    I2cInit();
    ScI2cMxReset(OLED_ADDRESS);
    delay(10);
}

void LCD_PCD8544::writeDigit(byte n, FONT_SIZE size)
{
    if (size == FONT_SIZE_SMALL) {
        write(n >= 0 && n <= 9 ? '0' + n : ' ');
    } else if (size == FONT_SIZE_MEDIUM) {
        unsigned char data[8];
        if (n >= 0 && n <= 9) {
            memcpy_P(data, digits8x8[n], 8);
        } else {
            memset(data, 0, sizeof(data));
        }
        draw8x8(data);
    } else {
        unsigned char data[32];
        if (n >= 0 && n <= 9) {
            memcpy_P(data, digits16x16[n], 32);
        } else {
            memset(data, 0, sizeof(data));
        }
        draw16x16(data);
        //column += 16;
    }
}

void LCD_SSD1306::setCursor(byte column, byte line)
{
    m_col = column;
    m_row = line;
    ssd1306_command(0xB0 + m_row);//set page address
    ssd1306_command(m_col & 0xf);//set lower column address
    ssd1306_command(0x10 | (m_col >> 4));//set higher column address
}

size_t LCD_SSD1306::write(uint8_t c)
{
    byte pgm_buffer[6];
    memcpy_P(pgm_buffer, &font5x8[(c < ' ' || c > 0x7f) ? 0 : c - ' '], sizeof(pgm_buffer));
    pgm_buffer[5] = 0;

    Wire.beginTransmission(_i2caddr);
    Wire.write(0x40);
    Wire.write(pgm_buffer, 6);
    Wire.endTransmission();
    return 1;
}

void LCD_SSD1306::writeDigit(byte n, FONT_SIZE size)
{
    uint8_t twbrbackup = TWBR;
    TWBR = 18; // upgrade to 400KHz!
    if (size == FONT_SIZE_SMALL) {
        byte pgm_buffer[6] = {0};
        if (n >= 0 && n <= 9) {
            memcpy_P(pgm_buffer, &font5x8[n + ('0' - ' ')], 5);
        }
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        Wire.write(pgm_buffer, 6);
        Wire.endTransmission();
        m_col += 6;
    } else if (size == FONT_SIZE_MEDIUM) {
        byte pgm_buffer[8];
        if (n >= 0 && n <= 9) {
            memcpy_P(pgm_buffer, &digits8x8[n], 8);
        } else {
            memset(pgm_buffer, 0, 8);
        }
        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        Wire.write(pgm_buffer, 8);
        Wire.endTransmission();
        m_col += 8;
    } else {
        byte pgm_buffer[32];
        if (n >= 0 && n <= 9) {
            memcpy_P(pgm_buffer, &digits16x16[n], 32);
        } else {
            memset(pgm_buffer, 0, 16);
        }

        ssd1306_command(0xB0 + m_row);//set page address
        ssd1306_command(m_col & 0xf);//set lower column address
        ssd1306_command(0x10 | (m_col >> 4));//set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        Wire.write(pgm_buffer, 16);
        Wire.endTransmission();

        ssd1306_command(0xB0 + m_row + 1);//set page address
        ssd1306_command(m_col & 0xf);//set lower column address
        ssd1306_command(0x10 | (m_col >> 4));//set higher column address

        Wire.beginTransmission(_i2caddr);
        Wire.write(0x40);
        Wire.write(pgm_buffer + 16, 16);
        Wire.endTransmission();

        m_col += 16;
    }
    TWBR = twbrbackup;
}
