#!/bin/sh

set -e

EXEC_PATH="$(dirname "$0")"
HEADER_FILE="$EXEC_PATH/PrimitiveMeshes.hpp"

cat > "$HEADER_FILE" << "EOF"
#pragma once
#include <unordered_map>
#include <vector>

namespace birb
{
	enum class primitive_mesh;
	static std::unordered_map<primitive_mesh, std::vector<unsigned char>> primitive_mesh_data = {
EOF

# Create an enum that maps to the different meshes
MESH_ENUM="enum class primitive_mesh
{
"

COUNTER="0"
for obj_file in *.obj
do
	xxd -i "$obj_file" | sed 's/};/}},/' | sed "s/{/{ primitive_mesh($COUNTER), {/g" >> "$HEADER_FILE"

	filename="$(echo "$obj_file" | cut -d '.' -f1)"
	MESH_ENUM="$MESH_ENUM $filename = $COUNTER,
"
	COUNTER="$(echo "$COUNTER+1" | bc)"
done
echo "};" >> "$HEADER_FILE"

sed -i "s/^unsigned char.* = //" "$HEADER_FILE"
sed -i "/^unsigned int/d" "$HEADER_FILE"
sed -i "s/\[\]//" "$HEADER_FILE"


# Close the primitive_mesh enum
MESH_ENUM="$MESH_ENUM
};"

# Add the primitive_mesh enum to the header
echo "$MESH_ENUM" >> "$HEADER_FILE"

# Close the birb namespace
echo "}" >> "$HEADER_FILE"
