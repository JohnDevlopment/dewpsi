#!/bin/bash

shopt -s expand_aliases

alias sedr="sed -r 's/.*/    &/g'"

argv0="${0##*/}"

# print usage
if [ "$1" = "" ]; then
    echo "Usage:
    $argv0 type parameter [options]
" >&2
    echo 'ARGUMENTS

type
===='
    echo 'PARAMETER,DESCRIPTION
type,type of file to create
parameter,type-specific parameter' | column -ts, | sedr >&2
    echo -e "\nparameter per type
=================="
    echo 'TYPE,PARAMETER
header,file to write (default extension .h)
source,file to write (default extension .cc)' | column -ts, | sedr >&2
    
    echo >&2
    echo "For the 'source' and 'header' types, an optional list of header files can be specified to be included.
Pass '-s' and subsequent arguments are taken as system headers (default). Pass '-l' to take them as local headers instead." >&2
    echo "For source files, pass '--main' and main() will be included as well."
    
    exit 1
fi

# basic options
type="$1"

case $type in
    header)
        filename="${2:?no file specified}"
        if [ "${filename#*.}" = "" ]; then
            filename="$filename.h"
        fi
        
        # open and close braces
        openbrace='<'
        closebrace='>'
        
        # open file
        exec 3>"$filename"
        
        # create header guard name (e.g., temp.sh => TEMP_H)
        headertag="${filename##*/}"
        headertag="${headertag^^}"
        headertag="${headertag/./_}"
        
        printf "#ifndef $headertag\n#define $headertag\n" >&3
        
        shift 2
        
        # each subsequent argument is an #include
        for header in "$@"; do
            if [ "$header" = '-s' ]; then
                openbrace='<'
                closebrace='>'
                continue
            elif [ "$header" = '-l' ]; then
                openbrace='"'
                closebrace='"'
                continue
            fi
            
            echo "#include ${openbrace}$header${closebrace}" >&3
        done
        
        printf "\n\n#endif /* $headertag */" >&3
        exec 3>&-
        
        echo "Created $filename"
        ;;
    
    source)
        filename="${2:?no file specified}"
        shift 2
        
        # no extension
        if [ "${filename#*.}" = "" ]; then
            filename="$filename.cc"
        fi
        
        # open and close braces
        openbrace='<'
        closebrace='>'
        
        # open file
        exec 3>"$filename"
        
        # write to file
        for header in "$@"; do
            if [ "$header" = '-s' ]; then
                openbrace='<'
                closebrace='>'
                continue
            elif [ "$header" = '-l' ]; then
                openbrace='"'
                closebrace='"'
                continue
            elif [ "$header" = '--main' ]; then
                includemain=1
                continue
            fi
            
            echo "#include ${openbrace}$header${closebrace}" >&3
        done
        
        # write main function
        if [ -v includemain ]; then
            echo -e "\nint main(int argc, const char* argv[])\n{\n}" >&3
        fi
        
        # close file
        exec 3>&-
        
        echo "Created $filename"
        ;;
    
    *)
        echo "Unknown type '$type'" >&2
        exit 1
        ;;
esac
