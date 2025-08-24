#!/bin/bash -eu

# Build script for ClusterFuzzLite
# This script is called by the ClusterFuzzLite Docker container to build fuzzers.

# Exit on any error
set -euxo pipefail

# Ensure we're in the source directory
cd $SRC/clusterfizzlitepoc
echo "Current directory: $(pwd)"

# Set compiler flags for fuzzing and AddressSanitizer
# These environment variables are provided by the ClusterFuzzLite base image:
# - $CXX: C++ compiler (usually clang++)
# - $CXXFLAGS: C++ compilation flags (includes -fsanitize=address)
# - $LIB_FUZZING_ENGINE: Path to libFuzzer library
# - $OUT: Output directory for fuzzer binaries

# Additional flags for our build
FUZZ_CXXFLAGS="$CXXFLAGS -std=c++17 -I$SRC/clusterfizzlitepoc/src/lib"

echo "=== Building TinyLib library ==="
# Compile the library
$CXX $FUZZ_CXXFLAGS -c src/lib/tiny_lib.cpp -o tiny_lib.o

echo "=== Building fuzz target ==="
# Compile the fuzz target
$CXX $FUZZ_CXXFLAGS -c src/fuzz/tiny_fuzz_target.cpp -o tiny_fuzz_target.o

echo "=== Linking fuzzer binary ==="
# Link the fuzzer with libFuzzer
$CXX $FUZZ_CXXFLAGS tiny_fuzz_target.o tiny_lib.o $LIB_FUZZING_ENGINE -o $OUT/tiny_fuzz_target

echo "=== Creating seed corpus directory (optional) ==="
# Create a minimal seed corpus to help fuzzing start efficiently
# This is optional but recommended for better fuzzing coverage
mkdir -p $OUT/tiny_fuzz_target_seed_corpus

# Add some basic seed inputs to help the fuzzer
echo "123" > $OUT/tiny_fuzz_target_seed_corpus/number1
echo "-456" > $OUT/tiny_fuzz_target_seed_corpus/number2
echo "" > $OUT/tiny_fuzz_target_seed_corpus/empty
echo "not_a_number" > $OUT/tiny_fuzz_target_seed_corpus/invalid
echo "999999999999999999999" > $OUT/tiny_fuzz_target_seed_corpus/overflow
printf "\x80\x81\x82\x83" > $OUT/tiny_fuzz_target_seed_corpus/non_ascii

echo "=== Build completed successfully ==="
echo "Fuzzer binary: $OUT/tiny_fuzz_target"
echo "Seed corpus: $OUT/tiny_fuzz_target_seed_corpus"

# List the output for verification
ls -la $OUT/
