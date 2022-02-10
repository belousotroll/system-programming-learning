#!/bin/bash

min_count=$1

Products="$(cat database | awk -F' ' '($2 >= '$min_count') { print $1 }' | sed '1d')"

echo $Products
