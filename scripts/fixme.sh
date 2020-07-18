#!/bin/sh

startdir="${1:?no dirname provided}"
rootdir=/home/john/programming/C++/git/dewpsi

grep -rn $rootdir/$startdir/ -e '// FIXME.*$'
