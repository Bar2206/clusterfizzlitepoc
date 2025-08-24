# ClusterFuzzLite Demo Repository

This repository demonstrates how to integrate **ClusterFuzzLite** into a C++ project for automated fuzz testing in CI/CD pipelines using GitHub Actions.

## What is ClusterFuzzLite?

[ClusterFuzzLite](https://google.github.io/clusterfuzzlite/) is a simple, lightweight version of [ClusterFuzz](https://google.github.io/clusterfuzz/) that can run in CI/CD environments like GitHub Actions. It provides:

- **Continuous Fuzzing**: Automatically finds bugs by testing your code with millions of generated inputs
- **Multiple Sanitizers**: Detects memory errors, undefined behavior, and other issues using AddressSanitizer, MemorySanitizer, and UBSan
- **Pull Request Integration**: Tests every code change before it's merged
- **Easy Integration**: Works with existing Docker-based build systems
- **OSS-Fuzz Compatible**: Uses the same battle-tested infrastructure as OSS-Fuzz

## What This Demo Shows

This repository contains a minimal C++ project that demonstrates ClusterFuzzLite integration:

- **Simple C++ Library** (`src/lib/`): A tiny library with number parsing functionality
- **Fuzz Target** (`src/fuzz/`): A LibFuzzer target that tests the library functions
- **Docker Build** (`Dockerfile`, `build.sh`): ClusterFuzzLite-compatible build system
- **GitHub Actions** (`.github/workflows/cflite_pr.yml`): Automated fuzzing on pull requests

## Repository Structure

```
.
├── .github/
│   └── workflows/
│       └── cflite_pr.yml      # GitHub Actions workflow for PR fuzzing
├── src/
│   ├── lib/
│   │   ├── tiny_lib.h         # Library header
│   │   └── tiny_lib.cpp       # Library implementation
│   └── fuzz/
│       └── tiny_fuzz_target.cpp # LibFuzzer target
├── build.sh                   # Build script for ClusterFuzzLite
├── Dockerfile                 # Docker configuration for fuzzing
├── README.md                  # This file
└── LICENSE                    # MIT License
```

## How to Use This Demo

### 1. Fork and Test

1. **Fork this repository** to your own GitHub account
2. **Enable GitHub Actions** in your fork (should be enabled by default)
3. **Create a pull request** with any small change (e.g., update this README)
4. **Watch the workflow run** - check the "Actions" tab to see ClusterFuzzLite in action

### 2. Understanding the Workflow

The GitHub Actions workflow (`.github/workflows/cflite_pr.yml`) runs two main steps:

1. **Build Fuzzers**: Compiles the fuzz targets using the `Dockerfile` and `build.sh`
2. **Run Fuzzers**: Executes the fuzzers for 2 minutes to find potential issues

If crashes are found, they'll be uploaded as artifacts and a comment will be posted on the PR.

### 3. Local Testing (Optional)

You can test the Docker build locally:

```bash
# Build the Docker image
docker build -t clusterfuzzlite-demo .

# Run the container to build fuzzers
docker run --rm -it clusterfuzzlite-demo /bin/bash -c "
  export OUT=/tmp/out
  export CC=clang
  export CXX=clang++
  export CFLAGS='-fsanitize=address -fsanitize-address-use-after-scope'
  export CXXFLAGS='-fsanitize=address -fsanitize-address-use-after-scope'
  export LIB_FUZZING_ENGINE='-fsanitize=fuzzer'
  mkdir -p \$OUT
  ./build.sh
  ls -la \$OUT/
"
```

## Key Components Explained

### TinyLib Library (`src/lib/`)

A minimal C++ library that provides:
- `parse_number()`: Parses strings to integers with error handling
- `is_valid_ascii()`: Validates ASCII strings
- `process_data()`: Processes raw byte data

### Fuzz Target (`src/fuzz/tiny_fuzz_target.cpp`)

The LibFuzzer target that:
- Tests all library functions with random inputs
- Exercises different code paths and edge cases
- Uses the standard `LLVMFuzzerTestOneInput` interface

### Build Configuration

- **`Dockerfile`**: Based on `gcr.io/oss-fuzz-base/base-builder` image
- **`build.sh`**: Compiles the library and links the fuzzer with LibFuzzer and AddressSanitizer

### GitHub Actions Workflow

- **Triggers**: On pull requests and manual dispatch
- **Build**: Uses `google/clusterfuzzlite/actions/build_fuzzers@v1`
- **Run**: Uses `google/clusterfuzzlite/actions/run_fuzzers@v1`
- **Artifacts**: Uploads crash reproducers and logs
- **Reporting**: Comments on PRs when issues are found

## Customizing for Your Project

To adapt this demo for your own project:

1. **Replace the library code** in `src/lib/` with your actual code
2. **Update the fuzz target** in `src/fuzz/` to test your functions
3. **Modify `build.sh`** to compile your project correctly
4. **Adjust the `Dockerfile`** if you need additional dependencies
5. **Customize the workflow** timing and parameters in `cflite_pr.yml`

## Resources

- **ClusterFuzzLite Documentation**: https://google.github.io/clusterfuzzlite/
- **Build Integration Guide**: https://google.github.io/clusterfuzzlite/build-integration/
- **GitHub Actions Guide**: https://google.github.io/clusterfuzzlite/running-clusterfuzzlite/github-actions/
- **LibFuzzer Documentation**: https://llvm.org/docs/LibFuzzer.html
- **OSS-Fuzz**: https://github.com/google/oss-fuzz

## Benefits of Continuous Fuzzing

- **Early Bug Detection**: Find issues before they reach production
- **Automated Testing**: No manual intervention required
- **Comprehensive Coverage**: Tests edge cases humans might miss
- **Security Focus**: Detects memory corruption and security vulnerabilities
- **CI/CD Integration**: Fits naturally into existing development workflows

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

**Happy Fuzzing!** 🐛🔍

For questions or issues with this demo, please create a GitHub issue.
Update