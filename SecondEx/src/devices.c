#include "devices.h"
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>



LOG_MODULE_REGISTER(Second_ex,LOG_LEVEL_DBG);
const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
const struct device *uart= DEVICE_DT_GET(DT_NODELABEL(uart0));


uint8_t tx_buf[50] =   {"UART EXAMPLE \n\r"
                             "PASSWORD is 101 to unlock diodes\n\r"};
uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type)
    {
        case UART_RX_RDY:
          

if ((evt->data.rx.len) > 0)
{
    char *received_line = malloc(evt->data.rx.len + 1);  // +1 na znak null-termination
    printk("Received %d bytes\n", evt->data.rx.len);
// Kopiowanie danych do received_line

// Po przetworzeniu danych

    int line_index = 0;

    // Copy data into a separate buffer, excluding newline characters
    for (int i = 0; i < evt->data.rx.len; ++i)
    {
        if (evt->data.rx.buf[evt->data.rx.offset + i] != '\r' && evt->data.rx.buf[evt->data.rx.offset + i] != '\n')
        {
            received_line[line_index++] = evt->data.rx.buf[evt->data.rx.offset + i];
        }
    }

    // Null-terminate the string
    received_line[line_index] = '\0';
int buf_size = sizeof(evt->data.rx.buf) / sizeof(evt->data.rx.buf[0]);

for (int i = 0; i < evt->data.rx.len; ++i)
{
    if (evt->data.rx.offset + i >= buf_size)
    {
        printk("Buffer overflow\n");
        break;
    }

    // Rest of the code...
}
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
