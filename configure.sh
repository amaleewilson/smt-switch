#!/bin/sh

# Syntax and structure borrowed from CVC4's configure.sh script

CONF_FILE=Makefile.conf

usage () {
cat <<EOF
Usage: $0 [<option> ...]

-h, --help              display this message and exit
--prefix=STR            install directory
--btor-home=STR         custom BTOR location
--cvc4-home=STR         custom CVC4 location
--clean                 remove any existing configuration state
EOF
  exit 0
}

die () {
    echo "*** $0: $*" 1>&2
    exit 1
}

rm -f $CONF_FILE

if [ $# = 0 ]
then
    usage
fi

while [ $# -gt 0 ]
do
    case $1 in
        -h|--help) usage;;
        --prefix) die "missing argument to $1 (see -h)" ;;
        --prefix=*)
            install_prefix=${1##*=}
            # Check if install_prefix is an absolute path and if not, make it
            # absolute.
            case $install_prefix in
                /*) ;;                                      # absolute path
                *) install_prefix=$(pwd)/$install_prefix ;; # make absolute path
            esac
            echo -e "prefix=$install_prefix" >> $CONF_FILE;;
        --btor-home=*)
            btor_home=${1##*=}
            # Check if btor_home is an absolute path and if not, make it
            # absolute.
            case $btor_home in
                /*) ;;                                      # absolute path
                *) btor_home=$(pwd)/$btor_home ;; # make absolute path
            esac
            echo -e "BTOR_HOME=$btor_home" >> $CONF_FILE;;
        --cvc4-home=*)
            cvc4_home=${1##*=}
            # Check if cvc4_home is an absolute path and if not, make it
            # absolute.
            case $cvc4_home in
                /*) ;;                                      # absolute path
                *) cvc4_home=$(pwd)/$cvc4_home ;; # make absolute path
            esac
            echo -e "CVC4_HOME=$cvc4_home" >> $CONF_FILE;;
        --clean) rm -f $CONF_FILE;;
        *) die "unexpected argument: $1";;
    esac
    shift
done
