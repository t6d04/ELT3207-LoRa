#include "button.h"

volatile uint8_t system_powered_on = 0;
volatile uint8_t buzzer_should_off = 0;

void button_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // PC13 - ON/OFF
    BTN_ONOFF_PORT->MODER &= ~(0x3 << (BTN_ONOFF_PIN * 2));
    BTN_ONOFF_PORT->PUPDR &= ~(0x3 << (BTN_ONOFF_PIN * 2));
    BTN_ONOFF_PORT->PUPDR |=  (0x2 << (BTN_ONOFF_PIN * 2));

    SYSCFG->EXTICR[3] &= ~(0xF << 4);  // EXTI13 ← PC13
    EXTI->IMR  |= (1 << BTN_ONOFF_PIN);
    EXTI->RTSR |= (1 << BTN_ONOFF_PIN);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    // PB3 - RESET
    BTN_RESET_PORT->MODER &= ~(0x3 << (BTN_RESET_PIN * 2));
    BTN_RESET_PORT->PUPDR &= ~(0x3 << (BTN_RESET_PIN * 2));
    BTN_RESET_PORT->PUPDR |=  (0x2 << (BTN_RESET_PIN * 2));

    SYSCFG->EXTICR[0] &= ~(0xF << (4 * BTN_RESET_PIN));
    SYSCFG->EXTICR[0] |=  (0x1 << (4 * BTN_RESET_PIN));
    EXTI->IMR  |= (1 << BTN_RESET_PIN);
    EXTI->RTSR |= (1 << BTN_RESET_PIN);
    NVIC_EnableIRQ(EXTI3_IRQn);

    // PB5 - BUZZ OFF
    BTN_BUZZ_PORT->MODER &= ~(0x3 << (BTN_BUZZ_PIN * 2));
    BTN_BUZZ_PORT->PUPDR &= ~(0x3 << (BTN_BUZZ_PIN * 2));
    BTN_BUZZ_PORT->PUPDR |=  (0x2 << (BTN_BUZZ_PIN * 2));

    SYSCFG->EXTICR[1] &= ~(0xF << (4 * (BTN_BUZZ_PIN - 4)));
    SYSCFG->EXTICR[1] |=  (0x1 << (4 * (BTN_BUZZ_PIN - 4)));
    EXTI->IMR  |= (1 << BTN_BUZZ_PIN);
    EXTI->RTSR |= (1 << BTN_BUZZ_PIN);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}
