#!/bin/sh
set -eu

cd "$(dirname "$0")"
work_dir=$(mktemp -d)
trap 'rm -rf "$work_dir"' EXIT
project="$PWD"

if [ ! -x build/Blinky ] || [ ! -x build/leo_sim ]; then
    printf 'Build the models with ./run.sh first. See README.md.\n' >&2
    exit 1
fi

case "${1:-}" in
    blinky)
        cp src/blinky/input.txt "$work_dir/input.txt"
        printf '$ ./build/Blinky\n\n'
        (cd "$work_dir" && "$project/build/Blinky" > trace.txt)
        head -n 13 "$work_dir/trace.txt"
        printf '\nAt 10 s, the input changes the time advance to 0.5 s.\n'
        ;;
    satellite|odu|coupled)
        printf '$ ./build/leo_sim %s src/odu_input.txt %s.csv\n\n' "$1" "$1"
        build/leo_sim "$1" src/odu_input.txt "$work_dir/$1.csv"
        awk -F';' '
            BEGIN { printf "%-10s %-14s %-14s %s\n", "Time (s)", "Component", "Port", "Value" }
            NR > 2 && $4 != "" { printf "%-10s %-14s %-14s %s\n", $1, $3, $4, $5 }
        ' "$work_dir/$1.csv"
        ;;
    *)
        printf 'Usage: %s blinky|satellite|odu|coupled\n' "$0" >&2
        exit 1
        ;;
esac
