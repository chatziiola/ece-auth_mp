#include "stm32f4xx.h"

void timerInitialize() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  // Enable TIM2 clock
	
	TIM2->PSC = 16000 - 1;  // Set prescaler to divide clock by 16000
	TIM2->ARR = 1000 - 1;   // Set auto-reload value for 1 second interval
	TIM2->EGR = TIM_EGR_UG; // Generate an update event to load the prescaler value
	
	TIM2->DIER |= TIM_DIER_UIE;  // Enable update interrupt (UIE)

	NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt in NVIC
	NVIC_SetPriority(TIM2_IRQn, 1);  // Set priority (optional)
}

void timerEnable() {
	TIM2->CR1 |= TIM_CR1_CEN;  // Enable the counter
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {  // Check if update interrupt flag is set
        TIM2->SR &= ~TIM_SR_UIF;  // Clear the update interrupt flag
        // Your code here (e.g., toggle an LED)
    }
}