#!/bin/sh
set -eu

exe="$1"
input_file="$2"

rm -f pareto.txt data.txt

set +e
out="$($exe 2 "$input_file" 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for malformed input"
  exit 1
fi

printf "%s" "$out" | grep -q "invalid input at line 1"
