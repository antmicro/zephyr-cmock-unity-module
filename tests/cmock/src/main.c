#include "mock_gpio.h"

void test_cmock(void)
{
	const struct device *gpio_dev = device_get_binding("gpio0");

	/* Expect pin 0 to be high */
	gpio_pin_get_raw_ExpectAndReturn(gpio_dev, 0, 1);

	/* Expect pin 1 to be low */
	gpio_pin_get_raw_ExpectAndReturn(gpio_dev, 1, 0);

	/* Test if pin 0 is high */
	TEST_ASSERT_TRUE(gpio_pin_get_raw(gpio_dev, 0));

	/* Test if pin 1 is low */
	TEST_ASSERT_FALSE(gpio_pin_get_raw(gpio_dev, 1));
}

void main(void)
{
	(void)unity_main();
}
