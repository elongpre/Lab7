// Game.c
// Evan Longpre and Alex Wishlinski
// 30 Mar. 2016
// Game driver for Lab 8
// Lab 8
// Mahesh Srinivasan
// 30 Mar. 2016 

#include "inc/tm4c123gh6pm.h"
#include "Music.h"
#include "PLL.h"
#include "Switch.h"
#include "ST7735.h"
#include "DAC.h"
#include "LCD.h"
#include "Background.h"


#define PF1   (*((volatile uint32_t *)0x40025008))
#define PF4   (*((volatile uint32_t *)0x40025040))

const uint32_t PlayerMode[3] = {0, 1, 2}; //Mode 0 is demo mode, Mode 1 is Computer vs. Human, and Mode 2 is Human vs. Human
const int32_t Angle[9] = {20,40,60,80,90,100,120,140,160}
const int32_t X_Diff[9] = {940,766,500,174,0,-174,-500,-766,-940}
const int32_t Y_Diff[9] = {342,643,866,985,1000,985,866,643,342}

void movePaddle();
void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y);
void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y);


uint16_t Game_Play(uint16_t option){
	//play game
}

void movePaddle(){

// Potentiometer or computer generated
// call graphics function for paddle 

	GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);

}

void ballTrajectory(int32_t angle, int32_t curr_x, int32_t curr_y){
	//--> increment over slope
	uint32_t i, j;
	for(i=0;i<9;i++){
		if Angle[i] == angle {
			j = i;
		}
	}
	int32_t new_y = (curr_y*1000 + Y_Diff[j])/1000;
	int32_t new_x = (curr_x*1000 + X_Diff[j])/1000;
	GFX_Ball(new_x, new_y, null);

}

void ballBounce(int32_t angle, int32_t curr_x, int32_t curr_y){

// Paddle Hit location  --> center of ball? it passes if the ball edge won't hit paddle edge (so center is as from away from the paddle corner as the length of the radius)
// call graphics function for ball



	GFX_Ball(uint16_t x, uint16_t y, uint16_t option);

}
