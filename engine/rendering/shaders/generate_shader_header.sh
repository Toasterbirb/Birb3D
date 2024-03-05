#!/bin/sh

set -e

EXEC_PATH="$(dirname "$0")"
HEADER_FILE="$EXEC_PATH/ShaderSource.hpp"

cat > "$HEADER_FILE" << "EOF"
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace birb
{
	static std::unordered_map<std::string, std::string> shader_src = {
EOF

# Find all shaders in the current directory
SHADER_FRAG_NAME_LIST=""
SHADER_VERT_NAME_LIST=""
SHADER_FILES="$(find . -iname "*.glsl")"
for i in $SHADER_FILES
do
	SHADER_FILE_NAME="$(basename "$i")"
	SHADER_NAME="$(echo "$SHADER_FILE_NAME" | sed 's/.glsl//')"

	{ echo "{ \"$SHADER_NAME\", R\"("; cat "$i"; echo ')" },'; } >> "$HEADER_FILE"

	if echo "$SHADER_NAME" | grep -q "_frag$"
	then
		SHADER_FRAG_NAME_LIST="$SHADER_FRAG_NAME_LIST\"$SHADER_NAME\","
	else
		SHADER_VERT_NAME_LIST="$SHADER_VERT_NAME_LIST\"$SHADER_NAME\","
	fi
done

printf "\t};\n" >> "$HEADER_FILE"

# Create a std::vector that contains the list of shader names
SHADER_FRAG_NAME_LIST="$(echo "$SHADER_FRAG_NAME_LIST" | sed 's/[[:space:]]$//')"
printf "const static std::vector<std::string> shader_src_frag_names = {%s};\n" "$SHADER_FRAG_NAME_LIST" >> "$HEADER_FILE"

SHADER_VERT_NAME_LIST="$(echo "$SHADER_VERT_NAME_LIST" | sed 's/[[:space:]]$//')"
printf "const static std::vector<std::string> shader_src_vert_names = {%s};\n" "$SHADER_VERT_NAME_LIST" >> "$HEADER_FILE"

# Add the closing bracket for the namespace
printf "\n}" >> "$HEADER_FILE"
