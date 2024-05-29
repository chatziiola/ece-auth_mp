#include "stdint.h"

void timerEnable();
void timerDisable();
void timerSetCallback(void (*callback)());
void timerInitialize(uint32_t duration);