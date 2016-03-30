#include <stdint.h>

void Switch_Init(void);

void GPIOPortE_Handler(void);

uint32_t Switch_GetButton(void);

void Switch_ResetButton(void);
