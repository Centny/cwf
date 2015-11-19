AUTOMAKE_OPTIONS = foreign subdir-objects
lib_LTLIBRARIES=libcwf.la
libcwf_la_SOURCES=log/log.c netw/netw.c netw/impl/impl.c proc/proc.c im/im.c cwf.c
nobase_include_HEADERS=../include/cwf.h ../include/log/log.h ../include/netw/netw.h ../include/netw/impl/impl.h ../include/proc/proc.h ../include/im/im.h
libcwf_la_CFLAGS=${LIBCWF_CFLAGS} -std=c99
libcwf_la_LDFLAGS=${LIBCWF_CFLAGS}