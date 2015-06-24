set -e

fail_message() {
    bad.sh
}

trap " fail_message " INT TERM EXIT
macaroni --messagesPath=Source/main/resources --libraryRepoPath=../Libraries  -b  --showPaths -o "link=static --toolset=msvc-12.0 -j8" --printTimes --verbose
trap - INT TERM EXIT
good.sh
