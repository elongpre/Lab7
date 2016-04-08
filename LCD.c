#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "LCD.h"
#include "../inc/tm4c123gh6pm.h"

/*-------------------------------------
Screen coordinates mimic a coordinate plane
		 ________
   (0,Y)|		 |(X,Y)
		|		 |
		|		 |
		|		 | 	
   (0,0)|________|(X,0)

-------------------------------------*/
void Write_Command(uint16_t Wcommand);
void Write_Data(uint16_t Wdata);
void Write_Command_Data(uint16_t Wcommand,uint16_t Wdata);
void Delay(uint32_t X);
void DisableInterrupts(void);
void EnableInterrupts(void);

#define LCD_RS (*((volatile uint32_t *)0x40004020))		//PA3
#define LCD_WR (*((volatile uint32_t *)0x40004040))		//PA4
#define LCD_RD (*((volatile uint32_t *)0x40004080))		//PA5
#define LCD_CS (*((volatile uint32_t *)0x40004100))		//PA6
#define LCD_RST (*((volatile uint32_t *)0x40004200))	//PA7
//Parallel data bits 0-8: 	PB0-7
//				bits 9-12:	PD0-3
//				bits 13-16:	PC4-7

const uint16_t FONT_8x8[784] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // !
0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00, // "
0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00, // #
0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00, // $
0x00,0x63,0x66,0x0C,0x18,0x33,0x63,0x00, // %
0x1C,0x36,0x1C,0x3B,0x6E,0x66,0x3B,0x00, // &
0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00, // '
0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00, // (
0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00, // )
0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00, // *
0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30, // ,
0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00, // .
0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00, // ' (forward slash)
0x3E,0x63,0x63,0x6B,0x63,0x63,0x3E,0x00, // 0 0x30
0x18,0x38,0x58,0x18,0x18,0x18,0x7E,0x00, // 1
0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00, // 2
0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00, // 3
0x0E,0x1E,0x36,0x66,0x7F,0x06,0x0F,0x00, // 4
0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00, // 5
0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00, // 6
0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00, // 7
0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00, // 8
0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00, // 9
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00, // :
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30, // ;
0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00, // <
0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00, // =
0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00, // >
0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00, // ?
0x3E,0x63,0x6F,0x69,0x6F,0x60,0x3E,0x00, // @ 0x40
0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00, // A
0x7E,0x33,0x33,0x3E,0x33,0x33,0x7E,0x00, // B
0x1E,0x33,0x60,0x60,0x60,0x33,0x1E,0x00, // C
0x7C,0x36,0x33,0x33,0x33,0x36,0x7C,0x00, // D
0x7F,0x31,0x34,0x3C,0x34,0x31,0x7F,0x00, // E
0x7F,0x31,0x34,0x3C,0x34,0x30,0x78,0x00, // F
0x1E,0x33,0x60,0x60,0x67,0x33,0x1F,0x00, // G
0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00, // H
0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // I
0x0F,0x06,0x06,0x06,0x66,0x66,0x3C,0x00, // J
0x73,0x33,0x36,0x3C,0x36,0x33,0x73,0x00, // K
0x78,0x30,0x30,0x30,0x31,0x33,0x7F,0x00, // L
0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00, // M
0x63,0x73,0x7B,0x6F,0x67,0x63,0x63,0x00, // N
0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00, // O
0x7E,0x33,0x33,0x3E,0x30,0x30,0x78,0x00, // P 0x50
0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00, // Q
0x7E,0x33,0x33,0x3E,0x36,0x33,0x73,0x00, // R
0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00, // S
0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00, // T
0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x00, // U
0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00, // V
0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00, // W
0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00, // X
0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00, // Y
0x7F,0x63,0x46,0x0C,0x19,0x33,0x7F,0x00, // Z
0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00, // [
0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00, // \ (back slash)
0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x3C,0x06,0x3E,0x66,0x3B,0x00, // a
0x70,0x30,0x3E,0x33,0x33,0x33,0x6E,0x00, // b
0x00,0x00,0x3C,0x66,0x60,0x66,0x3C,0x00, // c
0x0E,0x06,0x3E,0x66,0x66,0x66,0x3B,0x00, // d
0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00, // e
0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00, // f
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x7C, // g
0x70,0x30,0x36,0x3B,0x33,0x33,0x73,0x00, // h
0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00, // i
0x06,0x00,0x06,0x06,0x06,0x66,0x66,0x3C, // j
0x70,0x30,0x33,0x36,0x3C,0x36,0x73,0x00, // k
0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // l
0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00, // m
0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00, // n
0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00, // o
0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78, // p 0x70
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F, // q
0x00,0x00,0x6E,0x3B,0x33,0x30,0x78,0x00, // r
0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00, // s
0x08,0x18,0x3E,0x18,0x18,0x1A,0x0C,0x00, // t
0x00,0x00,0x66,0x66,0x66,0x66,0x3B,0x00, // u
0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00, // v
0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00, // w
0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00, // x
0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x7C, // y
0x00,0x00,0x7E,0x4C,0x18,0x32,0x7E,0x00, // z
0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00, // {
0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x0C,0x00, // |
0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00, // }
0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x1C,0x36,0x36,0x1C,0x00,0x00,0x00,0x00, // DEL
0x60,0x90,0x90,0x60,0x00,0x00,0x00,0x00,  // DEGREE use:�
0x22,0x77,0x7f,0x7f,0x3e,0x1c,0x08,0x00            //HEART use:�
};

