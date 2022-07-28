#!/bin/sh
 
files=$(ulimit -n)
echo "Increasing the maximal number of opened files from $files to 16384"
ulimit -n 16384
 
exec $@