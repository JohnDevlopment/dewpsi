#!/bin/bash

shopt -s expand_aliases extglob
alias grep='grep --color=auto -no'

pattern="'// ${1:?provide a word}.*\$'"

rootdir=/home/john/programming/C++/git/dewpsi

error() {
    echo "$@" >&2
}

select startdir in Dewpsi "Dewpsi/vendor/getopt" Sandbox
do
    case "$startdir" in
        Dewpsi)
            prefix="$rootdir/$startdir"
            eval grep -e "$pattern" "$prefix/src/platform/sdl/Dewpsi_*.h"
            for subdir in bits debug events ImGui matrices os shapes; do
                eval grep -e "$pattern" "$prefix/src/$subdir/Dewpsi_*.h"
            done
            break
            ;;

        */getopt)
            prefix="$rootdir/$startdir"
            eval grep -r -e "$pattern" "$prefix/*"
            break
            ;;

        Sandbox)
            prefix="$rootdir/$startdir"
            eval grep -e "$pattern" "$prefix/src/*.[hc]"
            break
            ;;

        *)
            error "Invalid option"
            exit 1
            ;;
    esac
done