const uint16_t FONT_16x16[3040]  = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //  <Space>
0x00,0x00,0x00,0x00,0x07,0x00,0x0F,0x80,0x0F,0x80,0x0F,0x80,0x0F,0x80,0x0F,0x80,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00, // !
0x00,0x00,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x06,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // "
0x00,0x00,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x7F,0xFE,0x7F,0xFE,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x7F,0xFE,0x7F,0xFE,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x00,0x00, // #
0x00,0x00,0x02,0x40,0x02,0x40,0x0F,0xF8,0x1F,0xF8,0x1A,0x40,0x1A,0x40,0x1F,0xF0,0x0F,0xF8,0x02,0x58,0x02,0x58,0x1F,0xF8,0x1F,0xF0,0x02,0x40,0x02,0x40,0x00,0x00, // $
0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x10,0x0E,0x30,0x0E,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x70,0x0C,0x70,0x08,0x70,0x00,0x00,0x00,0x00,0x00,0x00, // %
0x00,0x00,0x00,0x00,0x0F,0x00,0x19,0x80,0x19,0x80,0x19,0x80,0x0F,0x00,0x0F,0x08,0x0F,0x98,0x19,0xF8,0x18,0xF0,0x18,0xE0,0x19,0xF0,0x0F,0x98,0x00,0x00,0x00,0x00, // &
0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // '
0x00,0x00,0x00,0x00,0x00,0xF0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xF0,0x00,0x00,0x00,0x00, // (
0x00,0x00,0x00,0x00,0x0F,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x0F,0x00,0x00,0x00,0x00,0x00, // )
0x00,0x00,0x00,0x00,0x01,0x80,0x11,0x88,0x09,0x90,0x07,0xE0,0x07,0xE0,0x3F,0xFC,0x3F,0xFC,0x07,0xE0,0x07,0xE0,0x09,0x90,0x11,0x88,0x01,0x80,0x00,0x00,0x00,0x00, // *
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x01,0x80,0x0F,0xF0,0x0F,0xF0,0x01,0x80,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x0E,0x00,0x00,0x00, // ,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x1F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00, // ,
0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x06,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x00,0x00,0x00,0x00, // '/
0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x38,0x1C,0x78,0x1C,0xF8,0x1C,0xF8,0x1D,0xB8,0x1D,0xB8,0x1F,0x38,0x1F,0x38,0x1E,0x38,0x1C,0x38,0x0F,0xF0,0x00,0x00,0x00,0x00, // 0
0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x03,0x80,0x1F,0x80,0x1F,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x1F,0xF0,0x00,0x00,0x00,0x00, // 1
0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x70,0x1C,0x38,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x38,0x1C,0x38,0x1F,0xF8,0x00,0x00,0x00,0x00, // 2
0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x70,0x1C,0x38,0x00,0x38,0x00,0x70,0x03,0xC0,0x03,0xC0,0x00,0x70,0x00,0x38,0x1C,0x38,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // 3
0x00,0x00,0x00,0x00,0x00,0xE0,0x01,0xE0,0x03,0xE0,0x06,0xE0,0x0C,0xE0,0x18,0xE0,0x1F,0xF8,0x1F,0xF8,0x00,0xE0,0x00,0xE0,0x00,0xE0,0x03,0xF8,0x00,0x00,0x00,0x00, // 4
0x00,0x00,0x00,0x00,0x1F,0xF8,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1F,0xE0,0x1F,0xF0,0x00,0x78,0x00,0x38,0x1C,0x38,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // 5
0x00,0x00,0x00,0x00,0x03,0xE0,0x07,0x00,0x0E,0x00,0x1C,0x00,0x1C,0x00,0x1F,0xF0,0x1F,0xF8,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x0F,0xF0,0x00,0x00,0x00,0x00, // 6
0x00,0x00,0x00,0x00,0x1F,0xFC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x03,0x80,0x03,0x80,0x00,0x00,0x00,0x00, // 7
0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1F,0x38,0x07,0xE0,0x07,0xE0,0x1C,0xF8,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x0F,0xF0,0x00,0x00,0x00,0x00, // 8
0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1F,0xF8,0x0F,0xF8,0x00,0x38,0x00,0x38,0x00,0x70,0x00,0xE0,0x07,0xC0,0x00,0x00,0x00,0x00, // 9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x00,0x00,0x00,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // :
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x00,0x00,0x00,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ;
0x00,0x00,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,0x00,0x70,0x00,0x00, // <
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFC,0x3F,0xFC,0x00,0x00,0x00,0x00,0x3F,0xFC,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // =
0x00,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,0x00,0x70,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x00,0x00, // >
0x00,0x00,0x03,0xC0,0x0F,0xF0,0x1E,0x78,0x18,0x38,0x00,0x38,0x00,0x70,0x00,0xE0,0x01,0xC0,0x01,0xC0,0x00,0x00,0x00,0x00,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x00,0x00, // ?
0x00,0x00,0x0F,0xF8,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xFC,0x1C,0xFC,0x1C,0xFC,0x1C,0xFC,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1F,0xF0,0x07,0xF8,0x00,0x00, // @
0x00,0x00,0x00,0x00,0x03,0xC0,0x07,0xE0,0x0E,0x70,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1F,0xF8,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x00,0x00,0x00,0x00, // A
0x00,0x00,0x00,0x00,0x1F,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0F,0xF0,0x0F,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x1F,0xF0,0x00,0x00,0x00,0x00, // B
0x00,0x00,0x00,0x00,0x07,0xF0,0x0E,0x38,0x1C,0x38,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x38,0x0E,0x38,0x07,0xF0,0x00,0x00,0x00,0x00, // C
0x00,0x00,0x00,0x00,0x1F,0xE0,0x0E,0x70,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x70,0x1F,0xE0,0x00,0x00,0x00,0x00, // D
0x00,0x00,0x00,0x00,0x1F,0xF8,0x0E,0x18,0x0E,0x08,0x0E,0x00,0x0E,0x30,0x0F,0xF0,0x0F,0xF0,0x0E,0x30,0x0E,0x00,0x0E,0x08,0x0E,0x18,0x1F,0xF8,0x00,0x00,0x00,0x00, // E
0x00,0x00,0x00,0x00,0x1F,0xF8,0x0E,0x18,0x0E,0x08,0x0E,0x00,0x0E,0x30,0x0F,0xF0,0x0F,0xF0,0x0E,0x30,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x1F,0x00,0x00,0x00,0x00,0x00, // F
0x00,0x00,0x00,0x00,0x07,0xF0,0x0E,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0xF8,0x1C,0x38,0x1C,0x38,0x0E,0x38,0x07,0xF8,0x00,0x00,0x00,0x00, // G
0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1F,0xF0,0x1F,0xF0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x00,0x00,0x00,0x00, // H
0x00,0x00,0x00,0x00,0x0F,0xE0,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x0F,0xE0,0x00,0x00,0x00,0x00, // I
0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x38,0x70,0x38,0x70,0x38,0x70,0x38,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // J
0x00,0x00,0x00,0x00,0x1E,0x38,0x0E,0x38,0x0E,0x70,0x0E,0xE0,0x0F,0xC0,0x0F,0x80,0x0F,0x80,0x0F,0xC0,0x0E,0xE0,0x0E,0x70,0x0E,0x38,0x1E,0x38,0x00,0x00,0x00,0x00, // K
0x00,0x00,0x00,0x00,0x1F,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x08,0x0E,0x18,0x0E,0x38,0x1F,0xF8,0x00,0x00,0x00,0x00, // L
0x00,0x00,0x00,0x00,0x1C,0x1C,0x1E,0x3C,0x1F,0x7C,0x1F,0xFC,0x1F,0xFC,0x1D,0xDC,0x1C,0x9C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x00,0x00,0x00, // M
0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1C,0x1E,0x1C,0x1F,0x1C,0x1F,0x9C,0x1D,0xDC,0x1C,0xFC,0x1C,0x7C,0x1C,0x3C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x00,0x00,0x00,0x00, // N
0x00,0x00,0x00,0x00,0x03,0xE0,0x07,0xF0,0x0E,0x38,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x0E,0x38,0x07,0xF0,0x03,0xE0,0x00,0x00,0x00,0x00, // O
0x00,0x00,0x00,0x00,0x1F,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0F,0xF0,0x0F,0xF0,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x1F,0x00,0x00,0x00,0x00,0x00, // P
0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,0x0E,0x38,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x7C,0x1C,0xFC,0x0F,0xF8,0x0F,0xF8,0x00,0x38,0x00,0xFC,0x00,0x00, // Q
0x00,0x00,0x00,0x00,0x1F,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0F,0xF0,0x0F,0xF0,0x0E,0x70,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x1E,0x38,0x00,0x00,0x00,0x00, // R
0x00,0x00,0x00,0x00,0x0F,0xF0,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x00,0x0F,0xE0,0x07,0xF0,0x00,0x38,0x1C,0x38,0x1C,0x38,0x1C,0x38,0x0F,0xF0,0x00,0x00,0x00,0x00, // S
0x00,0x00,0x00,0x00,0x1F,0xFC,0x19,0xCC,0x11,0xC4,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x07,0xF0,0x00,0x00,0x00,0x00, // T
0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // U
0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0E,0xE0,0x07,0xC0,0x03,0x80,0x00,0x00,0x00,0x00, // V
0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x0F,0xF8,0x0F,0xF8,0x07,0x70,0x07,0x70,0x00,0x00,0x00,0x00, // W
0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0E,0xE0,0x07,0xC0,0x03,0x80,0x03,0x80,0x07,0xC0,0x0E,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x00,0x00,0x00,0x00, // X
0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0E,0xE0,0x07,0xC0,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x0F,0xE0,0x00,0x00,0x00,0x00, // Y
0x00,0x00,0x00,0x00,0x1F,0xF8,0x1C,0x38,0x18,0x38,0x10,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x08,0x1C,0x18,0x1C,0x38,0x1F,0xF8,0x00,0x00,0x00,0x00, // Z
0x00,0x00,0x00,0x00,0x07,0xF0,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0xF0,0x00,0x00,0x00,0x00, // [
0x00,0x00,0x00,0x00,0x10,0x00,0x18,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0x07,0x00,0x00,0x00,0x00, // <Backslash>
0x00,0x00,0x00,0x00,0x07,0xF0,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x07,0xF0,0x00,0x00,0x00,0x00, // ]
0x00,0x00,0x01,0x80,0x03,0xC0,0x07,0xE0,0x0E,0x70,0x1C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0x7F,0xFF, // _
0x00,0x00,0x00,0x00,0x1C,0x00,0x1C,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // '
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,0x70,0x00,0x70,0x0F,0xF0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xD8,0x00,0x00,0x00,0x00, // a
0x00,0x00,0x00,0x00,0x1E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0F,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x1B,0xF0,0x00,0x00,0x00,0x00, // b
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x00,0x1C,0x00,0x1C,0x70,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // c
0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x70,0x00,0x70,0x00,0x70,0x0F,0xF0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xD8,0x00,0x00,0x00,0x00, // d
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x70,0x1C,0x70,0x1F,0xF0,0x1C,0x00,0x1C,0x70,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // e
0x00,0x00,0x00,0x00,0x03,0xE0,0x07,0x70,0x07,0x70,0x07,0x00,0x07,0x00,0x1F,0xE0,0x1F,0xE0,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x1F,0xC0,0x00,0x00,0x00,0x00, // f
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xD8,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xF0,0x07,0xF0,0x00,0x70,0x1C,0x70,0x0F,0xE0, // g
0x00,0x00,0x00,0x00,0x1E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0xF0,0x0F,0x38,0x0F,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x1E,0x38,0x00,0x00,0x00,0x00, // h
0x00,0x00,0x00,0x00,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x00,0x00,0x0F,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x0F,0xF8,0x00,0x00,0x00,0x00, // i
0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x00,0x03,0xF0,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x00,0x70,0x1C,0x70,0x0C,0xF0,0x07,0xE0, // j
0x00,0x00,0x00,0x00,0x1E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x38,0x0E,0x70,0x0E,0xE0,0x0F,0xC0,0x0E,0xE0,0x0E,0x70,0x0E,0x38,0x1E,0x38,0x00,0x00,0x00,0x00, // k
0x00,0x00,0x00,0x00,0x0F,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x0F,0xF8,0x00,0x00,0x00,0x00, // l
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF8,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x1C,0x9C,0x00,0x00,0x00,0x00, // m
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x00,0x00,0x00,0x00, // n
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // o
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0xF0,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0F,0xF0,0x0E,0x00,0x0E,0x00,0x1F,0x00, // p
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xB0,0x38,0xE0,0x38,0xE0,0x38,0xE0,0x38,0xE0,0x38,0xE0,0x1F,0xE0,0x00,0xE0,0x00,0xE0,0x01,0xF0, // q
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0xF0,0x0F,0xF8,0x0F,0x38,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x1F,0x00,0x00,0x00,0x00,0x00, // r
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x1C,0x30,0x1C,0x30,0x0F,0x80,0x03,0xE0,0x18,0x70,0x18,0x70,0x0F,0xE0,0x00,0x00,0x00,0x00, // s
0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x07,0x00,0x1F,0xF0,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x70,0x07,0x70,0x03,0xE0,0x00,0x00,0x00,0x00, // t
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0F,0xD8,0x00,0x00,0x00,0x00, // u
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x70,0x0E,0xE0,0x07,0xC0,0x03,0x80,0x00,0x00,0x00,0x00, // v
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x9C,0x1C,0x9C,0x0F,0xF8,0x07,0x70,0x07,0x70,0x00,0x00,0x00,0x00, // w
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0xE0,0x1C,0xE0,0x0F,0xC0,0x07,0x80,0x07,0x80,0x0F,0xC0,0x1C,0xE0,0x1C,0xE0,0x00,0x00,0x00,0x00, // x
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x0E,0x38,0x07,0xF0,0x03,0xE0,0x00,0xE0,0x01,0xC0,0x1F,0x80, // y
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x18,0xE0,0x11,0xC0,0x03,0x80,0x07,0x00,0x0E,0x20,0x1C,0x60,0x1F,0xE0,0x00,0x00,0x00,0x00, // z
0x00,0x00,0x00,0x00,0x01,0xF8,0x03,0x80,0x03,0x80,0x03,0x80,0x07,0x00,0x1C,0x00,0x1C,0x00,0x07,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x01,0xF8,0x00,0x00,0x00,0x00, // {
0x00,0x00,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x00,0x00, // |
0x00,0x00,0x00,0x00,0x1F,0x80,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x00,0xE0,0x00,0x38,0x00,0x38,0x00,0xE0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00, // }
0x00,0x00,0x00,0x00,0x1F,0x1C,0x3B,0x9C,0x39,0xDC,0x38,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00  // ~
};

