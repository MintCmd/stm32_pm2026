#include <stdint.h>
#include <stm32f10x.h>

void delay_ms(volatile uint32_t ms) {
    volatile uint32_t count = ms * 8000;
    while (count--) {
        __NOP();
    }
}

int __attribute__((noreturn)) main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    GPIOC->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
    GPIOC->CRH |= GPIO_CRH_CNF14_1;
    GPIOC->ODR |= GPIO_ODR_ODR14;

    GPIOC->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
    GPIOC->CRH |= GPIO_CRH_CNF15_1;
    GPIOC->ODR |= GPIO_ODR_ODR15;

    uint32_t delay_multiplier = 16;

    while (1) {
        if (!(GPIOC->IDR & GPIO_IDR_IDR14)) {
            delay_ms(20);
            if (!(GPIOC->IDR & GPIO_IDR_IDR14)) {
                if (delay_multiplier > 1) {
                    delay_multiplier /= 2;
                }
                uint32_t timeout = 0;
                while (!(GPIOC->IDR & GPIO_IDR_IDR14) && timeout < 5) {
                    delay_ms(20);
                    timeout++;
                }
            }
        }

        if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
            delay_ms(20);
            if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
                if (delay_multiplier < 64) {
                    delay_multiplier *= 2;
                }
                uint32_t timeout = 0;
                while (!(GPIOC->IDR & GPIO_IDR_IDR15) && timeout < 5) {
                    delay_ms(20);
                    timeout++;
                }
            }
        }

        GPIOC->ODR |= GPIO_ODR_ODR13;
        for (uint32_t i = 0; i < delay_multiplier; i++) {
            delay_ms(30); // Базовый шаг 30 мс
        }

        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        for (uint32_t i = 0; i < delay_multiplier; i++) {
            delay_ms(30);
        }
    }
}