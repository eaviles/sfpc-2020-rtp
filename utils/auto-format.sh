#!/usr/bin/env bash

set -e
set -o pipefail

find -E . -regex ".*\.(c|cpp|h|frag)" | xargs clang-format -i
