/*
 * Copyright (c) 2023 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#ifdef CONFIG_ARCH_POSIX
#include "posix_board_if.h"
#endif

__weak int __unused suite_teardown(int num_failures)
{
	int exit_code = 0;

	if (num_failures > 0) {
		exit_code = 1;
	}

	if (exit_code == 0) {
		printk("unity: PROJECT EXECUTION SUCCESSFUL\n");
	} else {
		printk("unity: PROJECT EXECUTION FAILED\n");
	}


#ifdef CONFIG_ARCH_POSIX
	posix_exit(exit_code);
#endif

	return exit_code;
}
