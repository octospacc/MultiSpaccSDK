#!/bin/sh
[ "$(shell uname --operating-system)" = "Msys" ] \
&& cmd //c "$(cygpath --windows "$(dirname "$(realpath "$0")")/WindowsRefreshEnv.cmd") && python3 $@" \
|| python3 $@ \
;