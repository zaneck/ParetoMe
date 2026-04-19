#!/bin/sh
set -eu

exe="$1"
input_file="$2"

set +e
out="$($exe -d abc "$input_file" 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for invalid -d value"
  exit 1
fi

printf "%s" "$out" | grep -q "usage: paretoMe -d dim file"
