#include <stdint.h>

#define LTUR 	0xC61E
#define DTUR	0x529A
#define LRED	0xF618
#define DRED	0xD28A
#define GRY		0x4204
#define LGRY	0x7388
#define DGRY	0x2102

#define PADDLEH	39	
#define PADDLEW	16
#define BALLR 	5

void GFX_Init(void);

void GFX_Background(uint16_t option);

void GFX_Ball(uint16_t x, uint16_t y, uint16_t option);

void GFX_BallDel(void);

void GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);
