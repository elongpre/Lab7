#include <stdint.h>

#define PADDLEH	39
#define PADDLEW	5
#define PADDLEF	5
#define BALLR 	5

void GFX_Init(void);

void GFX_Background(uint16_t option);

void GFX_Ball(uint16_t x, uint16_t y, uint16_t option);

void GFX_BallDel(void);

void GFX_Paddle(uint16_t x, uint16_t y, uint16_t option);