void Write_Command(uint16_t Wcommand){
	LCD_RD = 0x20;		//Delay(1);
	LCD_RS = 0x00;		//Delay(1);
	GPIO_PORTB_DATA_R = Wcommand & 0x00FF;
	GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & 0xE1) + ((Wcommand & 0x0F00) >> 7);
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & 0xE1) + ((Wcommand & 0xF000) >> 11);
	LCD_WR = 0x00;		//Delay(1);
	LCD_WR = 0x10;		//Delay(1);
}

void Write_Data(uint16_t Wdata){
	LCD_RD = 0x20;		//Delay(1);
	LCD_RS = 0x08;		//Delay(1);
	GPIO_PORTB_DATA_R = Wdata & 0x00FF;
	GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R & 0xE1) + ((Wdata & 0x0F00) >> 7);
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & 0xE1) + ((Wdata & 0xF000) >> 11);
	LCD_WR = 0x00;		//Delay(1);
	LCD_WR = 0x10;		//Delay(1);
}

void Write_Command_Data(uint16_t Wcommand,uint16_t Wdata){
    Write_Command(Wcommand);
    Write_Data(Wdata);
}

void Delay(uint32_t X){
	uint32_t n = X;
	uint32_t volatile tick;
  	while(n){
    	tick = 72724*2/91;  // 1msec
    	while(tick){
      		tick--;
    	}
    	n--;
  	}	
}
void LCD_Set_Address(uint16_t PX1,uint16_t PY1,uint16_t PX2,uint16_t PY2){
	Write_Command_Data(68,(PY2 << 8) + PY1 );  //Column address start2
	Write_Command_Data(69,PX1);        	//Column address start1
	Write_Command_Data(70,PX2);  		//Column address end2
	Write_Command_Data(78,PY1);        	//Column address end1
	Write_Command_Data(79,PX1);  		//Row address start2
	Write_Command(34);
}

