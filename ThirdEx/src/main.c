/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
// This exercise is based on BME280 sample communicated by I2C, the topic of this sample was LED thermometer
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include "defines.h"
#define LED1_NODE DT_NODELABEL(led4)
#define LED2_NODE DT_NODELABEL(led5)
#define LED3_NODE DT_NODELABEL(led6)
#define LED4_NODE DT_NODELABEL(led7)
#define LED5_NODE DT_NODELABEL(led8)



#define SLEEP_TIME_MS 1000
#define STACKSIZE 1024
#define PRIORITY 5


LOG_MODULE_REGISTER(LEDTHERMOMETER,LOG_LEVEL_DBG);


K_THREAD_STACK_DEFINE(thread_led_stack,STACKSIZE);
K_THREAD_STACK_DEFINE(thread_sensor_stack,STACKSIZE);


static struct k_thread thread_led;
static struct k_thread thread_sensor;


typedef struct led {
	struct gpio_dt_spec spec;
}led_t;


static const led_t leds[] = {
        {.spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios)},
        {.spec = GPIO_DT_SPEC_GET(LED2_NODE, gpios)},
        {.spec = GPIO_DT_SPEC_GET(LED3_NODE, gpios)},
        {.spec = GPIO_DT_SPEC_GET(LED4_NODE, gpios)},
        {.spec = GPIO_DT_SPEC_GET(LED5_NODE, gpios)}
    };


struct sensor_value temp, press, humidity;


//BME280 init
static const struct device *get_bme280_device(void)
{
	const struct device *const dev = DEVICE_DT_GET_ANY(bosch_bme280);

	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		printk("\nError: no device found.\n");
		return NULL;
	}

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return NULL;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	return dev;
}
//BME280 init
void sensorbme280(){
const struct device *dev = get_bme280_device();

	if (dev == NULL) {
		return 0;
	}

	while (1) {
		

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &humidity);

		printk("temp: %d.%06d; press: %d.%06d; humidity: %d.%06d\n",
		      temp.val1, temp.val2, press.val1, press.val2,
		      humidity.val1, humidity.val2);
                k_msleep(100);
		
	}
	return 0;
}

void led_toggler(const struct led *led_array,size_t array_size){
        for(int i =0; i<array_size;i++){
        const struct gpio_dt_spec *spec=&led_array[i].spec;
        gpio_pin_configure_dt(spec, GPIO_OUTPUT);
        if(temp.val1 >= 25+i){
        gpio_pin_set(spec->port,spec->pin,1);
        k_msleep(100);
        }
        else{
           gpio_pin_set(spec->port,spec->pin,0);     
        }
        k_msleep(100);
        }
}


// Funfact when coding I shorted my bme280 with spoon by the mistake, holy fuck XD 


void led_toggler_exec() {
    while (1) {
        led_toggler(leds, ARRAY_SIZE(leds));
    }
}

int main(void)
{
	k_thread_create(&thread_led, thread_led_stack,
			K_THREAD_STACK_SIZEOF(thread_led_stack),
			led_toggler_exec, NULL, NULL, NULL,
			PRIORITY, 0, K_FOREVER);

	k_thread_create(&thread_sensor, thread_sensor_stack,
			K_THREAD_STACK_SIZEOF(thread_sensor_stack),
			sensorbme280, NULL, NULL, NULL,
			PRIORITY+1, 0, K_FOREVER);

	k_thread_start(&thread_led);
	k_thread_start(&thread_sensor);
}
