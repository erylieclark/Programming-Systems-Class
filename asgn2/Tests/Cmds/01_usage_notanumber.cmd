#!/bin/csh -f

#
# Run the given command against the set of arguments given below
#

# the arguments---only change these
#-----------------------------------------------------------------------
set timeout = 1
set args = ( -n notanumber )
#-----------------------------------------------------------------------


# config stuff
if ( $#argv != 2 ) then
  echo "usage: $0 <cmd> <datadir>"
  exit -1
endif

set cmd     = $argv[1]
set datadir = $argv[2]


# test for a usage message:

# The run
timelimit $timeout $cmd $args 	|& iolimit 1024 1048576 >& tmpfile.$$

if ( { ( grep -i 'usage' tmpfile.$$ > /dev/null ) } ) then
    echo "Usage message found."
else 
    echo "No usage message".
endif

rm -f  tmpfile.$$

# exit with the status left behind by cmd...
exit 0
