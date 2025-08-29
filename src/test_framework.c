#include "test_framework.h"

/* Test framework globals */
int test_count = 0;
int test_passed = 0;
int test_failed = 0;

void test_framework_init(void) {
    test_count = 0;
    test_passed = 0;
    test_failed = 0;
}