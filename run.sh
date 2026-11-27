#!/bin/sh
set -eu

cd "$(dirname "$0")"
: "${CADMIUM:?Set CADMIUM to the path of the Cadmium v2 dev-rt checkout}"
compiler=${CXX:-c++}

mkdir -p build results
"$compiler" -std=c++17 -pthread -I"$CADMIUM/include" \
    -I"$CADMIUM/include/cadmium" -Isrc/blinky src/blinky/main.cpp -o build/Blinky
"$compiler" -std=c++17 -pthread -I"$CADMIUM/include" \
    src/leo_sim.cpp -o build/leo_sim

build_dir="$PWD/build"
results_dir="$PWD/results"
(
    cd src/blinky
    "$build_dir/Blinky" > "$results_dir/blinky_trace.txt"
    mv blinkyLog.csv "$results_dir/blinky.csv"
)
for mode in satellite odu coupled; do
    build/leo_sim "$mode" src/odu_input.txt "results/$mode.csv"
done
