#!/usr/bin/env python3
import math
import os
import shutil
import subprocess
import sys
import tempfile


def usage() -> None:
    print(
        f"usage: {os.path.basename(sys.argv[0])} <all_points_2d.ssv> <pareto_points_2d.ssv> [output_png]",
        file=sys.stderr,
    )


def read_points(path: str) -> list[tuple[float, float]]:
    points: list[tuple[float, float]] = []
    with open(path, "r", encoding="utf-8") as handle:
        for line_no, raw in enumerate(handle, start=1):
            line = raw.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) < 2:
                continue
            try:
                x = float(parts[0])
                y = float(parts[1])
            except ValueError:
                raise ValueError(f"invalid numeric point at {path}:{line_no}")
            points.append((x, y))
    return points


def point_in_pareto(point: tuple[float, float], pareto_points: list[tuple[float, float]], tol: float = 1e-9) -> bool:
    x, y = point
    for px, py in pareto_points:
        if math.isclose(x, px, rel_tol=0.0, abs_tol=tol) or math.isclose(y, py, rel_tol=0.0, abs_tol=tol):
            return True
    return False


def write_points(path: str, points: list[tuple[float, float]]) -> None:
    with open(path, "w", encoding="utf-8") as handle:
        for x, y in points:
            handle.write(f"{x:.10g} {y:.10g}\n")


def main() -> int:
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        usage()
        return 1

    all_points_file = sys.argv[1]
    pareto_points_file = sys.argv[2]
    output_png = sys.argv[3] if len(sys.argv) == 4 else "pareto_minmax.png"

    if not os.path.isfile(all_points_file):
        print(f"all-points file not found: {all_points_file}", file=sys.stderr)
        return 1

    if not os.path.isfile(pareto_points_file):
        print(f"pareto-points file not found: {pareto_points_file}", file=sys.stderr)
        return 1

    if shutil.which("gnuplot") is None:
        print("gnuplot is required but was not found", file=sys.stderr)
        return 1

    output_dir = os.path.dirname(output_png)
    if output_dir:
        os.makedirs(output_dir, exist_ok=True)

    try:
        all_points = read_points(all_points_file)
        pareto_points = read_points(pareto_points_file)
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 1

    highlight_points = [p for p in all_points if point_in_pareto(p, pareto_points)]
    pareto_line_points = sorted(pareto_points, key=lambda p: (p[0], p[1]))

    with tempfile.TemporaryDirectory() as tmp_dir:
        all_points_tmp = os.path.join(tmp_dir, "all_points.dat")
        highlight_points_tmp = os.path.join(tmp_dir, "highlight_points.dat")
        pareto_line_tmp = os.path.join(tmp_dir, "pareto_line.dat")
        gnuplot_script = os.path.join(tmp_dir, "plot.gnuplot")

        write_points(all_points_tmp, all_points)
        write_points(highlight_points_tmp, highlight_points)
        write_points(pareto_line_tmp, pareto_line_points)

        with open(gnuplot_script, "w", encoding="utf-8") as handle:
            handle.write("set terminal pngcairo size 1200,800 enhanced\n")
            handle.write(f"set output \"{output_png}\"\n")
            handle.write("set title \"Points with Pareto Matches\"\n")
            handle.write("set xlabel \"x\"\n")
            handle.write("set ylabel \"y\"\n")
            handle.write("set grid\n")
            handle.write("set key outside right\n")

            if highlight_points:
                handle.write(
                    "plot \"{}\" using 1:2 with points pt 7 ps 0.9 lc rgb \"#7f7f7f\" title \"All points\", \\\n"
                    "  \"{}\" using 1:2 with lines lw 2 lc rgb \"#1f77b4\" title \"Pareto front\", \\\n"
                    "  \"{}\" using 1:2 with points pt 5 ps 1.4 lc rgb \"#d62728\" title \"Pareto matches\"\n".format(
                        all_points_tmp, pareto_line_tmp, highlight_points_tmp
                    )
                )
            else:
                handle.write(
                    "plot \"{}\" using 1:2 with points pt 7 ps 0.9 lc rgb \"#7f7f7f\" title \"All points\", \\\n"
                    "  \"{}\" using 1:2 with lines lw 2 lc rgb \"#1f77b4\" title \"Pareto line\"\n".format(
                        all_points_tmp, pareto_line_tmp
                    )
                )

        completed = subprocess.run(["gnuplot", gnuplot_script], capture_output=True, text=True)
        if completed.returncode != 0:
            if completed.stderr.strip():
                print(completed.stderr.strip(), file=sys.stderr)
            else:
                print("gnuplot failed", file=sys.stderr)
            return 1

    print(f"graph generated: {output_png}")
    print(f"matched points: {len(highlight_points)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