void LCD_Init(void){
	DisableInterrupts();
	SYSCTL_RCGCGPIO_R |= 0x33;
	while((SYSCTL_PRGPIO_R&0x33)!=0x33){};

	GPIO_PORTA_LOCK_R = 0x4C4F434B;   	// unlock
	GPIO_PORTA_CR_R |= 0xF8;           	// allow changes
	GPIO_PORTA_DIR_R |= 0xF8;			// output
	GPIO_PORTA_AFSEL_R &= ~0xF8;		// disable alt funct    
	GPIO_PORTA_DEN_R |= 0xF8;   		// enable digital I/O    
	GPIO_PORTA_PCTL_R &= ~0xFFFFF000; 	// configure as GPIO
	GPIO_PORTA_AMSEL_R &= ~0xF8;  		// disable analog function

	GPIO_PORTB_LOCK_R = 0x4C4F434B;   	// unlock
	GPIO_PORTB_CR_R |= 0xFF;           	// allow changes
	GPIO_PORTB_DIR_R |= 0xFF;			// output
	GPIO_PORTB_AFSEL_R &= ~0xFF;		// disable alt funct    
	GPIO_PORTB_DEN_R |= 0xFF;   		// enable digital I/O    
	GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF; 	// configure as GPIO
	GPIO_PORTB_AMSEL_R &= ~0xFF;  		// disable analog function
	GPIO_PORTB_PUR_R |= 0xFF;

	GPIO_PORTE_LOCK_R = 0x4C4F434B;   	// unlock
	GPIO_PORTE_CR_R |= 0x1E;           	// allow changes
	GPIO_PORTE_DIR_R |= 0x1E;			// output
	GPIO_PORTE_AFSEL_R &= ~0x1E;		// disable alt funct    
	GPIO_PORTE_DEN_R |= 0x1E;   		// enable digital I/O    
	GPIO_PORTE_PCTL_R &= ~0x000FFFF0; 	// configure as GPIO
	GPIO_PORTE_AMSEL_R &= ~0x1E;  		// disable analog function

	GPIO_PORTF_LOCK_R = 0x4C4F434B;   	// unlock
	GPIO_PORTF_CR_R |= 0x1E;           	// allow changes
	GPIO_PORTF_DIR_R |= 0x1E;			// output
	GPIO_PORTF_AFSEL_R &= ~0x1E;		// disable alt funct    
	GPIO_PORTF_DEN_R |= 0x1E;   		// enable digital I/O    
	GPIO_PORTF_PCTL_R &= ~0x000FFFF0; 	// configure as GPIO
	GPIO_PORTF_AMSEL_R &= ~0x1E;  		// disable analog function
	EnableInterrupts();

	LCD_RD = 0x20;		Delay(1);
	LCD_RST = 0x80;		Delay(1);
	Delay(5);
	LCD_RST = 0x00;		Delay(1);
	Delay(15);
	LCD_RST = 0x80;		Delay(1);
	Delay(15);
	LCD_CS = 0x00;		Delay(1);
	
	Write_Command_Data(0x0000,0x0001); Delay(1);
	Write_Command_Data(0x0003,0xA8A4); Delay(1);
	Write_Command_Data(0x000C,0x0000); Delay(1);
	Write_Command_Data(0x000D,0x800C); Delay(1);
	Write_Command_Data(0x000E,0x2B00); Delay(1);
	Write_Command_Data(0x001E,0x00B7); Delay(1);
	Write_Command_Data(0x0001,0x2B3F); Delay(1);
	Write_Command_Data(0x0002,0x0600); Delay(1);
	Write_Command_Data(0x0010,0x0000); Delay(1);
	Write_Command_Data(0x0011,0x6070); Delay(1);
	Write_Command_Data(0x0005,0x0000); Delay(1);
	Write_Command_Data(0x0006,0x0000); Delay(1);
	Write_Command_Data(0x0016,0xEF1C); Delay(1);
	Write_Command_Data(0x0017,0x0003); Delay(1);
	Write_Command_Data(0x0007,0x0233); Delay(1);
	Write_Command_Data(0x000B,0x0000); Delay(1);
	Write_Command_Data(0x000F,0x0000); Delay(1);
	Write_Command_Data(0x0041,0x0000); Delay(1);
	Write_Command_Data(0x0042,0x0000); Delay(1);
	Write_Command_Data(0x0048,0x0000); Delay(1);
	Write_Command_Data(0x0049,0x013F); Delay(1);
	Write_Command_Data(0x004A,0x0000); Delay(1);
	Write_Command_Data(0x004B,0x0000); Delay(1);
	Write_Command_Data(0x0044,0xEF95); Delay(1);
	Write_Command_Data(0x0045,0x0000); Delay(1);
	Write_Command_Data(0x0046,0x013F); Delay(1);
	Write_Command_Data(0x0030,0x0707); Delay(1);
	Write_Command_Data(0x0031,0x0204); Delay(1);
	Write_Command_Data(0x0032,0x0204); Delay(1);
	Write_Command_Data(0x0033,0x0502); Delay(1);
	Write_Command_Data(0x0034,0x0507); Delay(1);
	Write_Command_Data(0x0035,0x0204); Delay(1);
	Write_Command_Data(0x0036,0x0204); Delay(1);
	Write_Command_Data(0x0037,0x0502); Delay(1);
	Write_Command_Data(0x003A,0x0302); Delay(1);
	Write_Command_Data(0x003B,0x0302); Delay(1);
	Write_Command_Data(0x0023,0x0000); Delay(1);
	Write_Command_Data(0x0024,0x0000); Delay(1);
	Write_Command_Data(0x0025,0x8000); Delay(1);
	Write_Command_Data(0x004f,0x0000); Delay(1);
	Write_Command_Data(0x004e,0x0000); Delay(1); 
	Write_Command(0x0022); Delay(1);
	LCD_CS = 0x40;		Delay(1);
	LCD_Fill(LCD_RED);
}


