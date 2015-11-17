/*
 * proc_test.c
 *
 *  Created on: Nov 17, 2015
 *      Author: vty
 */
#include "../../include/cwf.h"

void v_cwf_proc_lck_test() {
	v_cwf_proc_lck lck = v_cwf_proc_lck_new(1);
	v_cwf_proc_lck_lock(lck);
	v_cwf_proc_lck_unlock(lck);
	v_cwf_proc_lck_free(lck);
}

void proc_test() {
//	while (1) {
	v_cwf_proc_lck_test();
//	}
	printf("proc_test done...\n");
	//
}
