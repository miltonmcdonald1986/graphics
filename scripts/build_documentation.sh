#!/usr/bin/env bash
set -e

# Optional: detect CI vs local
if [[ "$CI" == "true" ]]; then
    echo "Running in GitHub Actions"
else
    echo "Running locally"
fi

REQUIRED_DOXYGEN="1.14.0"

#Extract the actual version installed
INSTALLED_DOXYGEN="$(doxygen --version 2>/dev/null | awk '{print $1}')"

echo "Required Doxygen:  $REQUIRED_DOXYGEN"
echo "Installed Doxygen: $INSTALLED_DOXYGEN"

#Compare versions
if [[ "$INSTALLED_DOXYGEN" != "REQUIRED_DOXYGEN" ]]; then
	echo "Error: Doxygen $REQUIRED_DOXYGEN is required."
	echo "Found: $INSTALLED_DOXYGEN"
	exit 1
fi

echo "Doxygen version OK."

# Generate docs
doxygen Doxyfile
