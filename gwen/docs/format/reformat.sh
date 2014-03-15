#!/bin/sh

UNCRUST=uncrustify

if [ ! -n "$1" ]; then
    echo "Syntax is: recurse.sh dirname filesuffix"
    echo "Syntax is: recurse.sh filename"
    echo "Example: recurse.sh temp cpp"
    exit 1
fi

function reformat {
    echo "Indenting file $1"
    $UNCRUST -f "$1" -l cpp -c "gwen_uncrustify.cfg" -o indentoutput.tmp
    mv indentoutput.tmp "$1"
}

function reformatlist {
    echo "Indenting file $1"
    $UNCRUST -F "$1" -l cpp -c "gwen_uncrustify.cfg" --replace --no-backup
}

function fmttree {
    find $1 -name "*.$2" -type f > fmtfiles.txt
    reformatlist fmtfiles.txt
}

if [ -d "$1" ]; then

    fmttree $1 h
    fmttree $1 cpp

else
    if [ -f "$1" ]; then
        reformat "$1"

        else
        echo "ERROR: As parameter given directory or file does not exist!"
        echo "Syntax is: call_Uncrustify.sh dirname filesuffix"
        echo "Syntax is: call_Uncrustify.sh filename"
        echo "Example: call_Uncrustify.sh temp cpp"
        exit 1
    fi
fi
