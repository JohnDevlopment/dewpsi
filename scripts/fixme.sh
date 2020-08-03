#!/bin/sh

startdir="${1:?no dirname provided}"
rootdir=/home/john/programming/C++/git/dewpsi

grep -rno $rootdir/$startdir/ -e '// FIXME.*$'
