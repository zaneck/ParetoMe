#!/bin/sh
set -eu

exe="$1"

set +e
out="$($exe 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for missing args"
  exit 1
fi

printf "%s" "$out" | grep -q "paretoMe dim file"
