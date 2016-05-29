set -e
set -o xtrace;
WORK_DIR=`pwd`

#libboost1.58-tools-dev libboost-filesystem1.58-dev libboost-program-options1.58-dev libboost-regex1.58-dev

echo 'What GCC is this anyway?'
export CXX="g++-5"
export CC="gcc-5";

echo `which gcc`
echo `gcc --version`
echo `which gcc-5`

# echo 'Switch to GCC 5'
# sudo unlink /usr/bin/gcc && sudo ln -s /usr/bin/gcc-5 /usr/bin/gcc
# gcc --version

mkdir -p ~/logs

if [ ! -d ~/Tools/boost_1_58_0 ]; then
    echo 'Boost (I know, I am a bad person)'
    mkdir -p ~/Tools
    pushd ~/Tools
    wget http://sourceforge.net/projects/boost/files/boost/1.58.0/boost_1_58_0.tar.gz/download
    echo 'Extracting Boost...'
    tar -xvf download > /dev/null
    cd ~/Tools/boost_1_58_0
    ./bootstrap.sh
    #./b2 --toolset=gcc
    popd
fi

cp CI/Travis/site-config.jam ~/

if [ ! -d ~/OldMacaroniBuild ]; then
    echo 'Build previous version of Macaroni'
    mkdir -p ~/OldMacaroniBuild
    pushd ~/OldMacaroniBuild
    wget http://border-town.com/macaroni/downloads/macaroni-0.4.1-pureCpp.zip
    unzip macaroni-0.4.1-pureCpp.zip
    sed "s%boost.use-project 1.57 ;%boost.use-project 1.58 ;%" jamroot.jam > jr2
    cp jr2 jamroot.jam
    echo 'Bjam, go!'
    export BOOST_ROOT=$HOME/Tools/boost_1_58_0
    export PATH="$PATH:$BOOST_ROOT"
    bjam -j2 link=static release -q
    popd
fi

if [ ! -d ~/OldMacaroni ]; then
    echo 'Install previous version of Macaroni'
    mkdir -p ~/OldMacaroni
    pushd ~/OldMacaroni
    wget http://border-town.com/macaroni/downloads/macaroni-0.4.1-windows.zip
    unzip macaroni-0.3.0-windows.zip
    rm macaroni.exe
    cp ../OldMacaroniBuild/exe/macaroni_p macaroni
    # chmod +x macaroni
    cp "${WORK_DIR}/CI/Travis/FileGenerator-Fix.lua" Generators/Cpp/FileGenerator.lua
    echo 'Test previous version of Macaroni works.'
    ./macaroni --about
    popd
fi



if [ ! -d ~/MacDeps ]; then
    mkdir -p ~/MacDeps
    pushd ~/MacDeps
    wget http://885875f368d47bdd75f4-fccc9ce7610ca8b8e6180232413c69c8.r20.cf2.rackcdn.com/macdeps.zip
    unzip macdeps.zip
    popd
fi


mkdir -p ~/Macaroni
cp CI/Travis/init.lua ~/Macaroni/
echo 'Fix the complete failure to parse the tilda (which is ironic).'
sed "s%~HOME~%$HOME%" CI/Travis/init.lua > ~/Macaroni/init.lua
pushd Main/Dependencies/Lua
~/OldMacaroni/macaroni "--libraryRepoPath=$HOME/OldMacaroni/Libraries" "--generatorPath=$HOME/OldMacaroni/Generators" -i
popd
pushd Main/Dependencies/MoonscriptDeps
~/OldMacaroni/macaroni "--libraryRepoPath=$HOME/OldMacaroni/Libraries" "--generatorPath=$HOME/OldMacaroni/Generators" -i
popd


# pushd ~/Tools
# git clone https://github.com/boostorg/build.git
# cd build
# ./bootstrap.sh
