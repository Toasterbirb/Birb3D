#!/bin/sh

set -e

EXEC_PATH="$(dirname "$0")"
HEADER_FILE="$EXEC_PATH/ShaderSource.hpp"

cat > "$HEADER_FILE" << "EOF"
#pragma once
#include <unordered_map>

namespace birb
{
	static std::unordered_map<std::string, std::string> shader_src = {
EOF

# Find all shaders in the current directory
SHADER_FILES="$(find . -iname "*.glsl")"
for i in $SHADER_FILES
do
	SHADER_FILE_NAME="$(basename "$i")"
	SHADER_NAME="$(echo "$SHADER_FILE_NAME" | sed 's/.glsl//')"

	{ echo "{ \"$SHADER_NAME\", R\"("; cat "$i"; echo ')" },'; } >> "$HEADER_FILE"
done

printf "\t};\n}" >> "$HEADER_FILE"