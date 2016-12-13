#!/bin/bash

BASEDIR=`dirname $0`
SRCDIR=$BASEDIR/src
BUILDDIR=$BASEDIR/build

while true
do
	echo "Waiting for a change ..."
	find $SRCDIR | entr -cdp make -C $BUILDDIR
	echo
done
