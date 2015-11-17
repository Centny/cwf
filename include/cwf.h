/*
 * cwf.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef V_CWF_H_
#define V_CWF_H_

#include "log/log.h"
#include "netw/impl/impl.h"
#include "netw/netw.h"
#include "proc/proc.h"
//
#ifdef __cplusplus
#include "im/im.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

char* v_cwf_alloc_c(unsigned int l);
void v_cwf_free_c(char** v);

#ifdef __cplusplus
}
#endif
#endif /* V_CWF_H_ */