uint16_t LCD_Color(uint16_t r,uint16_t g,uint16_t b){
	uint16_t temp;
	temp = r & 0x001F;
	temp |= (g & 0x003F) << 5; 
	temp |= (b & 0x001F) << 11;
	return temp;
}


void LCD_Fill(uint16_t color){
	uint16_t i,j;
	LCD_CS  = 0x00;
	LCD_Set_Address(0,0,319,239);
	Write_Data(color);
	for(i = 0; i <= 239; i++){
		for(j = 0; j <= 319; j++){
			LCD_WR = 0x00;
			LCD_WR = 0x10;
		}
	}
	LCD_CS = 0x40;	
}


void LCD_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color){
	uint16_t  i,j;
	LCD_CS  = 0x00;		
	LCD_Set_Address(x1,y1,x2,y2);
	Write_Data(color);
	for(i = y1; i <= y2; i++){
		for(j = x1; j <= x2; j++){
			LCD_WR = 0x00;
			LCD_WR = 0x10;
		}
	}
	LCD_CS = 0x40;
}


void LCD_Dot(uint16_t x,uint16_t y,uint16_t color){
	LCD_CS  = 0x00;	
	LCD_Set_Address(x,y,x,y);
	Write_Data(color);
	LCD_CS = 0x40;	
}

