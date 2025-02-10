#!/bin/sh

# This script runs the jōgetsu build system

if ! command -v "lua" > /dev/null 2>&1; then
	echo "Error: Lua interpreter not found"
	exit 1
fi

exec "lua" "src/main.lua" "$@"
