TOOLS_ROOT=$HOME/Tools
export BOOST_ROOT=$TOOLS_ROOT/boost/boost_1_51_0
MACARONI_SOURCE=/macaroni
MACARONI_EXE=$MACARONI_SOURCE/Main/App/bin/gcc-4.7/debug/link-static/threading-multi/macaroni

export PATH=$PATH:$BOOST_ROOT
export PATH=$PATH:$BOOST_ROOT/stage/lib

function install_gcc() {
    # Get access to a PPA where the latest GCC is available.
    sudo apt-get update
    sudo apt-get install -y python-software-properties
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install -y gcc-4.7
    # Now make sure the default gcc and G++ are 4.7.
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.7 100
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.7 100
    sudo update-alternatives --install /usr/bin/cpp cpp-bin /usr/bin/cpp-4.7 100
    sudo update-alternatives --set g++ /usr/bin/g++-4.7
    sudo update-alternatives --set gcc /usr/bin/gcc-4.7
    sudo update-alternatives --set cpp-bin /usr/bin/cpp-4.7
}

function install_boost() {
    sudo apt-get install -y python-dev
    set +e
    mkdir $TOOLS_ROOT
    mkdir $TOOLS_ROOT/boost
    set -e
    cd $TOOLS_ROOT/boost
    wget http://sourceforge.net/projects/boost/files/boost/1.51.0/boost_1_51_0.tar.gz/download
    tar -xvf download
    cd $BOOST_ROOT
    ./bootstrap.sh
    ./b2 --toolset=gcc cxxflags=-std=gnu++11 --layout=versioned --build-type=complete stage
    sudo cp -rf ~/Tools/boost/boost_1_51_0/stage/lib/* /usr/local/lib/
}

function cmd_install() {
    install_gcc
    install_boost
}

function cmd_build() {
    cd $MACARONI_SOURCE/Main/App
    export CPLUS_INCLUDE_PATH=$BOOST_ROOT
    export LD_LIBRARY_PATH=$BOOST_ROOT/stage/lib
    export LIBRARY_PATH=$BOOST_ROOT/stage/lib
    export BOOST_LIB_SUFFIX=-gcc47-mt-1_51
    pwd
    ls -la
    bjam -d+2 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi
}

function cmd_unit_test() {
    $MACARONI_EXE luaTests $MACARONI_SOURCE/Main/App/Source/test/lua
}

function create_init_lua() {
    set +e
    mkdir ~/Macaroni
    set -e
    echo "
properties =
{
  boost =
  {
    ['1.51'] =
    {
      include=[[C:/Users/Tim/Tools/Local/Boost/boost_1_51_0]],
      lib=[[C:/Users/Tim/Tools/Local/Boost/boost_1_51_0/stage/lib]],
      path=[[C:/Users/Tim/Tools/Local/Boost/boost_1_51_0]],
    },
  },
  lua =
  {
    ['5.1.4'] =
    {
      include = [[C:/Users/Tim/Tools/Local/Lua/lua-5.1.4/src]],
      source = [[C:/Users/Tim/Tools/Local/Lua/lua-5.1.4/src]],
    },
  },
}
properties.boost.current = properties.boost['1.51'];
properties.bjam_options = '-d+2 '
" > ~/Macaroni/init.lua
}

function cavatappi_args() {
    echo "generatorPath $MACARONI_SOURCE/Main/Generators libraryRepoPath $MACARONI_SOURCE/Main/Libraries messagesPath $MACARONI_SOURCE/Main/App/Source/main/resources cavatappi $@"
}

function cmd_run() {
    create_init_lua
    $MACARONI_EXE `cavatappi_args` $@
}

function cmd_debug() {
    create_init_lua
    echo "Type this to debug:"
    echo "run `cavatappi_args` $@"
    gdb $MACARONI_EXE
}

case "$1" in
        "install" ) cmd_install;;
        "build" ) cmd_build;;
        "unit-test" ) cmd_unit_test;;
        "run" ) cmd_run;;
        "debug" ) cmd_debug;;
    * )
    echo "'$1' not a valid command"
    exit 1
esac


