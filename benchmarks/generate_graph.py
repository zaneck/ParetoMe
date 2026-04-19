#!/usr/bin/env python3
"""
Generate gnuplot graph from hyperfine results.json
"""
import json
import sys
import re
import subprocess
import platform
from pathlib import Path
from collections import defaultdict


def _read_command_output(command):
    try:
        completed = subprocess.run(
            command,
            capture_output=True,
            text=True,
            timeout=2,
            check=False,
        )
        if completed.returncode == 0:
            return completed.stdout.strip()
    except Exception:
        return ""
    return ""


def detect_cpu_info():
    system_name = platform.system()

    if system_name == "Darwin":
        brand = _read_command_output(["sysctl", "-n", "machdep.cpu.brand_string"])
        if brand:
            return brand
        model = _read_command_output(["sysctl", "-n", "hw.model"])
        machine = _read_command_output(["sysctl", "-n", "hw.machine"])
        if model and machine:
            return f"{model} ({machine})"
        if model:
            return model
        if machine:
            return machine

    if system_name == "Linux":
        model_name = _read_command_output(["sh", "-c", "awk -F: '/model name/ {print $2; exit}' /proc/cpuinfo | sed 's/^ *//'" ])
        if model_name:
            return model_name

    return (platform.processor() or platform.machine() or "unknown").strip()

def main():
    if len(sys.argv) < 3:
        print("Usage: generate_graph.py <results.json> <output_dir>", file=sys.stderr)
        sys.exit(1)
    
    results_file = Path(sys.argv[1])
    output_dir = Path(sys.argv[2])
    
    if not results_file.exists():
        print(f"Error: {results_file} not found", file=sys.stderr)
        sys.exit(1)
    
    # Load results
    with open(results_file) as f:
        data = json.load(f)
    
    # Extract and organize data by dimension
    dims_data = defaultdict(list)  # dim -> [(rows, mean_ms), ...]
    
    for result in data.get("results", []):
        cmd = result.get("command", "")
        mean_sec = result.get("mean", 0)
        mean_ms = mean_sec * 1000  # Convert to milliseconds
        
        # Parse command: "paretoMe 10d 10000r"
        match = re.search(r'(\d+)d\s+(\d+)r', cmd)
        if match:
            dim = int(match.group(1))
            rows = int(match.group(2))
            dims_data[dim].append((rows, mean_ms))
    
    if not dims_data:
        print("Error: No benchmark data found in results.json", file=sys.stderr)
        sys.exit(1)
    
    # Sort data by rows for each dimension
    for dim in dims_data:
        dims_data[dim].sort(key=lambda x: x[0])
    
    # Write data file
    data_file = output_dir / "benchmark_data.txt"
    with open(data_file, 'w') as f:
        f.write("# rows\tdimension\ttime_ms\n")
        for dim in sorted(dims_data.keys()):
            for rows, mean_ms in dims_data[dim]:
                f.write(f"{rows}\t{dim}\t{mean_ms:.2f}\n")
    
    # Generate gnuplot script
    script_file = output_dir / "benchmark.gnuplot"
    graph_file = output_dir / "benchmark_results.png"

    os_info = f"{platform.system()} {platform.release()}".strip()
    cpu_info = detect_cpu_info()
    # Keep gnuplot title safe if environment strings contain quotes.
    os_info = os_info.replace('"', "")
    cpu_info = cpu_info.replace('"', "")
    
    with open(script_file, 'w') as f:
        f.write("set terminal png size 1200,700\n")
        f.write(f"set output '{graph_file}'\n")
        f.write(f"set title \"ParetoMe Benchmark Results by Dimension\\nOS: {os_info} | CPU: {cpu_info}\"\n")
        f.write("set xlabel 'Input Rows'\n")
        f.write("set ylabel 'Execution Time (ms)'\n")
        f.write("set grid\n")
        f.write("set key outside right\n")
        f.write("set style data lines\n")
        
        # Generate plot command with one line per dimension
        plot_commands = []
        colors = {10: 1, 200: 2, 300: 3, 400: 4, 500: 5}
        for dim in sorted(dims_data.keys()):
            color = colors.get(dim, len(plot_commands) + 1)
            plot_cmd = f"'{data_file}' using 1:($2=={dim}?$3:1/0) title '{dim}d' with lines linewidth 2 linecolor {color}"
            plot_commands.append(plot_cmd)
        
        f.write("plot " + ", ".join(plot_commands) + "\n")
    
    # Run gnuplot
    try:
        result = subprocess.run(['gnuplot', str(script_file)], 
                              capture_output=True, text=True, timeout=10)
        if result.returncode == 0:
            print(f"Graph generated: {graph_file}")
        else:
            print(f"Warning: gnuplot returned {result.returncode}", file=sys.stderr)
            if result.stderr:
                print(result.stderr, file=sys.stderr)
            sys.exit(1)
    except FileNotFoundError:
        print("Warning: gnuplot not found, skipping graph generation", file=sys.stderr)
        sys.exit(1)
    except subprocess.TimeoutExpired:
        print("Error: gnuplot timed out", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
