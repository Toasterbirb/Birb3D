#!/bin/sh

set -e

EXEC_PATH="$(dirname "$0")"
HEADER_FILE="$EXEC_PATH/ShaderSource.hpp"

shader_preprocessor() {
	SRC="$(cat)"

	IFS=$(printf "\n\b")
	for i in $SRC
	do
		# Replace includes
		if echo "$i" | grep -q '#include ".*"'
		then
			INCLUDE_PATH="$(echo "$i" | sed 's/#include //' | tr -d '"')"

			if [ ! -f "$INCLUDE_PATH" ]
			then
				echo "ERROR: Broken include path in $SHADER_FILE_NAME: $i" 1>&2
				echo "PWD: $(pwd)" 1>&2
				exit 1
			fi

			# Pass the include file to the preprocessor
			shader_preprocessor < "./$INCLUDE_PATH"
			continue
		fi

		# Remove some comments and unnecessary whitespace
		LINE="$(echo "$i" | sed 's|//.*||g; s|/\*[A-Za-z _.-]*\*/||g; s/[[:space:]]*$//g; s/^[[:space:]]*//; s/  / /g; s/[[:space:]]\t/ /g; s/, /,/g; s/ ,/,/g; s/ = /=/g; s/ \* /\*/g; s/ + /+/g; s| / |/|g; s/ - /-/g')"

		# Don't print the line if its empty
		[ -z "$LINE" ] && continue

		echo "$LINE"
	done
}

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

	{ echo "{ \"$SHADER_NAME\", R\"("; shader_preprocessor < "$i"; echo ')" },'; } >> "$HEADER_FILE"

	if echo "$SHADER_NAME" | grep -q "_frag$"
	then
		SHADER_FRAG_NAME_LIST="$SHADER_FRAG_NAME_LIST\"$(echo "$SHADER_NAME" | sed 's/_frag$//')\","
	else
		SHADER_VERT_NAME_LIST="$SHADER_VERT_NAME_LIST\"$(echo "$SHADER_NAME" | sed 's/_vert$//')\","
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
