#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_BSRR    (*(volatile uint32_t*)0x40020018)

#define GPIOAEN       (1u << 0)
#define PIN5          (1u << 5)
int counter = 5;

static void delay(void) {
  for (volatile uint32_t i = 0; i < 500000; ++i) {
  }
}

int main(void) {
  RCC_AHB1ENR |= GPIOAEN;

  GPIOA_MODER &= ~(3u << (5 * 2));
  GPIOA_MODER |=  (1u << (5 * 2));

  while (1) {
    GPIOA_BSRR = PIN5;          // set PA5
    delay();

    GPIOA_BSRR = (PIN5 << 16);  // reset PA5
    delay();
  }
}