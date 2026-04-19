#!/bin/sh
set -eu

script_dir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"

if ! command -v python3 >/dev/null 2>&1; then
  echo "python3 is required but was not found" >&2
  exit 1
fi

exec python3 "$script_dir/plot_pareto_minmax.py" "$@"
