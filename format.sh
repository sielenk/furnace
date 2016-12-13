#!/bin/bash

BASEDIR=`dirname $0`
SRCDIR=$BASEDIR/src

find $SRCDIR -name "*.[chi]pp" | xargs clang-format -style=file -i
