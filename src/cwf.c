/*
 ============================================================================
 Name        : cwf.c
 Author      : Centny
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

char* v_cwf_alloc_c(unsigned int l) {
	return malloc(sizeof(char) * l);
}
void v_cwf_free_c(char** v) {
	free(*v);
	*v = 0;
}
