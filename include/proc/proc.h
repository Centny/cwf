/*
 * proc.h
 *
 *  Created on: Nov 16, 2015
 *      Author: vty
 */

#ifndef V_CWF_PROC_H_
#define V_CWF_PROC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void** v_cwf_proc_lck;

v_cwf_proc_lck v_cwf_proc_lck_new(int len);
int v_cwf_proc_lck_lock(v_cwf_proc_lck l);
int v_cwf_proc_lck_unlock(v_cwf_proc_lck l);
int v_cwf_proc_lck_trylock(v_cwf_proc_lck l);
int v_cwf_proc_lck_wait(v_cwf_proc_lck l);
int v_cwf_proc_lck_timed_wait(v_cwf_proc_lck l, int second);
int v_cwf_proc_lck_signal(v_cwf_proc_lck l);
int v_cwf_proc_lck_broadcast(v_cwf_proc_lck l);
int v_cwf_proc_lck_free(v_cwf_proc_lck l);

#ifdef __cplusplus
}
#endif
#endif /* V_CWF_PROC_H_ */
