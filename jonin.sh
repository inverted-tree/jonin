#!/bin/sh

# This script runs the jōnin build system

if ! command -v "lua" > /dev/null 2>&1; then
	echo "Error: Lua interpreter not found."
	exit 1
fi

if ! command -v "ninja" > /dev/null 2>&1; then
	echo "Error: Ninja build tool not found."
	exit 1
fi

BUILDOPTS="${1:-./build-options.lua}"

if [[ -f "$BUILDOPTS" ]]; then
	exec "lua" "$BUILDOPTS" "$@"
else
	echo "Error: Build script '$BUILDOPTS' not found."
	exit 1
fi
