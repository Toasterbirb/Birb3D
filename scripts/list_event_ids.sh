#!/bin/sh

##########################################
# Find and list all registered event IDs #
##########################################

PROJECT_ROOT="$(dirname "$0")/.."

# Scan the engine directory
EVENT_LIST="$(grep -r "event_bus::register_event_id([0-9]*, this); //" "$PROJECT_ROOT/engine" "$PROJECT_ROOT/editor" "$PROJECT_ROOT/tests")"

IFS=$(printf "\n\b")
for i in $EVENT_LIST
do
	ID="$(echo "$i" | grep -o "event_bus::register_event_id([0-9]*, this);" | grep -o "[0-9]*")"
	COMMENT="$(echo "$i" | grep -o "//.*$")"
	printf "%s\t%s\n" "$ID" "$COMMENT"
done
