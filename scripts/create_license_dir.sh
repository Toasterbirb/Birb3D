#!/bin/sh

[ -z "$1" ] && echo "Usage: ./create_license_dir.sh [OUTPUT_DIRECTORY]" && exit 1

PROJECT_ROOT="$(dirname "$0")/.."
VENDOR_DIR="$PROJECT_ROOT/vendor"
OUTPUT_PATH="$1"

LICENSE_PATHS=$(find "$VENDOR_DIR" -maxdepth 2 -iname "LICENSE*")

# Create directories
mkdir -pv "$OUTPUT_PATH"
for i in $LICENSE_PATHS
do
	DIR_NAME="$(basename "$(dirname "$i")")"
	mkdir -pv "$OUTPUT_PATH/$DIR_NAME"

	LICENSE_FILES="$(find "$VENDOR_DIR/$DIR_NAME" -maxdepth 1 -iname "LICENSE*" -type f)"
	cp -v "$LICENSE_FILES" "$OUTPUT_PATH/$DIR_NAME"
done

# Also copy in the current Birb3D license
mkdir -pv "$OUTPUT_PATH/birb3d"
cp -v "$PROJECT_ROOT/LICENSE" "$OUTPUT_PATH/birb3d"
