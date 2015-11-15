/*
 * impl.h
 *
 *  Created on: Nov 15, 2015
 *      Author: vty
 */

#ifndef IMPL_H_
#define IMPL_H_

#include "../netw.h"

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

#endif /* IMPL_H_ */
