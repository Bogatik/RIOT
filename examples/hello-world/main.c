/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "periph/gpio.h"
#include "rtctimers-millis.h"

#include "thread.h"
#include "shell_commands.h"
#include "shell.h"

#include "periph/pm.h"
static kernel_pid_t process_pid ;
static msg_t process_msg ; 

#define MY_PROCESS_STACK_SIZE (1024)
#define ENABLE_DEBUG (1)
#include "debug.h"


uint32_t milliseconds_last_press = 0 ;

static void *process_thread(void *arg) {
(void)arg;
msg_t message ;
gpio_set(GPIO_PIN(PORT_B, 0));
while (1)
 {
	msg_receive(&message) ;
	gpio_toggle(GPIO_PIN(PORT_B, 0));
	if (gpio_read(GPIO_PIN(PORT_B, 0)) == 1)
	{
	DEBUG("Stasik privet %d\n", gpio_read(GPIO_PIN(PORT_B, 0)));
	
	}else 
	
	DEBUG("Stasik poka %d\n", gpio_read(GPIO_PIN(PORT_B, 0)));
	}
	return NULL;
}


static void btn_led_toggle(void* arg) {
(void)arg;

if ((rtctimers_millis_now() - milliseconds_last_press) > 200)
{
msg_send(&process_msg, process_pid);
milliseconds_last_press = rtctimers_millis_now();

}

}


int main(void)
{
	pm_init();
	pm_prevent_sleep = 1;

	rtctimers_millis_init();
    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

	gpio_init (GPIO_PIN(PORT_B, 0), GPIO_OUT) ;
	//gpio_set (GPIO_PIN(PORT_B, 0)) ;
	gpio_init_int(GPIO_PIN(PORT_B, 1), GPIO_IN_PU, GPIO_FALLING, btn_led_toggle, NULL);
	
	char stack[MY_PROCESS_STACK_SIZE] ; 
	process_pid = thread_create(stack, MY_PROCESS_STACK_SIZE,
				    THREAD_PRIORITY_MAIN -1, THREAD_CREATE_STACKTEST,
				    process_thread, NULL, "our process") ;

    return 0;
}
