#include "button.h"

volatile uint8_t system_powered_on = 0;
volatile uint8_t buzzer_should_off = 0;

void button_init(void) {
    // PC13 - ON/OFF
    SYSCFG->EXTICR[3] &= ~(0xF << 4);  // EXTI13 ← PC13
    SYSCFG->EXTICR[3] |=  (0x2 << 4);
    EXTI->IMR  |= (1 << BTN_ONOFF_PIN);
    EXTI->RTSR |= (1 << BTN_ONOFF_PIN);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    // PB3 - RESET

    SYSCFG->EXTICR[0] &= ~(0xF << (4 * BTN_RESET_PIN));
    SYSCFG->EXTICR[0] |=  (0x1 << (4 * BTN_RESET_PIN));
    EXTI->IMR  |= (1 << BTN_RESET_PIN);
    EXTI->RTSR |= (1 << BTN_RESET_PIN);
    NVIC_EnableIRQ(EXTI3_IRQn);

    // PB5 - BUZZ OFF

    SYSCFG->EXTICR[1] &= ~(0xF << (4 * (BTN_BUZZ_PIN - 4)));
    SYSCFG->EXTICR[1] |=  (0x1 << (4 * (BTN_BUZZ_PIN - 4)));
    EXTI->IMR  |= (1 << BTN_BUZZ_PIN);
    EXTI->RTSR |= (1 << BTN_BUZZ_PIN);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}
