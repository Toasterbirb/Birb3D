#!/bin/sh

############################################################################
# Make sure that for every event register call, there's an unregister call #
############################################################################

PROJECT_ROOT="$(dirname "$0")/.."

# Find all files that call event_bus::register_event_id
REGISTER_FILES="$(grep -rl "event_bus::register_event_id([0-9]*, this);" "$PROJECT_ROOT/engine" "$PROJECT_ROOT/tests")"

FOUND_MISSING_UNREGISTER_CALLS="false"

IFS=$(printf "\n\b")
for file in $REGISTER_FILES
do
	REGISTER_CALLS_IN_FILE="$(grep -o "event_bus::register_event_id([0-9]*, this);" "$file")"
	for reg_call in $REGISTER_CALLS_IN_FILE
	do
		ID="$(echo "$reg_call" | grep -o "[0-9]*")"
		if ! grep -oq "^[[:space:]]*\(birb::\)\?event_bus::unregister_event_id($ID, this);" "$file"
		then
			printf "\033[31mMissing call to event_bus::unregister_event_id() at %s\033[0m\n" "$file"
			FOUND_MISSING_UNREGISTER_CALLS="true"
		fi
	done
done

if [ "$FOUND_MISSING_UNREGISTER_CALLS" = "true" ]
then
	exit 1
fi
