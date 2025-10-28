#include "stm32f103xb.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    for(uint32_t i=0;i<ms*8000;i++) __NOP();
}

void pwm_init(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);
    GPIOA->CRL |=  (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);
    GPIOA->CRL |=   GPIO_CRL_CNF2_1;

    GPIOA->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
    GPIOA->CRL |=  (GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);
    GPIOA->CRL |=   GPIO_CRL_CNF3_1;

    TIM2->PSC  = 7;
    TIM2->ARR  = 1999;

    // ---- CH3 (IN1) ----
    TIM2->CCR3 = 500; // duty
    TIM2->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC3PE);
    TIM2->CCMR2 |=  (6 << TIM_CCMR2_OC3M_Pos);
    TIM2->CCMR2 |=   TIM_CCMR2_OC3PE;

    // ---- CH4 (IN2) ----
    TIM2->CCR4 = 500; // duty
    TIM2->CCMR2 &= ~(TIM_CCMR2_OC4M | TIM_CCMR2_OC4PE);
    TIM2->CCMR2 |=  (6 << TIM_CCMR2_OC4M_Pos);
    TIM2->CCMR2 |=   TIM_CCMR2_OC4PE;

    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM2->EGR  = TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;
}

int main(void){
    pwm_init();

    while(1){
        TIM2->CCER |=  TIM_CCER_CC3E;   // IN1 = PWM
        TIM2->CCER &= ~TIM_CCER_CC4E;   // IN2 = 0
        delay_ms(100);

        // --- Atrás: CH3 apagado, CH4 activo ---
        TIM2->CCER &= ~TIM_CCER_CC3E;   // IN1 = 0
        TIM2->CCER |=  TIM_CCER_CC4E;   // IN2 = PWM
        delay_ms(100);

        // --- Stop: ambos OFF ---
        TIM2->CCER &= ~(TIM_CCER_CC3E | TIM_CCER_CC4E);
        delay_ms(200);
    }
}
