#!/bin/bash

# abort on errors
set -e

# date
date=`date +%Y%m%d`

# version
version=`cat VERSION`

# avoid mac os copying ._* files
export COPY_EXTENDED_ATTRIBUTES_DISABLE=true
export COPYFILE_DISABLE=true

# clean directory tree
(cd src; make clean)
(cd lib; make clean)
rm -f eagle/*.b#? eagle/*.s#?
rm -f eagle/trackuino-avr.{GBL,GBO,GBS,GTL,GTO,GTS,TXT,dri,gpi,GTP}

# build arduino IDE package
dst=tmp/trackuino-ide-$version
mkdir -p $dst/trackuino
cp src/*.{cpp,h} $dst/trackuino
mv $dst/trackuino/trackuino.cpp $dst/trackuino/trackuino.pde
mkdir $dst/eagle
cp eagle/* $dst/eagle
cp LICENSE $dst/LICENSE.txt
cp README.ide-version  $dst/README.txt
cp WHATSNEW $dst/WHATSNEW.txt
for i in $dst/*.txt
do
  sed -i '' -e s/$// $i
done
test -f trackuino-ide-$version.zip && rm trackuino-ide-$version.zip
(cd tmp; zip -9r ../trackuino-ide-$version.zip *) 
rm -rf tmp

# build makefile-based package for use with gcc-avr
dst=tmp/trackuino-gcc-$version
mkdir -p $dst
cp -r WHATSNEW LICENSE include src Makefile.inc Makefile.user lib eagle $dst
cp README.gcc-version $dst/README
test -f trackuino-gcc-$version.tgz && rm trackuino-gcc-$version.tgz
(cd tmp; tar -cvzf ../trackuino-gcc-$version.tgz *)
rm -rf tmp
