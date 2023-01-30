/*
 * Copyright (c) 2023 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <unity.h>

void test_unity(void) {
	TEST_ASSERT_TRUE(2 == 2);
	TEST_ASSERT_FALSE(2 != 2);
	TEST_ASSERT_EQUAL(2, 1 + 1);
	TEST_ASSERT_INT_WITHIN(1, 4, 3);
	TEST_ASSERT_GREATER_THAN(2, 4);
}

void main(void)
{
	(void)unity_main();
}
