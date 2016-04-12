#include "GFX.h"
#include "Background.h"
#include "LCD.h"

#define LTUR 	0xC61E
#define DTUR	0x529A
#define LRED	0xF618
#define DRED	0xD28A
#define GRY		0x4204
#define LGRY	0x7388
#define DGRY	0x2102

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
	LCD_Init();
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

void GFX_BallDel(void){
	LCD_Circle(Ball_X, Ball_Y, BALLR, 1, LCD_BLACK);
}

void GFX_OldPaddle(uint16_t x, uint16_t y, uint16_t option){
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

void GFX_Paddle(uint16_t x, uint16_t y, uint16_t option){
	uint16_t xline;
	uint16_t pad_h = PADDLEH/2;
	uint16_t top = y+pad_h;
	if(x==0){
		LCD_Box(0, Paddle0_Y+pad_h, 15, Paddle0_Y-pad_h, LCD_BLACK);
		xline = 15;
		LCD_Box(xline, top-1, xline, top-37, DTUR);		//1
		xline--;
		LCD_Box(xline, top, xline, top-1, DTUR);		//2
		LCD_Box(xline, top-2, xline, top-36, LTUR);	
		LCD_Box(xline, top-37, xline, top-38, DTUR);	
		xline--;
		LCD_Box(xline, top-1, xline, top-37, DTUR);		//3
		xline--;
		LCD_Box(xline, top-2, xline, top-36, GRY);		//4
		LCD_Dot(xline, top-14, LGRY);
		LCD_Dot(xline, top-24, DGRY);
		xline--;
		LCD_Box(xline, top-3, xline, top-35, GRY);		//5
		xline--;
		LCD_Box(xline, top-5, xline, top-33, GRY);		//6
		LCD_Box(xline, top-13, xline+1, top-13, LGRY);
		LCD_Box(xline, top-25, xline+1, top-25, DGRY);
		xline--;
		LCD_Box(xline, top-7, xline, top-31, GRY);
		xline--;
		LCD_Box(xline, top-10, xline, top-28, GRY);
		xline--;
		LCD_Box(xline, top-13, xline-1, top-25, GRY);
		xline--;
		LCD_Box(xline, top-12, xline+3, top-12, LGRY);
		LCD_Box(xline, top-26, xline+3, top-26, DGRY);		
		xline--;
		LCD_Box(xline, top-14, xline-1, top-24, GRY);
		xline--;
		LCD_Box(xline, top-13, xline+1, top-13, LGRY);
		LCD_Box(xline, top-25, xline+1, top-25, DGRY);
		xline--;
		LCD_Box(xline, top-15, xline, top-24, GRY);
		LCD_Dot(xline, top-14, LGRY);
		LCD_Dot(xline, top-24, DGRY);
		xline--;
		LCD_Box(xline, top-16, xline, top-23, GRY);
		LCD_Dot(xline, top-15, LGRY);
		LCD_Dot(xline, top-23, DGRY);
		xline--;
		LCD_Box(xline, top-17, xline, top-21, GRY);
		Paddle0_Y = y;
	} else {
		LCD_Box(319, Paddle1_Y+pad_h, 304, Paddle1_Y-pad_h, LCD_BLACK);
		xline = 304;
		LCD_Box(xline, top-1, xline, top-37, DRED);		//1
		xline++;
		LCD_Box(xline, top, xline, top-1, DRED);		//2
		LCD_Box(xline, top-2, xline, top-36, LRED);	
		LCD_Box(xline, top-37, xline, top-38, DRED);	
		xline++;
		LCD_Box(xline, top-1, xline, top-37, DRED);		//3
		xline++;
		LCD_Box(xline, top-2, xline, top-36, GRY);		//4
		LCD_Dot(xline, top-14, LGRY);
		LCD_Dot(xline, top-24, DGRY);
		xline++;
		LCD_Box(xline, top-3, xline, top-35, GRY);		//5
		xline++;
		LCD_Box(xline, top-5, xline, top-33, GRY);		//6
		LCD_Box(xline, top-13, xline-1, top-13, LGRY);
		LCD_Box(xline, top-25, xline-1, top-25, DGRY);
		xline++;
		LCD_Box(xline, top-7, xline, top-31, GRY);
		xline++;
		LCD_Box(xline, top-10, xline, top-28, GRY);
		xline++;
		LCD_Box(xline, top-13, xline+1, top-25, GRY);
		xline++;
		LCD_Box(xline, top-12, xline-3, top-12, LGRY);
		LCD_Box(xline, top-26, xline-3, top-26, DGRY);		
		xline++;
		LCD_Box(xline, top-14, xline+1, top-24, GRY);
		xline++;
		LCD_Box(xline, top-13, xline-1, top-13, LGRY);
		LCD_Box(xline, top-25, xline-1, top-25, DGRY);
		xline++;
		LCD_Box(xline, top-15, xline, top-24, GRY);
		LCD_Dot(xline, top-14, LGRY);
		LCD_Dot(xline, top-24, DGRY);
		xline++;
		LCD_Box(xline, top-16, xline, top-23, GRY);
		LCD_Dot(xline, top-15, LGRY);
		LCD_Dot(xline, top-23, DGRY);
		xline++;
		LCD_Box(xline, top-17, xline, top-21, GRY);
		Paddle1_Y = y;

	}



}
