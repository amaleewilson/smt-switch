#!/bin/bash

# requires autoconf, gperf

YICES2_VERSION=98fa2d882d83d32a07d3b8b2c562819e0e0babd0

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
DEPS=$DIR/../deps

mkdir -p $DEPS

unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    *)          machine="UNKNOWN:${unameOut}"
esac
# echo ${machine}



if [ ! -d "$DEPS/yices2" ]; then
    cd $DEPS
    git clone https://github.com/SRI-CSL/yices2.git
    chmod -R 777 yices2
    cd yices2
    # Handle gmp dependence
    # TODO: put gmp in DEPS/gmp
    # then check if it exists
    # so that mathsat can use it too
    if [ "${machine}" == "Linux"  ]; then
        wget "https://gmplib.org/download/gmp/gmp-6.1.2.tar.xz"
        EXT=a
    elif [ "${machine}" == "Mac" ]; then
        curl -O https://gmplib.org/download/gmp/gmp-6.1.2.tar.xz
        EXT=dylib
    fi
    tar -xf gmp-6.1.2.tar.xz 
    rm gmp-6.1.2.tar.xz
    cd gmp-6.1.2
    ./configure --prefix=$DEPS/yices2/gmp
    make
    make install
    cd ..
    # End handle gmp dependence

    git checkout -f $YICES2_VERSION
    autoconf
    # TODO: unclear if this *needs* to be this complicated, but since we are 
    # interested in building yices2 statically, I think this is, in fact, necessary. 
    ./configure --with-pic-gmp=${PWD}/gmp/lib/libgmp.${EXT} --with-pic-gmp-include-dir=${PWD}/gmp/include LDFLAGS="-L${PWD}/gmp/lib" CPPFLAGS="-I${PWD}/gmp/include"

    make build_dir=build BUILD=build -j$(nproc)
    cd $DIR
else
    echo "$DEPS/yices2 already exists. If you want to rebuild, please remove it manually."
fi

# if [ -f $DEPS/yices2/build/lib/libyices.a ] && [ -f $DEPS/yices2/build/obj/parser_utils/parser.o ] ; then \
#     echo "It appears yices2 was setup successfully into $DEPS/yices2."
#     echo "You may now install it with make ./configure.sh --yices2 && cd build && make"
# else
#     echo "Building yices2 failed."
#     echo "You might be missing some dependencies."
#     echo "Please see their github page for installation instructions: https://github.com/SRI-CSL/yices2"
#     exit 1
# fi



