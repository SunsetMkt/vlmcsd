# Unit Testing for VLMCSD

This repository now includes a comprehensive unit testing framework for testing the core modules of the VLMCSD (Volume License Management Service Daemon).

## Running Tests

To run the unit tests, use the following command from the root directory:

```bash
make test
```

Or alternatively:

```bash
make check
```

## Test Framework

The unit testing framework consists of several components:

### Framework Files
- `src/test_framework.h` - Lightweight unit testing framework with assertion macros
- `src/test_framework.c` - Framework implementation
- `src/test_runner.c` - Main test runner that executes all test suites

### Test Modules
- `src/test_endian.c` - Tests for byte order and endianness functions
- `src/test_helpers.c` - Tests for utility and helper functions  
- `src/test_crypto.c` - Tests for cryptographic functions

## Test Coverage

The unit tests cover the following core modules:

### Endian Functions (`endian.c`)
- Byte swap operations (16-bit, 32-bit, 64-bit)
- Little/big endian conversions
- Unaligned memory access functions
- Endian consistency checks

### Helper Functions (`helpers.c`)
- String to integer conversion
- Hexadecimal to binary conversion
- Boolean argument parsing
- Network address parsing
- Time span string parsing

### Crypto Functions (`crypto.c`)
- XOR block operations
- AES key initialization
- AES encryption/decryption
- Known test vector validation
- Built-in key verification

## Test Output

The test runner provides detailed output showing:
- Individual test results (PASS/FAIL)
- Expected vs actual values for failed tests
- Summary statistics for each test suite
- Overall test summary

Example output:
```
=== VLMCSD Unit Test Runner ===

Running test suite: Endian Functions
----------------------------------------
  PASS: BS16 byte swap test
  PASS: BS32 byte swap test
  ...

Test Summary:
  Total tests: 17
  Passed: 17
  Failed: 0
  Result: ALL TESTS PASSED
```

## Building Tests

The tests are automatically built when running `make test`. The build process:

1. Builds the main vlmcsd binaries
2. Compiles the test framework and test modules
3. Links everything into a test runner executable
4. Executes the tests

## Integration with CI/CD

The test framework is designed to be CI/CD friendly:
- Returns appropriate exit codes (0 for success, 1 for failures)
- Provides clear pass/fail output
- Can be run automatically as part of build pipelines

## Extending Tests

To add new tests:

1. Create a new test file (e.g., `src/test_mymodule.c`)
2. Include the test framework header: `#include "test_framework.h"`
3. Write test functions using the provided assertion macros
4. Add the test module to the test runner in `src/test_runner.c`
5. Update the Makefile to build the new test module

## Available Assertion Macros

- `TEST_ASSERT(condition, message)` - Assert a condition is true
- `TEST_ASSERT_EQUAL_INT(expected, actual, message)` - Assert integers are equal
- `TEST_ASSERT_EQUAL_STR(expected, actual, message)` - Assert strings are equal
- `TEST_ASSERT_EQUAL_MEM(expected, actual, size, message)` - Assert memory blocks are equal
- `TEST_ASSERT_NOT_NULL(ptr, message)` - Assert pointer is not NULL

## Framework Macros

- `TEST_INIT()` - Initialize test framework for a test suite
- `RUN_TEST(test_func)` - Run a specific test function
- `TEST_SUMMARY()` - Print summary and return appropriate exit code