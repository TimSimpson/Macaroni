export CXX="g++-5"
export CC="gcc-5";
export BOOST_ROOT=$HOME/Tools/boost_1_58_0
export PATH="$PATH:$BOOST_ROOT"
pushd Main/App
~/OldMacaroni/macaroni --messagesPath Source/main/resources/Messages.txt --libraryRepoPath=../Libraries  -b  --showPaths -o "link=static release" --printTimes --verbose
popd
