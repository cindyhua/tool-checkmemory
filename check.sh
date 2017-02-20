#!/bin/sh
start=$1
echo "startline:"$start
#
logs=~/logs/ORACLE_SEL_MUTI.out
awk -v startline=$start -f check.awk  $logs


