#ifndef _lcddriver_
#define _lcddriver_

// Color definitions
#define LCD_BLACK   0x0000
#define LCD_BLUE    0xF800
#define LCD_RED     0x001F
#define LCD_GREEN   0x07E0
#define LCD_CYAN    0xFFE0
#define LCD_MAGENTA 0xF81F
#define LCD_YELLOW  0x07FF
#define LCD_WHITE   0xFFFF

void LCD_Set_Address(uint16_t PX1,uint16_t PY1,uint16_t PX2,uint16_t PY2);

void LCD_Init(void);

uint16_t LCD_Color(uint16_t r,uint16_t g,uint16_t b);

void LCD_Fill(uint16_t color);

void LCD_Box(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);

void LCD_Dot(uint16_t x,uint16_t y,uint16_t color);

void LCD_Line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);

void LCD_H_Line(uint16_t x1,uint16_t x2,uint16_t y_pos,uint16_t color);

void LCD_V_Line(uint16_t y1,uint16_t y2,uint16_t x_pos,uint16_t color);

void LCD_Rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);

void LCD_Circle(uint16_t x,uint16_t y,uint16_t radius,uint16_t fill,uint16_t color);

void LCD_Char(char C,uint16_t x,uint16_t y,uint16_t DimFont,uint16_t Fcolor,uint16_t Bcolor);

void LCD_Text(char* S,uint16_t x,uint16_t y,uint16_t DimFont,uint16_t Fcolor,uint16_t Bcolor);

void LCD_Image(uint16_t pos_x,uint16_t pos_y,uint16_t dim_x,uint16_t dim_y,const uint16_t *picture);

#endif
