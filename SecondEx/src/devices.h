#ifndef DEVICES_H
#define DEVICES_H
#include <zephyr/drivers/uart.h>

#define SLEEP_TIME_MS   1000
#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100


extern uint8_t tx_buf[100];
extern uint8_t rx_buf[RECEIVE_BUFF_SIZE];
extern const struct device *uart;
extern const struct gpio_dt_spec led0;
extern const struct gpio_dt_spec led1;
extern const struct gpio_dt_spec led2;

void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);
bool checking_devices(const struct device *dev);
void configure_devices(void);
#endif