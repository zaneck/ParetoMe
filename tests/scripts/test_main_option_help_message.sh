#!/bin/sh
set -eu

exe="$1"

set +e
out="$($exe -h 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for help option"
  exit 1
fi

printf "%s" "$out" | grep -q "usage: paretoMe -d dim file"
