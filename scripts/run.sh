#!/bin/bash

#!/bin/bash
set -e
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
INCLUDE="-I${ROOT_DIR}/include"
SRC="${ROOT_DIR}/src"
TESTS="${ROOT_DIR}/tests"

# Compile the source files for correctness tests
g++ -std=c++17 -O2 "${INCLUDE}" \
  "${SRC}/Chaining.cpp" "${SRC}/LinearProbing.cpp" "${SRC}/QuadraticProbing.cpp" \
  "${SRC}/CubicProbing.cpp" "${SRC}/Comp.cpp" "${SRC}/DoubleHashing.cpp" \
  "${TESTS}/test.cpp" -o "${ROOT_DIR}/test"

# Run the compiled program
"${ROOT_DIR}/test"

# CLI harness for file-driven testcases
g++ -std=c++17 -O2 "${INCLUDE}" \
  "${SRC}/Chaining.cpp" "${SRC}/LinearProbing.cpp" "${SRC}/QuadraticProbing.cpp" \
  "${SRC}/CubicProbing.cpp" "${SRC}/Comp.cpp" "${SRC}/DoubleHashing.cpp" \
  "${TESTS}/cli.cpp" -o "${ROOT_DIR}/cli"

CLI_IN="${ROOT_DIR}/tests/data/test2/in.txt"
CLI_OUT="${ROOT_DIR}/tests/data/test2/out.txt"
GEN_OUT="${ROOT_DIR}/tests/data/test2/out.generated.txt"

if [ -f "${CLI_IN}" ]; then
  "${ROOT_DIR}/cli" "${CLI_IN}" > "${GEN_OUT}"
  if diff -u "${CLI_OUT}" "${GEN_OUT}"; then
    echo "CLI testcase match."
  else
    echo "CLI testcase mismatch."
  fi
fi

# Clean up compiled executables and generated outputs
rm -f "${ROOT_DIR}/test" "${ROOT_DIR}/cli" "${GEN_OUT}"
