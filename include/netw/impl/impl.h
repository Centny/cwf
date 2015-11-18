/*
 * impl.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef V_CWF_NETW_IMPL_H_
#define V_CWF_NETW_IMPL_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "../netw.h"
#include "../../proc/proc.h"

/**
 *OBDH impl
 */
//
#define V_CWF_NETW_HSET_OBDH 100
int v_cwf_netw_hset_obdh_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd);
v_cwf_netw_hset* v_cwf_netw_hset_obdh_n(char* keys, v_cwf_netw_hset** hs,
		int len);
void v_cwf_netw_hset_obdh_f(v_cwf_netw_hset** hs);

/**
 * Queue impl
 */
#define V_CWF_NETW_HSET_QUEUE 200
int v_cwf_netw_hset_queue_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd);
v_cwf_netw_hset* v_cwf_netw_hset_queue_n(v_cwf_netw_hset** hs, int len);
void v_cwf_netw_hset_queue_f(v_cwf_netw_hset** hs);

/**
 * RC impl
 */
#define V_CWF_NETW_HSET_RC 300

typedef struct v_cwf_netw_hset_rc {
	v_cwf_netw_cmd* pref;
	v_cwf_netw_writer writer;
	//
	//RC lock
	v_cwf_proc_lck lck;
	//
	//the executor lock.
	unsigned int lcks_l;
	v_cwf_proc_lck* lcks;
} v_cwf_netw_hset_rc;
int v_cwf_netw_hset_rc_lck_n(v_cwf_netw_hset_rc* rc);
int v_cwf_netw_hset_rc_lck_f(v_cwf_netw_hset_rc* rc, int idx);
int v_cwf_netw_hset_rc_r(v_cwf_netw_hset* hs, v_cwf_netw_cmd* cmd);
v_cwf_netw_hset* v_cwf_netw_hset_rc_n(v_cwf_netw_cmd* pref,
		v_cwf_netw_writer writer, unsigned int len);
void v_cwf_netw_hset_rc_f(v_cwf_netw_hset** hs);
int v_cwf_netw_hset_rc_exec(v_cwf_netw_hset* hs, void* info, char mark,
		v_cwf_netw_cmd** cmd, int len, v_cwf_netw_cmd** out);

#ifdef __cplusplus
}
#endif
#endif /* V_CWF_NETW_IMPL_H_ */
