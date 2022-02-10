#!/bin/bash

selected_pid=$1

PIDS_TO_KILL="$(ps axfo pid | awk -F' ' '($1 >= '$selected_pid') { print $1 }')"

kill -9 $PIDS_TO_KILL
