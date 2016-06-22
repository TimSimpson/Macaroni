#!/usr/bin/env bash
###############################################################################
#  Macaroni Build Helper
###############################################################################
#
# I've got the memory of a peacock so I like to make Bash scripts like this
# to help run things.
#
# To make this work, you'll need to run "git submodule update --init" at least
# once.
#
# If you hate Bash, then fine- don't use it! Go run the commands by hand. The
# goal of this script is as more about making it easy to remember what command
# line invocation did what than it is about automation.
#
###############################################################################
set -e

bashi_base_command="${bashi_base_command:-$0}"
readonly m_root=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ "" == "${WINDIR}" ]; then
    readonly this_is_windows=
    function cavatappi() {
        cavatappi $@
    }
    function conan() {
        conan $@
    }
else
    readonly this_is_windows=true
    function cavatappi() {
        cmd //c cavatappi $@
    }
    function conan() {
        cmd //c conan $@
    }
fi

function cmd_clean() {
    # Delete the build directory
    cd "${m_root}/Main/App"
    rm -rf ./build
}

function cmd_build_deps() {
    # Builds dependencies
    cd "${m_root}/Main/Dependencies/Lua"
    conan export demo/testing
    conan test_package
}

function cmd_build() {
    # Build the main app
    cd "${m_root}/Main/App"
    mkdir -p build/release
    cd build/release
    if [ "" == "${this_is_windows}" ]; then
        conan install ../.. -s 'compiler=Visual Studio' -s compiler.version=12 -s build_type=Release -s arch=x86_64 -s os=Windows
    else
        cmd //c conan install ../.. -s compiler=Visual\ Studio -s compiler.version=12 -s build_type=Release -s arch=x86_64 -s os=Windows
    fi
    conan build ../..
}

function cmd_unit_tests() {
    # Run the meager unit tests.
    cd "${m_root}/Main/App"
    ./build/bin/macaroni.exe \
      --luaTests=Source/test/lua \
      --messagesPath=Source/main/resources/Messages.txt
}

function cmd_tests() {
    # Run the integration tests
    echo TODO
}

function cmd_refresh () {
    # Refresh the command list.
    mkdir -p Output
    python "${m_root}/BashiBazook/bashi.py" "${m_root}/mrun.sh" > "${m_root}/mrun.bashi-q"
    cp "${m_root}/mrun.bashi-q" "${m_root}/mrun.bashi"
}

bashi_help_preamble="
 :;;
 ;;;;.
 ;;;;;;
 ;;;';;;;
 ;;;'';;;;\`
 ;;;''';;;;;;                   @#@#
 :;;''''';;;;;;;:.              @':@
 \`;;;'''''''';;;;;;;            @'#@   #@@;  @++@\`
  ;;;'''''''''''';;;;;;\`     \`@@@,:@  @#,,@@+@;,,@.@@@
  :;;;''''''''''''';;;;;;,  ,@:,@@@@.#+,,#@@',@;,@\`#,+;
   ;;;;'''''''''''''';;;;;@#@,;@:,,,@@,,'@@@#@@@,# @@+@'
    ;;;;''''''''''''''';;;;#@:@,,,#@+',,@@::@,+@++@:,,,@
     ;;;;'''''''''''''''';;;#@:,,@+,#+,,@@::@@#:#@',:@@;  @@#
      .;;;'''''''''''''''';;;#,,'+,#@@@@@:@@@+,,,:@#@,@ , @,;@'
        :;;;''''''''''''''';;;#@@:@+,,,,#@;,@,,,;@#@,:@@+@@,,,@
          \`;;;;;'''''''''''';;;;'@,@@,,@',#@#,,:@,@+@@@@;@@,,:@
              ;;;;;''''''''''';;;:,@@,,@#,,+@,,@@@@#:,@@@@@,,@
               :;;;;;;'''''''''';;;;,,,:,,,,@;,@:,,;@@#+:@,,:@
                 \`;;;;;;;;'''''''';;;#@@@,,,,@,@,,::,,@'@:,,@\`
                    \`;;;;;;;;;''''''';@@@@,,,@@',@@@@,,@,,,##@
                      .@#';;;;;;;;;;'''@@@'@@#@,@;@,@','',+@,@
                      @@@:@;;'@,'@'#\`@@@;@@;,@@@@@#'@@+#'@@,,@
                      @\`.,,;;@,,@;,@#\`,@@',@@#,;@@,;+.:+@@@@@@@@
                      :@\`\`\`\`\`@,,@@,,;@#;,,:@+,,,,@\`\`\`\`\`\`\`\`\`\`\`\`@.
                        #\`\`\`\`@,,@.@',,,,;@@';,,,,@\`\`\`\`\`\`\`\`\`\`\`@
                        @+@@@@,,;#\`+@@@@@...#,,,,@'###@@@+\`\`@
                        @\`\`\`\`@,,,@@@#####+++@,,,,@,...\`\`\`\`\`:#
                        #,\`\`\`,@,,:@.\`\`\`\`\`\`\`\`@:,,,@\`\`\`\`\`\`\`\`\`@
                         @\`\`\`\`#@'@\`\`\`\`\`\`\`\`\`\`.@,,,:@\`\`\`\`\`\`\`.@
                         :@\`\`\`\`.#\`\`\`\`\`\`\`\`\`\`\`\`@',,,@,\`\`\`\`\`\`@
                          @\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`@:,,,@\`\`\`\`\`:@
                           @\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`@:,;@.\`\`\`\`@
                           @:\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`@@#\`\`\`\`\`;#
                            @\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`.@
                             @,\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`.@
                              #@@\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`\`@@.
                               .\`@@@@;,\`\`\`\`\`\`;;;;;;@@@,
                              @@@@@@@@@@@@@@@@@@@@@@@@@@
                              @@@@@@@@@@@@@@@@@@@@@@@@@@++\`
                                @@@@@@@@@@@@@@@@@@@@@@@@@++@@
                                '@@@@@@@@@@@@@@@@@@@@@@@@@@@.
                                ,#@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                              @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##;.

COMMANDS "

set +e
source "${m_root}/mrun.bashi" || set -e && cmd_refresh
set -e

source "${m_root}/mrun.bashi"

bashi_run $@
