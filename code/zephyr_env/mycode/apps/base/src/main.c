#include "ir.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>


/* size of stack area used by each thread */
#define STACKSIZE 1024
/* scheduling priority used by each thread */
#define PRIORITY 7
/* thread_b is a static thread spawned immediately */
#define MSG_SIZE 32
/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(data_msgq, MSG_SIZE, 20, 4);

/*
 * Thread handling bluetooth connections
 */
void blue_loop(void) {
  // Initialise bluetooth
}

K_THREAD_DEFINE(thread_blue, STACKSIZE, blue_loop, NULL, NULL, NULL, PRIORITY,
                0, 0);
extern const k_tid_t thread_blue;

int main(void) { 
  printf("hello world\n"); 
  printf("1\n"); 
  gpio_init();
  printf("2\n"); 
  test();
  printf("done\n"); 
}
