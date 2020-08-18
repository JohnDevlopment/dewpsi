#!/bin/bash

perror() {
    echo -e "$@" >&2
}

doError() {
    perror "$@"
    exit 1
}

rootdir="$PWD"

if [ -n "$rootdir" ]; then
    doError "Enviroment variable \$PWD not set"
fi

let error=1

if [ "${rootdir##*/}" = "dewpsi" ]; then
    let error=0
fi

declare -a dirs

dirs=("" "Dewpsi/vendor/ImGui" "Dewpsi/vendor/Glad" "Dewpsi/vendor/spdlog" \
"Dewpsi/vendor/getopt" "Dewpsi/vendor/glm")

for temp in "${dirs[@]}"; do
    echo $temp
    rm "$temp/Makefile"
done
