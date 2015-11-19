#!/bin/bash
find . -name \*.gcno -type f -delete
find . -name \*.gcda -type f -delete
rm -rf reports
if [ "$1" = "" ];then
 ./autogen.sh
 export LIBCWF_CFLAGS="-fprofile-arcs -ftest-coverage"
 export LIBCWF_LDFLAGS="-fprofile-arcs -ftest-coverage"
 ./configure --prefix=`pwd`/build
fi
#
make clean
make
make install
export DYLD_LIBRARY_PATH=`pwd`/build/lib
export LD_LIBRARY_PATH=`pwd`/build/lib
cd build/bin
./testcwf
cd ../../
mkdir reports
mkdir reports/html
mkdir reports/xml
cd src
gcovr --html --html-details -o ../reports/html/coverage.html -r . -v .
gcovr --xml -o reports/xml/coverage.xml -r . -v .