#!/bin/sh
set -eu

exe="$1"
input_file="$2"

set +e
out="$($exe -d 2 "$input_file" extra-arg 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for unexpected trailing argument"
  exit 1
fi

printf "%s" "$out" | grep -q "usage: paretoMe -d dim file"
