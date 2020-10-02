#!/bin/csh -f

#
# Run the given command against the set of arguments given below
#
# config stuff
if ( $#argv != 2 ) then
  echo "usage: $0 <cmd> <datadir>"
  exit -1
endif

set cmd     = $argv[1]
set datadir = $argv[2]

if ( -d /usr/share/man/man1 ) then
  set mandir = /usr/share/man/man1
else
  set mandir = /usr/man/man1
endif

# the arguments---only change these 
#-----------------------------------------------------------------------
set timeout = 300
set args    = ( $mandir/* )
#-----------------------------------------------------------------------

# The run
timelimit $timeout $cmd $args |& iolimit 1024 1048576

# exit with the status left behind by cmd...
exit $status
