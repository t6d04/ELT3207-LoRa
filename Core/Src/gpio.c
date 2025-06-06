#include "stm32f4xx.h"

void gpio_init_all(void) {
    // Enable GPIO clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
                    RCC_AHB1ENR_GPIOBEN |
                    RCC_AHB1ENR_GPIOCEN;

    // ------------------------
    // BUTTONS: PC13, PB3, PB5 (Input + Pull-down)
    // ------------------------
    GPIOC->MODER &= ~(0x3 << (13 * 2));  // Input
    GPIOC->PUPDR &= ~(0x3 << (13 * 2));
    GPIOC->PUPDR |=  (0x2 << (13 * 2));  // Pull-down

    GPIOB->MODER &= ~(0x3 << (3 * 2));   // PB3
    GPIOB->PUPDR &= ~(0x3 << (3 * 2));
    GPIOB->PUPDR |=  (0x2 << (3 * 2));

    GPIOB->MODER &= ~(0x3 << (5 * 2));   // PB5
    GPIOB->PUPDR &= ~(0x3 << (5 * 2));
    GPIOB->PUPDR |=  (0x2 << (5 * 2));

    // ------------------------
    // LEDS: PB0, PB1 (Output)
    // ------------------------
    GPIOB->MODER &= ~(0x3 << (0 * 2));
    GPIOB->MODER |=  (0x1 << (0 * 2));  // Output

    GPIOB->MODER &= ~(0x3 << (1 * 2));
    GPIOB->MODER |=  (0x1 << (1 * 2));  // Output

    GPIOB->OTYPER &= ~((1 << 0) | (1 << 1)); // Push-pull
    GPIOB->OSPEEDR |= (0x3 << (0 * 2)) | (0x3 << (1 * 2)); // High speed

    // ------------------------
    // BUZZER: PB10 (Output)
    // ------------------------
    GPIOB->MODER &= ~(0x3 << (10 * 2));
    GPIOB->MODER |=  (0x1 << (10 * 2));  // Output
    GPIOB->OTYPER &= ~(1 << 10);
    GPIOB->OSPEEDR |= (0x3 << (10 * 2));

    // ------------------------
    // LoRa SPI: PA5 (SCK), PA6 (MISO), PA7 (MOSI) – AF5
    // ------------------------
    GPIOA->MODER &= ~((0x3 << (5 * 2)) | (0x3 << (6 * 2)) | (0x3 << (7 * 2)));
    GPIOA->MODER |=  ((0x2 << (5 * 2)) | (0x2 << (6 * 2)) | (0x2 << (7 * 2))); // AF mode
    GPIOA->AFR[0] &= ~((0xF << (5 * 4)) | (0xF << (6 * 4)) | (0xF << (7 * 4)));
    GPIOA->AFR[0] |=  ((0x5 << (5 * 4)) | (0x5 << (6 * 4)) | (0x5 << (7 * 4))); // AF5 = SPI1

    // ------------------------
    // LoRa NSS: PA4 (Output)
    // ------------------------
    GPIOA->MODER &= ~(0x3 << (4 * 2));
    GPIOA->MODER |=  (0x1 << (4 * 2)); // Output
    GPIOA->OTYPER &= ~(1 << 4);
    GPIOA->OSPEEDR |= (0x3 << (4 * 2));

    // ------------------------
    // LoRa DIO0: PA0 (Input + Interrupt)
    // ------------------------
    GPIOA->MODER &= ~(0x3 << (0 * 2));
    GPIOA->PUPDR &= ~(0x3 << (0 * 2));
    GPIOA->PUPDR |=  (0x2 << (0 * 2)); // Pull-down

    // SYSCFG for EXTI0
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] &= ~(0xF << 0); // PA0 = EXTI0
    EXTI->IMR  |= (1 << 0);  // Unmask line 0
    EXTI->RTSR |= (1 << 0);  // Rising edge

    NVIC_EnableIRQ(EXTI0_IRQn);

    // ------------------------
    // LCD I2C: PB6 (SCL), PB7 (SDA) – AF4
    // ------------------------
    GPIOB->MODER &= ~((0x3 << (6 * 2)) | (0x3 << (7 * 2)));
    GPIOB->MODER |=  ((0x2 << (6 * 2)) | (0x2 << (7 * 2))); // AF mode
    GPIOB->AFR[0] &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
    GPIOB->AFR[0] |=  ((0x4 << (6 * 4)) | (0x4 << (7 * 4))); // AF4 = I2C1
    GPIOB->OTYPER |=  (1 << 6) | (1 << 7); // Open-drain
    GPIOB->PUPDR &= ~((0x3 << (6 * 2)) | (0x3 << (7 * 2)));
    GPIOB->PUPDR |=  (0x1 << (6 * 2)) | (0x1 << (7 * 2)); // Pull-up
}
