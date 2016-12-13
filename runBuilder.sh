#!/bin/bash

BASEDIR=`dirname $0`
SRCDIR=$BASEDIR/src
BUILDDIR=$BASEDIR/build

while true
do
	find $SRCDIR | entr -cd make -C $BUILDDIR
done