void LCD_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color){
	int x,y,addx,addy,dx,dy;
	long P;
	uint16_t i;

	x = x1;
	y = y1;
	if(x1 > x2){
		addx = -1;
		dx = x1-x2;
	} else {
		addx = 1;
		dx = x2 - x1;
	}
	if(y1 > y2){
		addy = -1;
		dy = y1 - y2;
	} else {
		addy = 1;
		dy = y2 - y1;
	}

	if(dx >= dy){
		P = (2*dy) - dx;
		for(i = 1; i <= (dx +1); i++){
			LCD_Dot(x,y,color);
			if(P < 0){
				P = P + (2*dy);
				x = (x + addx);
			} else {
				P = P+(2*dy) - (2*dx);
				x = x + addx;
				y = y + addy;
			}
		}
	} else {
		P = (2*dx) - dy;
		for(i = 1; i <= (dy +1); i++){
			LCD_Dot(x,y,color);
			if(P<0){
				P = P + (2*dx);
				y = y + addy;
			} else {
				P = P + (2*dx) - (2*dy);
				x = x + addx;
				y = y + addy;
			}
		}
	}
}

void LCD_H_Line(uint16_t x1,uint16_t x2,uint16_t y_pos,uint16_t color){
	LCD_Box(x1,y_pos,x2,y_pos+1,color);
}

