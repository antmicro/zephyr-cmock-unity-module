/*
 * Copyright (c) 2023 Antmicro <www.antmicro.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(unity, LOG_LEVEL_INF);

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
		LOG_INF("PROJECT EXECUTION SUCCESSFUL");
	} else {
		LOG_INF("PROJECT EXECUTION FAILED");
	}


#ifdef CONFIG_ARCH_POSIX
	posix_exit(exit_code);
#endif

	return exit_code;
}
