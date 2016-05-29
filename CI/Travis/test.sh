set -e
export CXX="g++-5"
export CC="gcc-5";
export BOOST_ROOT=$HOME/Tools/boost_1_58_0
export PATH="$PATH:$BOOST_ROOT"
export WORK_DIR=`pwd`

function cavatappi() {
    "$WORK_DIR/Main/App/GeneratedSource/exe/macaroni" "--generatorPath=$WORK_DIR/Main/Generators" --libraryRepoPath="$WORK_DIR/Main/Libraries"  "--messagesPath=$WORK_DIR/Main/App/Source/main/resources/Messages.txt" $@
}

pushd Next/Tests
cavatappi -c -i
popd
