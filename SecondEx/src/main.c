#include "devices.h"
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

int main(void)
{
    int ret;
    printk("working\n");
    if (!checkingdevices(uart))
    {
        return 1;
    }

    if (!checkingdevices(led0.port))
    {
        return 1;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }

    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
    if (ret < 0)
    {
        return 1;
    }

    ret = uart_callback_set(uart, uart_cb, NULL);
    if (ret)
    {
        return 1;
    }

    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
    if (ret)
    {
        return 1;
    }

    ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
    if (ret)
    {
        return 1;
    }

    while (1)
    {
        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}

