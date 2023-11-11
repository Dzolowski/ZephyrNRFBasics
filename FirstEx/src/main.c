/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


/*

-Topic of our first excercise was to learn very basics of ZephyrRTOS 
	-Threads
	-GPIO
	-Interrupts
	-Logging
	-DeviceTree basics
	-Synchronization of threads- Semaphores that are currently unused in code, the goal of using them was to "make" thread wait till the other one finish its 
	 work

*/
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

// DEFINES START
#define LED1_NODE DT_NODELABEL(led4)
#define LED2_NODE DT_NODELABEL(led5)
#define BUTTON_NODE DT_NODELABEL(button1)
#define BUTTON_NODE_2 DT_NODELABEL(button2)

#define SLEEP_TIME_MS 1000
#define STACKSIZE 1024
#define PRIORITY 5


#if !DT_NODE_HAS_STATUS(BUTTON_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

//K_SEM_DEFINE(instance_monitor_sem, 0, 1);


LOG_MODULE_REGISTER(Fist_Exercise,LOG_LEVEL_DBG);

K_THREAD_STACK_DEFINE(thread_led1_stack,STACKSIZE);
K_THREAD_STACK_DEFINE(thread_led2_stack,STACKSIZE);

// DEFINES END



//DEFINITIONS OF VARIABLES START
int toggler;

static struct k_thread thread_led1;
static struct k_thread thread_led2;


static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(BUTTON_NODE,gpios,{0});
static struct gpio_callback button_cb_data;

static const struct gpio_dt_spec button_2 = GPIO_DT_SPEC_GET_OR(BUTTON_NODE_2,gpios,{0});
static struct gpio_callback button_cb_data_2;

struct led {
	struct gpio_dt_spec spec;
};
static const struct led led0 ={
	.spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios),
};
static const struct led led1 ={
	.spec = GPIO_DT_SPEC_GET(LED2_NODE, gpios),
};
//DEFINITIONS OF VARIABLES END



// INITIALIZATION OF FUNCTIONS START
void button_pressed(const struct device *dev, struct gpio_callback*cb){

	

 	toggler++;

    LOG_INF("Toggler count %d \n", toggler);
	//k_sem_give(&instance_monitor_sem); 

}

void blink( const struct led *led, int sleep){
	if (!device_is_ready(button.port)) {

		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return;

	}

	

	const struct gpio_dt_spec *spec = &led->spec;

	gpio_pin_configure_dt(spec, GPIO_OUTPUT);

	//k_sem_take(&instance_monitor_sem, K_FOREVER); 

	while(1){
		
		if(toggler%2==0){

			gpio_pin_toggle(spec->port, spec->pin);
			k_msleep(sleep);

		}
		else{

			LOG_WRN("TOGGLER IS NOT EVEN NUMBER\n");
			k_msleep(500);

		}
		//k_sem_give(&instance_monitor_sem);
	}
	
}

void blink0(void){
	blink(&led0, SLEEP_TIME_MS);
}
void blink1(void){
	blink(&led1, SLEEP_TIME_MS);
}
void initialization(void){

	gpio_pin_configure_dt(&button, GPIO_INPUT);

	gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_RISING);

	gpio_init_callback(&button_cb_data, button_pressed,BIT(button.pin));

	gpio_add_callback(button.port, &button_cb_data);

}

// INITIALIZATION OF FUNCTIONS END


// MAIN START
void main(void){
	
	initialization();

	k_thread_create(&thread_led1, thread_led1_stack,
			K_THREAD_STACK_SIZEOF(thread_led1_stack),
			blink0, NULL, NULL, NULL,
			PRIORITY, 0, K_FOREVER);

	k_thread_name_set(&thread_led1, "thread_led1");

	k_thread_create(&thread_led2, thread_led2_stack,
			K_THREAD_STACK_SIZEOF(thread_led2_stack),
			blink1, NULL, NULL, NULL,
			PRIORITY, 0, K_FOREVER);

	k_thread_name_set(&thread_led1, "thread_led1");

	k_thread_start(&thread_led1);
	k_thread_start(&thread_led2);

	
}
//MAIN END