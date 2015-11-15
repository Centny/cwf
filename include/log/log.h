/*
 * log.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef LOC_H_
#define LOC_H_
#include <stdio.h>

#define V_CWF_LOG_D 10
#define V_CWF_LOG_I 20
#define V_CWF_LOG_W 30
#define V_CWF_LOG_E 40

//
extern int v_cwf_log_lvl_;
extern void (*v_cwf_log_f_)(const char* file, int line, int lvl,
		const char* fmt, va_list args);
//
void v_cwf_log_print(const char* file, int line, int lvl, const char* fmt,
		va_list args);
//
void v_cwf_log_(const char* file, int line, int lvl, const char* fmt, ...);

#define v_cwf_log_d(fmt,args...) v_cwf_log_(__FILE__,__LINE__,V_CWF_LOG_D,fmt,args)
#define v_cwf_log_i(fmt,args...) v_cwf_log_(__FILE__,__LINE__,V_CWF_LOG_I,fmt,args)
#define v_cwf_log_w(fmt,args...) v_cwf_log_(__FILE__,__LINE__,V_CWF_LOG_W,fmt,args)
#define v_cwf_log_e(fmt,args...) v_cwf_log_(__FILE__,__LINE__,V_CWF_LOG_E,fmt,args)

#endif /* LOC_H_ */
