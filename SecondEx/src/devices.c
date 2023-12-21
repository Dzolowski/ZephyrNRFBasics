#include "devices.h"
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <stdlib.h> 


LOG_MODULE_REGISTER(Second_ex,LOG_LEVEL_DBG);
const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
const struct device *uart= DEVICE_DT_GET(DT_NODELABEL(uart0));
struct uart_data_t {
	void *fifo_reserved;
	uint8_t data[128];
	uint16_t len;
};
K_FIFO_DEFINE(uart_fifo);
uint8_t tx_buf[100] =   {"UART EXAMPLE \n\r"
                             "PASSWORD is admin to unlock diodes\n\r"};
uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type)
    {
        case UART_RX_RDY:
          

if ((evt->data.rx.len) > 0)
{
    char *received_line = malloc(evt->data.rx.len );  // +1 na znak null-termination
    printk("Received %d bytes\n", evt->data.rx.len);
// Kopiowanie danych do received_line

// Po przetworzeniu danych

    int line_index = 0;

    static bool mResponsePacketReadyToProcess = false;

    // Copy data into a separate buffer, excluding newline characters
    for (int i = 0; i < evt->data.rx.len; ++i)
    {
        if (evt->data.rx.buf[evt->data.rx.offset + i] != '\r' && evt->data.rx.buf[evt->data.rx.offset + i] != '\n')
        {
            received_line[line_index++] = evt->data.rx.buf[evt->data.rx.offset + i];
        }
        // Check end-of-packet delimiter, assume lf =='/n' for now
        if (evt->data.rx.buf[evt->data.rx.offset + i] == '\n')
        {
            mResponsePacketReadyToProcess = true;
        }
    }

 // Only process the buffer when the entire string has been received
 if (mResponsePacketReadyToProcess)
 {
    mResponsePacketReadyToProcess = false;
    // Null-terminate the string
    received_line[line_index] = '\0';
    printk("Received line: %s\n", received_line);
    // Process the received line as needed
 }
    // Null-terminate the string

    printk("Received line: %s\n", received_line);

    // Process the received line as needed
    // (e.g., check if it equals "admin" and toggle LEDs)
    if (strcmp(received_line, "admin") == 0)
    {
        gpio_pin_toggle_dt(&led0);
        gpio_pin_toggle_dt(&led1);
        gpio_pin_toggle_dt(&led2);
        k_msleep(SLEEP_TIME_MS);
        
    }
    else
    {
        printk("Wrong input\n");
    }
    line_index = 0;
    memset(evt->data.rx.buf, 0, sizeof(evt->data.rx.buf));
    free(received_line);
}

            break;
        case UART_RX_DISABLED:
            uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
            break;
        default:
            break;
    }
}

bool checkingdevices(const struct device *dev){
    if (!device_is_ready(dev)){
		LOG_ERR("Device - %s is not readyr\n",dev->name);
		return 0;
	}
	else return 1;	

}
