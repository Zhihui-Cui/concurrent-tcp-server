#!/usr/bin/env bash
set -euo pipefail
project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

if [[ "$(uname -s)" != Linux ]]; then
    echo "Run this script inside Ubuntu/WSL, not Git Bash or PowerShell." >&2
    exit 1
fi

build_project() {
    cmake -S "$project_dir" -B "$project_dir/build" \
        -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cmake --build "$project_dir/build" --parallel 2
}

case "${1:-help}" in
    build)
        build_project
        ;;
    run)
        build_project
        shift
        exec "$project_dir/build/tcp_server" "$@"
        ;;
    debug)
        build_project
        shift
        exec gdb --args "$project_dir/build/tcp_server" "$@"
        ;;
    doctor)
        for tool in g++ cmake make gdb git python3; do
            command -v "$tool" >/dev/null || {
                echo "Missing tool: $tool" >&2
                exit 1
            }
        done
        uname -sr
        g++ --version
        cmake --version
        gdb --version
        git --version
        python3 --version
        ;;
    help|-h|--help)
        echo "Usage: bash dev.sh {build|run|debug|doctor}"
        ;;
    *)
        echo "Unknown command: $1" >&2
        echo "Usage: bash dev.sh {build|run|debug|doctor}" >&2
        exit 2
        ;;
esac
