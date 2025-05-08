#include "stm32f103xb.h"

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) __NOP();
}

void pwm_init(void) {
    // 1. Habilitar reloj para GPIOA y TIM2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Configurar PA0 como salida alternativa push-pull (TIM2_CH1)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1;

    // 3. Configurar TIM2 en modo PWM
    TIM2->PSC = 7;
    TIM2->ARR = 1999;
    TIM2->CCR1 = 0;

    // 4. Configurar canal 1 de TIM2 para PWM
    TIM2->CCMR1 |= (6 << 4);
    TIM2->CCER |= TIM_CCER_CC1E;

    // 5. Habilitar el contador de TIM2
    TIM2->CR1 |= TIM_CR1_CEN;
}

void increment_pwm_duty(void) {
    if (TIM2->CCR1 < 2000) {
        TIM2->CCR1 += 400;
    } else {
        TIM2->CCR1 = 0;
    }
}

int main(void) {
    pwm_init();

    while (1) {
        increment_pwm_duty();

        delay_ms(50);
    }
}
