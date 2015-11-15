/*
 * log_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include <stdio.h>
#include "../../include/cwf.h"

void log_test() {
	v_cwf_log_(__FILE__, __LINE__, 4, "the log->%s", "u");
	v_cwf_log_d("the log->%s", "debug");
	v_cwf_log_i("the log->%s", "info");
	v_cwf_log_w("the log->%s", "warn");
	v_cwf_log_e("the log->%s", "error");
	v_cwf_log_(__FILE__, __LINE__, 1000, "the log->%s", "u");
	v_cwf_log_f_ = 0;
	v_cwf_log_e("the log->%s", "error");
	v_cwf_log_f_ = v_cwf_log_print;
	printf("log_test done...\n");
}
