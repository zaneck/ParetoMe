#!/bin/sh
set -eu

exe="$1"
input_file="$2"

rm -f pareto.txt data.txt

$exe 2 "$input_file" >/tmp/paretome-main-success.out 2>/tmp/paretome-main-success.err

[ -f pareto.txt ]
grep -q "min:" /tmp/paretome-main-success.out
grep -q "max:" /tmp/paretome-main-success.out
