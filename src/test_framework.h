#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test framework globals */
extern int test_count;
extern int test_passed;
extern int test_failed;

/* Test framework macros */
#define TEST_INIT() do { \
    test_count = 0; \
    test_passed = 0; \
    test_failed = 0; \
    printf("Starting test suite...\n"); \
} while(0)

#define TEST_ASSERT(condition, message) do { \
    test_count++; \
    if (condition) { \
        test_passed++; \
        printf("  PASS: %s\n", message); \
    } else { \
        test_failed++; \
        printf("  FAIL: %s\n", message); \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual, message) do { \
    test_count++; \
    if ((expected) == (actual)) { \
        test_passed++; \
        printf("  PASS: %s (expected: %d, actual: %d)\n", message, expected, actual); \
    } else { \
        test_failed++; \
        printf("  FAIL: %s (expected: %d, actual: %d)\n", message, expected, actual); \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_STR(expected, actual, message) do { \
    test_count++; \
    if (strcmp((expected), (actual)) == 0) { \
        test_passed++; \
        printf("  PASS: %s\n", message); \
    } else { \
        test_failed++; \
        printf("  FAIL: %s (expected: '%s', actual: '%s')\n", message, expected, actual); \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_MEM(expected, actual, size, message) do { \
    test_count++; \
    if (memcmp((expected), (actual), (size)) == 0) { \
        test_passed++; \
        printf("  PASS: %s\n", message); \
    } else { \
        test_failed++; \
        printf("  FAIL: %s (memory comparison failed)\n", message); \
    } \
} while(0)

#define TEST_ASSERT_NOT_NULL(ptr, message) do { \
    test_count++; \
    if ((ptr) != NULL) { \
        test_passed++; \
        printf("  PASS: %s\n", message); \
    } else { \
        test_failed++; \
        printf("  FAIL: %s (pointer is NULL)\n", message); \
    } \
} while(0)

#define TEST_SUMMARY() do { \
    printf("\nTest Summary:\n"); \
    printf("  Total tests: %d\n", test_count); \
    printf("  Passed: %d\n", test_passed); \
    printf("  Failed: %d\n", test_failed); \
    if (test_failed == 0) { \
        printf("  Result: ALL TESTS PASSED\n"); \
    } else { \
        printf("  Result: %d TESTS FAILED\n", test_failed); \
    } \
    return (test_failed == 0) ? 0 : 1; \
} while(0)

#define RUN_TEST(test_func) do { \
    printf("\nRunning %s...\n", #test_func); \
    test_func(); \
} while(0)

/* Test framework function declarations */
void test_framework_init(void);

#endif /* TEST_FRAMEWORK_H */