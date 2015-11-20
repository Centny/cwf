AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS=testcwf
testcwf_SOURCES=log/log_test.c netw/impl/impl_test.c netw/netw_test.c proc/proc_test.c im/im_test.c cwf_test.c
testcwf_CFLAGS=${LIBCWF_CFLAGS} -std=c99 -pthread
testcwf_LDFLAGS=-lcwf -L../src/.libs ${LIBCWF_LDLAGS} -pthread