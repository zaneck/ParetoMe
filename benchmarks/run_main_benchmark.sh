#!/bin/sh
set -eu

hyperfine_bin="$1"
exe="$2"
output_dir="$3"
dims_csv="$4"
base_rows="$5"
row_multipliers_csv="$6"
repetitions="$7"
warmup="$8"

results_json="$output_dir/results.json"
summary_file="$output_dir/summary.txt"
summary_markdown="$output_dir/summary.md"
legacy_timings_file="$output_dir/timings.txt"

mkdir -p "$output_dir"
rm -f "$results_json" "$summary_file" "$summary_markdown" "$legacy_timings_file" "$output_dir/pareto.txt" "$output_dir/data.txt"
rm -f "$output_dir"/input_*d_*rows.ssv

old_ifs="$IFS"
IFS=,
set -- $dims_csv
IFS="$old_ifs"

if [ "$#" -eq 0 ]; then
	echo "no benchmark dimensions configured" >&2
	exit 1
fi

benchmark_dim_values="$*"

old_ifs="$IFS"
IFS=,
set -- $row_multipliers_csv
IFS="$old_ifs"

if [ "$#" -eq 0 ]; then
	echo "no benchmark row multipliers configured" >&2
	exit 1
fi

benchmark_row_multiplier_values="$*"

benchmark_dims=""
benchmark_row_counts=""
benchmark_inputs=""

# Job pool for parallel input generation
max_jobs=10
job_count=0

# Function to generate a single input file
generate_input_file() {
	local rows="$1"
	local dim="$2"
	local input_file="$3"
	
	awk -v rows="$rows" -v dim="$dim" '
		BEGIN {
			for (i = 0; i < rows; ++i) {
				for (j = 0; j < dim; ++j) {
					value = ((i * (j + 3)) % 997) / 100.0 + (j + 1);
					printf(j + 1 == dim ? "%.6f\n" : "%.6f ", value);
				}
			}
		}
	' > "$input_file"
}

export -f generate_input_file
export output_dir base_rows

# Generate all input files in parallel
for dim in $benchmark_dim_values; do
	benchmark_dims="${benchmark_dims}  ${dim}\n"
	for multiplier in $benchmark_row_multiplier_values; do
		rows=$((base_rows * multiplier))
		input_file="$output_dir/input_${dim}d_${rows}rows.ssv"

		# Job pool management - wait if max_jobs reached
		while [ "$(jobs -r | wc -l)" -ge "$max_jobs" ]; do
			sleep 0.01
		done

		# Generate input file in background
		generate_input_file "$rows" "$dim" "$input_file" &

		benchmark_row_counts="${benchmark_row_counts}  ${rows}\n"
		benchmark_inputs="${benchmark_inputs}  ${input_file}\n"
	done
done

# Wait for all background jobs to complete
wait

# Build hyperfine command with all benchmark cases
set -- "$hyperfine_bin" \
	--style basic \
	--warmup "$warmup" \
	--runs "$repetitions" \
	--time-unit millisecond \
	--prepare "rm -f \"$output_dir/pareto.txt\" \"$output_dir/data.txt\"" \
	--cleanup "rm -f \"$output_dir/pareto.txt\" \"$output_dir/data.txt\"" \
	--export-json "$results_json" \
	--export-markdown "$summary_markdown"

for dim in $benchmark_dim_values; do
	for multiplier in $benchmark_row_multiplier_values; do
		rows=$((base_rows * multiplier))
		input_file="$output_dir/input_${dim}d_${rows}rows.ssv"
		set -- "$@" --command-name "paretoMe ${dim}d ${rows}r" "cd \"$output_dir\" && \"$exe\" -d \"$dim\" \"$input_file\""
	done
done



{
	printf "paretoMe benchmark\n"
	printf "  dims:\n%b" "$benchmark_dims"
	printf "  row base: %s\n" "$base_rows"
	printf "  row multipliers: %s\n" "$row_multipliers_csv"
	printf "  runs: %s\n" "$repetitions"
	printf "  warmup: %s\n\n" "$warmup"
	"$@"
} | tee "$summary_file"

printf "\nArtifacts:\n"
printf "  inputs:\n%b" "$benchmark_inputs"
printf "  results: %s\n" "$results_json"
printf "  summary: %s\n" "$summary_file"
printf "  markdown: %s\n" "$summary_markdown"