#!/bin/bash

set -e


# @see https://stackoverflow.com/a/1482133
DIRECTORY_OF_SCRIPT=`dirname "$(readlink -f "$0")"`

SOURCE_DIRECTORY="${DIRECTORY_OF_SCRIPT}/.."
BUILD_DIRECTORY="${DIRECTORY_OF_SCRIPT}/../build"


if [ -d "${BUILD_DIRECTORY}" ]; then
	rm -rf "${BUILD_DIRECTORY}"
fi
mkdir "${BUILD_DIRECTORY}"

(cd "${BUILD_DIRECTORY}" && cmake -DCMAKE_BUILD_TYPE=Release ..)
make --directory "${BUILD_DIRECTORY}" --silent
make --directory "${BUILD_DIRECTORY}" --silent test

