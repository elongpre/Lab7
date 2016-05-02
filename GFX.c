#include "GFX.h"
#include "Background.h"
#include "LCD.h"

uint16_t GFX_Ball_X, GFX_Ball_Y;
uint16_t GFX_Pad0_Y;
uint16_t GFX_Pad1_Y;

void GFX_Init(void){
	GFX_Ball_X = 160;
	GFX_Ball_Y = GFX_Pad0_Y = GFX_Pad1_Y = 120;
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
	uint16_t Lcolor, Dcolor;
	if((x<4)||(y<4)||(x>315)||(y>235)){
		return;
	}
	LCD_Box(GFX_Ball_X-4, GFX_Ball_Y-4, GFX_Ball_X+4, GFX_Ball_Y+4, LCD_BLACK);
	if(option == 0){
		Dcolor = DTUR;
		Lcolor = LTUR;
	} else {
		Dcolor = DRED;
		Lcolor = LRED;
	}
	LCD_Box(x-1, y+4, x+1, y+2, Dcolor);
	LCD_Box(x-1, y-4, x+1, y-2, Dcolor);
	LCD_Box(x+4, y-1, x+2, y+1, Dcolor);
	LCD_Box(x-4, y-1, x-2, y+1, Dcolor);
	LCD_Dot(x-1, y-1, Dcolor);
	LCD_Dot(x-1, y+1, Dcolor);
	LCD_Dot(x+1, y-1, Dcolor);
	LCD_Dot(x+1, y+1, Dcolor);
	LCD_Dot(x-3, y-2, Dcolor);
	LCD_Dot(x-3, y+2, Dcolor);
	LCD_Dot(x+3, y-2, Dcolor);
	LCD_Dot(x+3, y+2, Dcolor);
	LCD_Dot(x-2, y-3, Dcolor);
	LCD_Dot(x-2, y+3, Dcolor);
	LCD_Dot(x+2, y-3, Dcolor);
	LCD_Dot(x+2, y+3, Dcolor);
	LCD_Box(x-1, y+3, x+1, y+3, Lcolor);	
	LCD_Box(x-1, y-3, x+1, y-3, Lcolor);
	LCD_Box(x+3, y-1, x+3, y+1, Lcolor);
	LCD_Box(x-3, y-1, x-3, y+1, Lcolor);
	LCD_Box(x-2, y+2, x-1, y+2, Lcolor);	
	LCD_Box(x-2, y-2, x-1, y-2, Lcolor);
	LCD_Box(x+2, y+2, x+1, y+2, Lcolor);	
	LCD_Box(x+2, y-2, x+1, y-2, Lcolor);
	LCD_Dot(x-2, y-1, Lcolor);
	LCD_Dot(x-2, y+1, Lcolor);
	LCD_Dot(x+2, y-1, Lcolor);
	LCD_Dot(x+2, y+1, Lcolor);
	GFX_Ball_X = x;
	GFX_Ball_Y = y;
}


void GFX_BallDel(void){
	LCD_Box(GFX_Ball_X-4, GFX_Ball_Y-4, GFX_Ball_X+4, GFX_Ball_Y+4, LCD_BLACK);
}

void GFX_Paddle(uint16_t x, uint16_t y, uint16_t option){
	uint16_t xline;
	uint16_t pad_h = PADDLEH/2;
	uint16_t top = y+pad_h;
	if((y>(239-pad_h))||(y<pad_h)){
		return;
	}
	if(x==0){
		LCD_Box(0, GFX_Pad0_Y+pad_h, 15, GFX_Pad0_Y-pad_h, LCD_BLACK);
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
		GFX_Pad0_Y = y;
	} else {
		LCD_Box(319, GFX_Pad1_Y+pad_h, 304, GFX_Pad1_Y-pad_h, LCD_BLACK);
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
		GFX_Pad1_Y = y;

	}



}
