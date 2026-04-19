#!/bin/sh
set -eu

exe="$1"
missing_file="$2"

set +e
out="$($exe 2 "$missing_file" 2>&1)"
status=$?
set -e

if [ "$status" -eq 0 ]; then
  echo "expected non-zero exit status for missing input file"
  exit 1
fi

printf "%s" "$out" | grep -q "fopen:"
