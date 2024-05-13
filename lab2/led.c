#include "platform.h"
#include "queue.h"
#include <gpio.h>

// Should any1 ask why this queue, well, seemed like an easy change
#define LEDPIN P_LED1

Queue led_queue;

// Toggle led and print seems like a repeatable pattern
void ledInit(void) {
  queueInit(&led_queue, QUEUE_SIZE);
  gpio_set_mode(LEDPIN, Output);
}

void ledPlaying(void) {
  timer_init(period_us / NUM_STEPS);
  timer_enable();
  int ledValue = 0;

  while (1) // Should make it exit somehow - it is not really viable otherwise
  {
    if (queue_dequeue(&led_queue, &ledValue))
      uart_print("Led toggled due to timer\r\n")
  }
}

void timer_callback_isr(void) {
  // Avoiding to overpopulate the isr
  gpio_toggle(LEDPIN);
  queue_enqueue(&led_queue, gpio_get(LEDPIN));
}
