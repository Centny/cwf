/*
 * cwf.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef CWF_H_
#define CWF_H_

#include "log/log.h"
#include "netw/impl/impl.h"
#include "netw/netw.h"

char* v_cwf_alloc_c(unsigned int l);
void v_cwf_free_c(char** v);

#endif /* CWF_H_ */
