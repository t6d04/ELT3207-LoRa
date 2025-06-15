#include "main.h"
#include "stm32f4xx_it.h"
#include "button.h"
#include "lora.h"
#include "gpio.h"
#include "system.h"
#include "buzzer.h"
#include "uart.h"
#include "led.h"

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {
        EXTI->PR |= (1 << 0);
        uart2_send_string("DIO0 Interrupt triggered\r\n");
        led_toggle(LED_OK_PORT, LED_OK_PIN);
        lora_handle_packet_interrupt();
    }
}

void EXTI3_IRQHandler(void) {
    if (EXTI->PR & (1 << 3)) {
        EXTI->PR |= (1 << 3);
        NVIC_SystemReset();
    }
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << 5)) {
        EXTI->PR |= (1 << 5);
        buzzer_off();
    }
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        EXTI->PR |= (1 << 13);
        system_powered_on ^= 1;
        if (system_powered_on)
            system_startup();
        else
            system_shutdown();
    }
}

void NMI_Handler(void) {
    while (1) {}
}

void HardFault_Handler(void) {
    uart2_send_string("HardFault occurred!\r\n");
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (0x1 << (13 * 2));
    GPIOC->OTYPER &= ~(1 << 13);
    while (1) {
        GPIOC->ODR ^= (1 << 13);
        for (volatile uint32_t i = 0; i < 1000000; i++);
    }
}

void MemManage_Handler(void) {
    while (1) {}
}

void BusFault_Handler(void) {
    while (1) {}
}

void UsageFault_Handler(void) {
    while (1) {}
}

void SVC_Handler(void) {}

void DebugMon_Handler(void) {}

void PendSV_Handler(void) {}

#include "timer.h"
void SysTick_Handler(void) {
    timer_systick_callback();
}
