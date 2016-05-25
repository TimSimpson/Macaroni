#!/bin/bash

set -e

CPU_BITS=${CPU_BITS:-32}
TOOLS_ROOT=${TOOLS_ROOT:-$HOME/Tools}
export BOOST_ROOT=${BOOST_ROOT:-$TOOLS_ROOT/boost/boost_1_58_0}
export LUA_ROOT=${LUA_ROOT:-$TOOLS_ROOT/lua/lua-5.1.4}
MACARONI_SOURCE=${MACARONI_SOURCE:-/macaroni}
MACARONI_PURECPP_SOURCE=${MACARONI_PURECPP_SOURCE:-MACARONI_SOURCE/Main/App/PureCpp}
MACARONI_EXE=$MACARONI_SOURCE/Main/App/PureCpp/bin/gcc-linux/debug/address-model-$CPU_BITS/link-static/threading-multi/macaroni_p
PATH=$PATH:$BOOST_ROOT
export PATH=$PATH:$BOOST_ROOT/stage/lib

function explain_vars() {
    echo "
    TOOLS_ROOT - This is a directory where the script will try to download
        multiple dependencies, such as the Boost libraries and Lua.
    BOOST_ROOT - Boost lib location. Defaults to a directory inside TOOLS_ROOT.
    LUA_ROOT - Lua lib location. Defaults to a directory inside TOOLS_ROOT.
    MACARONI_SOURCE - The root directory of a working copy of Macaroni's repo.
    MACARONI_PURECPP_SOURCE - Directory of 'pure C++' distribution of Macaroni.
        This defaults to a location within MACARONI_SOURCE.
    MACARONI_EXE - The binary of Macaroni once it's build. Defaults to a path
        relative to the MACARONI_PURECPP_SOURCE directory.
    "
}

function install_gcc() {
    # Get access to a PPA where the latest GCC is available.
    sudo apt-get update
    sudo apt-get install -y python-software-properties
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install -y gcc-4.8
    sudo apt-get install -y g++-4.8
    # Now make sure the default gcc and G++ are 4.8.
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 100
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 100
    sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.8 100
    sudo update-alternatives --set g++ /usr/bin/g++-4.8
    sudo update-alternatives --set gcc /usr/bin/gcc-4.8
    sudo update-alternatives --set cpp-bin /usr/bin/cpp-4.8

    # I think this is needed to compile 32 and 64 bit versions.
    sudo apt-get install gcc-multilib
    sudo apt-get install gcc-4.8-multilib
    sudo apt-get install g++-4.8-multilib
}

function invoke_b2() {
    ./b2 --toolset=gcc cxxflags=-std=gnu++11 --layout=versioned \
        --build-type=complete $1 stage
}
function install_boost() {
    sudo apt-get install -y python-dev
    set +e
    mkdir $TOOLS_ROOT
    mkdir $TOOLS_ROOT/boost
    set -e
    cd $TOOLS_ROOT/boost
    wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz/download
    tar -xvf download
    cd $BOOST_ROOT
    ./bootstrap.sh
    invoke_b2 --with-filesystem
    invoke_b2 --with-regex
    invoke_b2 --with-test
    sudo cp -rf ~/Tools/boost/boost_1_58_0/stage/lib/* /usr/local/lib/
}

function install_lua() {

    mkdir -p $TOOLS_ROOT
    pushd $TOOLS_ROOT
    wget http://885875f368d47bdd75f4-fccc9ce7610ca8b8e6180232413c69c8.r20.cf2.rackcdn.com/macdeps.zip
    unzip macdeps.zip
    popd
}

function create_user_config() {
    echo "
using boost
    :   1.58
    ;

using gcc
        : linux
        : g++
        : <cxxflags>"--std=c++14"
        ;
" > ~/user-config.jam
}


function cmd_install() {
    # install_gcc
    install_boost
    install_lua
    create_user_config
}


function _build() {
    cd $MACARONI_SOURCE/Main/App/PureCpp
    bjam -d+2 -q \
        address-model=$CPU_BITS \
        --toolset=gcc-linux link=static \
        threading=multi $_EXTRA_BUILD_OPTIONS $@
}
function cmd_build() {
    _build $@
}

function cmd_build_release() {
    _EXTRA_BUILD_OPTIONS=release
    _build $@
}

function cmd_unit_test() {
    $MACARONI_EXE --messagesPath $MACARONI_SOURCE/Main/App/Source/main/resources luaTests $MACARONI_SOURCE/Main/App/Source/test/lua
}

function create_init_lua() {
    set +e
    mkdir -p ~/Macaroni
    set -e
    echo "
properties =
{
    boost =
    {
        ['1.58'] =
        {
          include=[[$BOOST_ROOT]],
          lib=[[$BOOST_ROOT/stage/lib]],
          path=[[$BOOST_ROOT]],
        },
    },
    lpeg = {
        ['0.12'] = {
            bin = '$TOOLS/lpeg/lpeg-0.12/bin',
            include = '$TOOLS/lpeg/lpeg-0.12',
            source = '$TOOLS/lpeg/lpeg-0.12'
        },
    },
    lua = {
        ['5.2.3'] =
        {
            bin = '$TOOLS/lua/lua-5.2.3/bin',
            include = '$TOOLS/lua/lua-5.2.3/src',
            source = '$TOOLS/lua/lua-5.2.3/src',
        }
    },
    luafilesystem = {
        ['1.6.2'] = {
            source = '$TOOLS/luafilesystem/luafilesystem-1_6_2/src',
        },
    },
}
properties.boost.current = properties.boost['1.58'];
properties.bjam_options = '-d+2 -q ';
properties.boost.version = '1.58';
" > ~/Macaroni/init.lua
}

function cavatappi_args() {
    echo "--generatorPath $MACARONI_SOURCE/Main/Generators --libraryRepoPath $MACARONI_SOURCE/Main/Libraries --messagesPath $MACARONI_SOURCE/Main/App/Source/main/resources $@"
}

function cmd_run() {
    create_init_lua
    $MACARONI_EXE `cavatappi_args $@`
}

function cmd_debug() {
    create_init_lua
    echo "Type this to debug:"
    echo "run `cavatappi_args $@`"
    gdb $MACARONI_EXE
}

function cmd_all_tests() {
    cmd_unit_test
    pushd /macaroni/Main/Dependencies/Lua
    cmd_run clean install
    popd
    pushd /macaroni/Next/Tests
    cmd_run clean install
    popd
}

function cmd_help() {
    echo "Unknown command: $1

Here are the valid commands, in order they should be run:

    install   - Installs the GCC compiler and Boost libraries.
                Only do this if you haven't already set this stuff up yourself
                or it might screw with files you've customized.
    build     - Builds Macaroni
    unit-test - Runs the unit tests.
    debug     - Runs Macaroni in debug mode.
    run       - Runs Macaroni normally.
    build-release - Builds the release version.
    all-tests  - Runs all tests.
"
}

case "$1" in
        "install" ) cmd_install;;
        "build" ) shift 1; cmd_build $@;;
        "unit-test" ) cmd_unit_test;;
        "run" ) shift 1; cmd_run $@;;
        "debug" ) shift 1; cmd_debug $@;;
        "install_gcc" ) install_gcc;;
        "install_lua" ) install_lua;;
        "build-release" ) shift 1; cmd_build_release $@;;
        "all-tests" ) shift 1; cmd_all_tests $@;;
    * )
    cmd_help $1
    exit 1
esac


