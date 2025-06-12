#ifndef LED_H
#define LED_H

#define LED_POWER_PORT    GPIOB
#define LED_POWER_PIN     0

#define LED_OK_PORT       GPIOB
#define LED_OK_PIN        1

#define LED_RAW_PORT      GPIOC
#define LED_RAW_PIN       2

#define LED_IGNORE_PORT   GPIOC
#define LED_IGNORE_PIN    3

typedef struct {
    GPIO_TypeDef* port;
    uint8_t       pin;
    uint32_t      start_time;
    uint32_t      duration;
    uint8_t       active;
} Led;

void led_on(GPIO_TypeDef* port, uint8_t pin);
void led_off(GPIO_TypeDef* port, uint8_t pin);
void led_toggle(GPIO_TypeDef* port, uint8_t pin);
void led_blink(GPIO_TypeDef* port, uint8_t pin, int delay_ms);
void led_check(void);
#endif
