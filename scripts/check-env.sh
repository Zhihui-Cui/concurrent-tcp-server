#!/usr/bin/env bash
set -euo pipefail
project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$project_dir"
if [[ "$(uname -s)" != Linux ]]; then
    echo "Run this script inside Ubuntu/WSL." >&2
    exit 1
fi
missing=0
for tool in g++ cmake make gdb git python3; do
    if ! command -v "$tool" >/dev/null; then
        echo "Missing tool: $tool"
        missing=1
    fi
done
if (( missing )); then
    echo 'Installing missing tools. Ubuntu may ask for your Linux password.'
    sudo apt-get update
    sudo apt-get install -y build-essential cmake gdb git python3
fi
mkdir -p build
bash dev.sh doctor > build/environment.txt
bash dev.sh build
echo 'Environment verification passed: tools checked and server built. Run client tests separately.'