void LCD_V_Line(uint16_t y1,uint16_t y2,uint16_t x_pos,uint16_t color){
	LCD_Box(x_pos,y1,x_pos+1,y2,color);
}

void LCD_Rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color){
	LCD_H_Line(x1,x2,y1,color);
	LCD_H_Line(x1,x2,y2,color);
	LCD_V_Line(y1,y2,x1,color);
	LCD_V_Line(y1,y2,x2,color);
}

void LCD_Circle(uint16_t x,uint16_t y,uint16_t radius,uint16_t fill,uint16_t color){
	int a_,b_,P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_){
		if(fill == 1){
			LCD_Box(x-a_,y-b_,x+a_,y+b_,color);
			LCD_Box(x-b_,y-a_,x+b_,y+a_,color);
		} else {
			LCD_Dot(a_+x, b_+y, color);
			LCD_Dot(b_+x, a_+y, color);
			LCD_Dot(x-a_, b_+y, color);
			LCD_Dot(x-b_, a_+y, color);
			LCD_Dot(b_+x, y-a_, color);
			LCD_Dot(a_+x, y-b_, color);
			LCD_Dot(x-a_, y-b_, color);
			LCD_Dot(x-b_, y-a_, color);
		}

		if (P < 0 ){
			P = (P + 3) + (2* a_);
			a_ ++;
		} else {
			P = (P + 5) + (2* (a_ - b_));
			a_ ++;
			b_ --;
		}
	}
}

