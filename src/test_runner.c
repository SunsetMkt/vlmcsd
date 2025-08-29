#ifndef CONFIG
#define CONFIG "config.h"
#endif // CONFIG
#include CONFIG

#include <stdio.h>
#include <stdlib.h>

// Function declarations for test suites
extern int test_endian_main(void);
extern int test_helpers_main(void);
extern int test_crypto_main(void);

typedef struct {
    const char* name;
    int (*test_func)(void);
} TestSuite;

// Test suite registry
static TestSuite test_suites[] = {
    {"Endian Functions", test_endian_main},
    {"Helper Functions", test_helpers_main}, 
    {"Crypto Functions", test_crypto_main},
    {NULL, NULL} // sentinel
};

int main(int argc, char* argv[]) {
    int total_failed = 0;
    int suite_count = 0;
    int passed_suites = 0;
    
    printf("=== VLMCSD Unit Test Runner ===\n\n");
    
    // Run all test suites
    for (int i = 0; test_suites[i].name != NULL; i++) {
        suite_count++;
        printf("Running test suite: %s\n", test_suites[i].name);
        printf("----------------------------------------\n");
        
        int result = test_suites[i].test_func();
        
        if (result == 0) {
            printf("✓ Test suite PASSED\n\n");
            passed_suites++;
        } else {
            printf("✗ Test suite FAILED\n\n");
            total_failed++;
        }
    }
    
    // Print final summary
    printf("========================================\n");
    printf("Final Test Summary:\n");
    printf("  Total test suites: %d\n", suite_count);
    printf("  Passed: %d\n", passed_suites);
    printf("  Failed: %d\n", total_failed);
    
    if (total_failed == 0) {
        printf("  Result: ALL TEST SUITES PASSED ✓\n");
        return 0;
    } else {
        printf("  Result: %d TEST SUITES FAILED ✗\n", total_failed);
        return 1;
    }
}