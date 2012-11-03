TOOLS_ROOT=$HOME/Tools
BOOST_ROOT=$TOOLS_ROOT/boost/boost_1_51_0

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
    ./b2 --toolset=gcc cxxflags=-std=gnu++11 stage
}

function cmd_install() {
    install_gcc
    install_boost
}

function cmd_build() {

}

case "$1" in
        "install" ) cmd_install;;
    * )
    echo "'$1' not a valid command"
    exit 1
esac


