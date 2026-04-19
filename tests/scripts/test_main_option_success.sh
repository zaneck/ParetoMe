#!/bin/sh
set -eu

exe="$1"
input_file="$2"

rm -f pareto.txt data.txt

$exe -d 2 "$input_file" >/tmp/paretome-main-option-success.out 2>/tmp/paretome-main-option-success.err

[ -f pareto.txt ]
grep -q "min:" /tmp/paretome-main-option-success.out
grep -q "max:" /tmp/paretome-main-option-success.out
