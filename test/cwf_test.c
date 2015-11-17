/*
 * cwf_test.c
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */
#include <stdlib.h>
#include <stdio.h>

void log_test();
void netw_test();
void proc_test();

void testAll() {
	log_test();
	netw_test();
	proc_test();
}

int main() {
	log_test();
	netw_test();
	proc_test();
	printf("done...\n");
	return 0;
}

//void log_test(){
//
//}
