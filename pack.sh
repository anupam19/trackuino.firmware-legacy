#!/bin/sh

# avoid mac os copying ._* files
export COPY_EXTENDED_ATTRIBUTES_DISABLE=true
export COPYFILE_DISABLE=true
#cd ..
(cd src; make clean)
(cd lib; make clean)
tar -C .. -cvz --exclude=.* --exclude trackuino*tgz -f trackuino`date +%Y%m%d`.tgz trackuino
