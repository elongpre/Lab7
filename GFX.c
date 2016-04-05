#include "GFX.h"
#include "Background.h"
#include "LCD.h"

#define PADDLEH	39
#define PADDLEW	5
#define PADDLEF	5
#define BALLR 	5

uint16_t Ball_X, Ball_Y;
uint16_t Paddle0_Y;
uint16_t Paddle1_Y;

void GFX_Init(void){
	Ball_X = 160;
	Ball_Y = Paddle0_Y = Paddle1_Y = 120;
	LCD_Fill(LCD_BLACK);
}
void GFX_Background(uint16_t option){
	uint16_t i, j;
	for(i=0; i<320; i+=80){
		for(j=0; j<240; j+=80){
			LCD_Image(i, j, 80, 80, background);
		}
	}
}

void GFX_Ball(uint16_t x, uint16_t y, uint16_t option){
	LCD_Circle(Ball_X, Ball_Y, BALLR, 1, LCD_BLACK);
	LCD_Circle(x, y, BALLR, 1, LCD_RED);
	Ball_X = x;
	Ball_Y = y;
}

void GFX_Paddle(uint16_t x, uint16_t y, uint16_t option){
	uint16_t pad_h = PADDLEH/2;
	if(x==0){
		if(y>Paddle0_Y){
			LCD_Box(PADDLEF, Paddle0_Y-pad_h, PADDLEF+PADDLEW, y-pad_h, LCD_BLACK);
		} else {
			LCD_Box(PADDLEF, y+pad_h, PADDLEF+PADDLEW, Paddle0_Y+pad_h, LCD_BLACK);
		}
		LCD_Box(PADDLEF, y-pad_h, PADDLEF+PADDLEW, y+pad_h, LCD_RED);
		Paddle0_Y = y;
	} else {
		if(y>Paddle1_Y){
			LCD_Box(320-PADDLEF-PADDLEW, Paddle1_Y-pad_h, 320-PADDLEF, y-pad_h, LCD_BLACK);
		} else {
			LCD_Box(320-PADDLEF-PADDLEW, y+pad_h, 320-PADDLEF, Paddle1_Y+pad_h, LCD_BLACK);
		}
		LCD_Box(320-PADDLEF-PADDLEW, y-pad_h, 320-PADDLEF, y+pad_h, LCD_RED);
		Paddle1_Y = y;
	}
}
