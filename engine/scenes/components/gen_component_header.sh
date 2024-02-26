#!/bin/sh

#########################################################
# Generate a header that includes all component headers #
#########################################################

EXEC_PATH="$(dirname "$0")"
COMPONENT_HEADER_DIR="$EXEC_PATH/include"

# Print the header file contents
cd "$COMPONENT_HEADER_DIR" || exit 1

echo "#pragma once"

FILES="$(ls)"
for header in $FILES
do
	echo "#include \"${header}\""
done
