#!/bin/sh

# abort on errors
set -e

# date
date=`date +%Y%m%d`

# avoid mac os copying ._* files
export COPY_EXTENDED_ATTRIBUTES_DISABLE=true
export COPYFILE_DISABLE=true

# clean directory tree
(cd src; make clean)
(cd lib; make clean)

# build arduino IDE package
mkdir tmp
for i in src/*.h; do
  lib=`basename $i .h`
  if [ "$lib" != trackuino ]; then
    mkdir -p tmp/libraries/$lib
    test -f src/$lib.h   && cp src/$lib.h   tmp/libraries/$lib
    test -f src/$lib.cpp && cp src/$lib.cpp tmp/libraries/$lib
    test -f src/$lib.c   && cp src/$lib.c   tmp/libraries/$lib
  fi
done
mkdir tmp/trackuino
cp src/trackuino.cpp tmp/trackuino/trackuino.pde
mkdir tmp/trackuino/eagle
cp eagle/* tmp/trackuino/eagle
cp LICENSE tmp/trackuino/LICENSE.txt
cp README  tmp/trackuino/README.txt
test -f trackuino-ide-$date.zip && rm trackuino-ide-$date.zip
(cd tmp; zip -9r ../trackuino-ide-$date.zip *) 
rm -rf tmp

# build makefile-based package for use with gcc-avr
tar -C .. -cvz --exclude=.\* --exclude trackuino-gcc-\*tgz --exclude trackuino-ide-\*zip \
  -f trackuino-gcc-$date.tgz trackuino