void LCD_Char(char C,uint16_t x,uint16_t y,uint16_t DimFont,uint16_t Fcolor,uint16_t Bcolor){
	const uint16_t *ptrfont;
	uint16_t Cptrfont;
	uint16_t font8x8[8];
	uint16_t font16x16[16];
	uint16_t k,i,print1,print2;
	uint16_t print3,print4;

	if(DimFont == 8){
		ptrfont = FONT_8x8;
		Cptrfont = (C-32)*8;
		ptrfont = ptrfont + Cptrfont;

		for(k = 0; k <= 7; k++){
			font8x8[k] = *ptrfont;
			ptrfont++;
		}

		LCD_CS = 0;
		LCD_Set_Address(x,y,x+7,y+7);
		for(i = 8; i > 0; i--){
			for(k = 8; k > 0; k--){
				print1 = font8x8[k-1] >> (i-1);
				print2 = print1 & 0x01;

				if(print2 == 1){
					Write_Data(Fcolor);
				} else {
					Write_Data(Bcolor);
				}
			}
		}
		LCD_CS = 1;
	} else if(DimFont == 16){
		ptrfont = FONT_16x16;
		Cptrfont = (C-32)*32;
		ptrfont = ptrfont + Cptrfont;

		for(k = 0; k <= 15; k++){
			font16x16[k] = *ptrfont;
			ptrfont++;
			font16x16[k] = (font16x16[k] << 8);
			font16x16[k] = font16x16[k] + *ptrfont;
			ptrfont++;
		}

		LCD_CS = 0;
		LCD_Set_Address(x,y,x+15,y+15);
		for(i = 0; i <= 15; i++){
			for(k = 0; k <= 15; k++){
				print3 = (font16x16[i] & 0x8000);
				print4 = print3 >>15;

				if(print4 == 1){
					Write_Data(Fcolor);
				} else {
					Write_Data(Bcolor);
				}
				font16x16[i] = font16x16[i] << 1;
			}
		}
		LCD_CS = 1;
	}

}


void LCD_Text(char* S,uint16_t x,uint16_t y,uint16_t DimFont,uint16_t Fcolor,uint16_t Bcolor){
	int lenght,cnt;
	uint16_t buffer[24];
	lenght = strlen(S);

	for(cnt = 0; cnt <= (lenght - 1); cnt++){
		buffer[cnt] = S[cnt];
	}

	if(DimFont == 8){
		for(cnt = 0; cnt <= (lenght - 1); cnt++){
			LCD_Char(buffer[cnt],x,y,DimFont,Fcolor,Bcolor);
			x = x + 8;
		}
	} else if(DimFont == 16){
		for(cnt = 0; cnt <= (lenght - 1); cnt++){
			LCD_Char(buffer[cnt],x,y,DimFont,Fcolor,Bcolor);
			x = x + 16;
		}
	}
}


void LCD_Image(uint16_t pos_x,uint16_t pos_y,uint16_t dim_x,uint16_t dim_y,const uint16_t *picture){
	uint16_t x, y;

	LCD_CS  = 0x00;
	LCD_Set_Address(pos_x, pos_y, pos_x + dim_x - 1, pos_y + dim_y - 1);
	for(y = pos_y; y < (pos_y + dim_y); y++ ){
		for(x = pos_x; x < (pos_x + dim_x); x++ ){
			Write_Data(*picture++);
		}
	}
	LCD_CS = 0x40;
}
