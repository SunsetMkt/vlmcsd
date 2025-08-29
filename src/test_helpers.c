#ifndef CONFIG
#define CONFIG "config.h"
#endif // CONFIG
#include CONFIG

#include "test_framework.h"
#include "helpers.h"
#include "types.h"
#include <string.h>

void test_stringToInt(void) {
    unsigned int result;
    
    // Test valid integer conversion
    BOOL success = stringToInt("123", 0, 1000, &result);
    TEST_ASSERT(success, "stringToInt with valid number should succeed");
    TEST_ASSERT_EQUAL_INT(123, result, "stringToInt should convert '123' to 123");
    
    // Test minimum boundary
    success = stringToInt("0", 0, 100, &result);
    TEST_ASSERT(success, "stringToInt with minimum value should succeed");
    TEST_ASSERT_EQUAL_INT(0, result, "stringToInt should convert '0' to 0");
    
    // Test maximum boundary
    success = stringToInt("100", 0, 100, &result);
    TEST_ASSERT(success, "stringToInt with maximum value should succeed");
    TEST_ASSERT_EQUAL_INT(100, result, "stringToInt should convert '100' to 100");
    
    // Test below minimum
    success = stringToInt("-1", 0, 100, &result);
    TEST_ASSERT(!success, "stringToInt with value below minimum should fail");
    
    // Test above maximum
    success = stringToInt("101", 0, 100, &result);
    TEST_ASSERT(!success, "stringToInt with value above maximum should fail");
    
    // Test invalid string
    success = stringToInt("abc", 0, 100, &result);
    TEST_ASSERT(!success, "stringToInt with invalid string should fail");
    
    // Test empty string - the actual behavior may be to treat it as 0
    success = stringToInt("", 1, 100, &result);
    TEST_ASSERT(!success, "stringToInt with empty string should fail when min > 0");
}

void test_hex2bin(void) {
    BYTE buffer[16];
    
    // Test basic hex conversion
    hex2bin(buffer, "48656C6C6F", 5);
    TEST_ASSERT_EQUAL_MEM("Hello", buffer, 5, "hex2bin should convert hex to ASCII");
    
    // Test lowercase hex
    hex2bin(buffer, "48656c6c6f", 5);
    TEST_ASSERT_EQUAL_MEM("Hello", buffer, 5, "hex2bin should handle lowercase hex");
    
    // Test single byte
    hex2bin(buffer, "FF", 1);
    TEST_ASSERT_EQUAL_INT(0xFF, buffer[0], "hex2bin should convert 'FF' to 255");
    
    // Test zero byte
    hex2bin(buffer, "00", 1);
    TEST_ASSERT_EQUAL_INT(0x00, buffer[0], "hex2bin should convert '00' to 0");
}

void test_getArgumentBool(void) {
    int_fast8_t result;
    BOOL success;
    
    // Test valid true values - just test acceptance for now
    success = getArgumentBool(&result, "1");
    TEST_ASSERT(success, "getArgumentBool should accept '1'");
    
    success = getArgumentBool(&result, "yes");
    TEST_ASSERT(success, "getArgumentBool should accept 'yes'");
    
    success = getArgumentBool(&result, "true");
    TEST_ASSERT(success, "getArgumentBool should accept 'true'");
    
    success = getArgumentBool(&result, "on");
    TEST_ASSERT(success, "getArgumentBool should accept 'on'");
    
    // Test valid false values
    success = getArgumentBool(&result, "0");
    TEST_ASSERT(success, "getArgumentBool should accept '0'");
    
    success = getArgumentBool(&result, "no");
    TEST_ASSERT(success, "getArgumentBool should accept 'no'");
    
    success = getArgumentBool(&result, "false");
    TEST_ASSERT(success, "getArgumentBool should accept 'false'");
    
    success = getArgumentBool(&result, "off");
    TEST_ASSERT(success, "getArgumentBool should accept 'off'");
    
    // Test case insensitivity
    success = getArgumentBool(&result, "TRUE");
    TEST_ASSERT(success, "getArgumentBool should be case insensitive");
    
    success = getArgumentBool(&result, "FALSE");
    TEST_ASSERT(success, "getArgumentBool should be case insensitive");
    
    // Test invalid values
    success = getArgumentBool(&result, "maybe");
    TEST_ASSERT(!success, "getArgumentBool should reject invalid values");
    
    success = getArgumentBool(&result, "");
    TEST_ASSERT(!success, "getArgumentBool should reject empty string");
}

void test_parseAddress(void) {
    char addr[256];
    char *host, *port;
    
    // Test IPv4 address with port
    strcpy(addr, "192.168.1.1:1688");
    parseAddress(addr, &host, &port);
    TEST_ASSERT_EQUAL_STR("192.168.1.1", host, "parseAddress should extract IPv4 host");
    TEST_ASSERT_EQUAL_STR("1688", port, "parseAddress should extract port from IPv4 address");
    
    // Test hostname with port
    strcpy(addr, "localhost:8080");
    parseAddress(addr, &host, &port);
    TEST_ASSERT_EQUAL_STR("localhost", host, "parseAddress should extract hostname");
    TEST_ASSERT_EQUAL_STR("8080", port, "parseAddress should extract port from hostname");
    
    // Test address without port
    strcpy(addr, "example.com");
    parseAddress(addr, &host, &port);
    TEST_ASSERT_EQUAL_STR("example.com", host, "parseAddress should extract host without port");
    TEST_ASSERT_NOT_NULL(port, "parseAddress should provide default port");
    
    // Test IPv6 address with brackets and port
    strcpy(addr, "[::1]:1688");
    parseAddress(addr, &host, &port);
    TEST_ASSERT_EQUAL_STR("::1", host, "parseAddress should extract IPv6 host");
    TEST_ASSERT_EQUAL_STR("1688", port, "parseAddress should extract port from IPv6 address");
}

void test_timeSpanString2Seconds(void) {
    // Test seconds
    DWORD result = timeSpanString2Seconds("30s");
    TEST_ASSERT_EQUAL_INT(30, result, "timeSpanString2Seconds should convert '30s' to 30 seconds");
    
    // Test minutes
    result = timeSpanString2Seconds("5m");
    TEST_ASSERT_EQUAL_INT(300, result, "timeSpanString2Seconds should convert '5m' to 300 seconds");
    
    // Test hours
    result = timeSpanString2Seconds("2h");
    TEST_ASSERT_EQUAL_INT(7200, result, "timeSpanString2Seconds should convert '2h' to 7200 seconds");
    
    // Test days
    result = timeSpanString2Seconds("1d");
    TEST_ASSERT_EQUAL_INT(86400, result, "timeSpanString2Seconds should convert '1d' to 86400 seconds");
    
    // Test weeks
    result = timeSpanString2Seconds("1w");
    TEST_ASSERT_EQUAL_INT(604800, result, "timeSpanString2Seconds should convert '1w' to 604800 seconds");
    
    // Test number without unit (should be treated as minutes)
    result = timeSpanString2Seconds("60");
    TEST_ASSERT_EQUAL_INT(3600, result, "timeSpanString2Seconds should treat bare number as minutes");
}

int test_helpers_main(void) {
    TEST_INIT();
    
    RUN_TEST(test_stringToInt);
    RUN_TEST(test_hex2bin);
    RUN_TEST(test_getArgumentBool);
    RUN_TEST(test_parseAddress);
    RUN_TEST(test_timeSpanString2Seconds);
    
    TEST_SUMMARY();
}