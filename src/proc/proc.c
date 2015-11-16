/*
 * sync.c
 *
 *  Created on: Nov 16, 2015
 *      Author: vty
 */
#include <pthread.h>
#include <stdlib.h>
#include "../../include/cwf.h"

v_cwf_proc_lck v_cwf_proc_lck_new(int len) {
	pthread_mutex_t *m = malloc(sizeof(pthread_mutex_t));
	pthread_cond_t *t = malloc(sizeof(pthread_cond_t));
	pthread_cond_init(t, 0);
	pthread_mutex_init(m, 0);
	v_cwf_proc_lck l = malloc(sizeof(void*) * (2 + len));
	l[0] = m;
	l[1] = t;
	return l;
}
int v_cwf_proc_lck_lock(v_cwf_proc_lck l) {
	return pthread_mutex_lock((pthread_mutex_t*) l[0]);
}
int v_cwf_proc_lck_unlock(v_cwf_proc_lck l) {
	return pthread_mutex_unlock((pthread_mutex_t*) l[0]);
}
int v_cwf_proc_lck_trylock(v_cwf_proc_lck l) {
	return pthread_mutex_trylock((pthread_mutex_t*) l[0]);
}
int v_cwf_proc_lck_wait(v_cwf_proc_lck l) {
	return pthread_cond_wait((pthread_cond_t*) l[1], (pthread_mutex_t*) l[0]);
}
int v_cwf_proc_lck_timed_wait(v_cwf_proc_lck l, int second) {
	struct timespec timeout;
	timeout.tv_sec = time(0) + second;
	timeout.tv_nsec = 0;
	return pthread_cond_timedwait((pthread_cond_t*) l[1],
			(pthread_mutex_t*) l[0], &timeout);
}
int v_cwf_proc_lck_signal(v_cwf_proc_lck l) {
	return pthread_cond_signal((pthread_cond_t*) l[1]);
}
int v_cwf_proc_lck_broadcast(v_cwf_proc_lck l) {
	return pthread_cond_broadcast((pthread_cond_t*) l[1]);
}
int v_cwf_proc_lck_free(v_cwf_proc_lck l) {
	int code = pthread_cond_destroy((pthread_cond_t*) l[1]);
	if (code != 0) {
		return code;
	}
	return pthread_mutex_init((pthread_mutex_t*) l[0], 0);
}
