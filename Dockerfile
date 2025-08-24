# ClusterFuzzLite Dockerfile for TinyLib Demo
# Based on the OSS-Fuzz base-builder image which provides:
# - Clang/LLVM with fuzzing support
# - AddressSanitizer, MemorySanitizer, UBSan
# - libFuzzer integration
# - Build environment variables

FROM gcr.io/oss-fuzz-base/base-builder

# Set maintainer information
LABEL maintainer="ClusterFuzzLite Demo"
LABEL description="Minimal C++ project demonstrating ClusterFuzzLite integration"

# For this minimal demo, the base image has everything we need:
# - clang++ (C++ compiler with fuzzing support)
# - Standard C++ libraries
# - Build tools and sanitizers
# - libFuzzer integration
# No additional packages required!

# Set the working directory to /src as expected by ClusterFuzzLite
WORKDIR /src

# Copy the entire project into the container
# ClusterFuzzLite expects the source code to be in /src
COPY . /src/

# Ensure build script is executable (belt and suspenders approach)
RUN chmod +x /src/build.sh

# Set default sanitizer to AddressSanitizer
# This can be overridden by ClusterFuzzLite at build time
ENV SANITIZER=address

# Verify our source structure
RUN ls -la /src && \
    ls -la /src/src/lib && \
    ls -la /src/src/fuzz

# The ClusterFuzzLite system will:
# 1. Mount this container
# 2. Set environment variables like $OUT, $CXX, $CXXFLAGS, $LIB_FUZZING_ENGINE
# 3. Execute ./build.sh to build the fuzzers
# 4. Expect fuzzer binaries to be placed in $OUT directory

# No CMD or ENTRYPOINT needed - ClusterFuzzLite will run the build.sh script
