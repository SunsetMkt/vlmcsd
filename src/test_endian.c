#ifndef CONFIG
#define CONFIG "config.h"
#endif // CONFIG
#include CONFIG

#include "test_framework.h"
#include "types.h"
#include "endian.h"
#include <stdint.h>

void test_byte_swap_functions(void) {
    // Test 16-bit byte swap
    uint16_t val16 = 0x1234;
    uint16_t expected16 = 0x3412;
    uint16_t result16 = BS16(val16);
    TEST_ASSERT_EQUAL_INT(expected16, result16, "BS16 byte swap test");

    // Test 32-bit byte swap
    uint32_t val32 = 0x12345678;
    uint32_t expected32 = 0x78563412;
    uint32_t result32 = BS32(val32);
    TEST_ASSERT_EQUAL_INT(expected32, result32, "BS32 byte swap test");

    // Test 64-bit byte swap
    uint64_t val64 = 0x123456789ABCDEF0ULL;
    uint64_t expected64 = 0xF0DEBC9A78563412ULL;
    uint64_t result64 = BS64(val64);
    TEST_ASSERT(result64 == expected64, "BS64 byte swap test");
}

void test_endian_conversions(void) {
    // Test little endian conversions
    uint16_t val16 = 0x1234;
    uint32_t val32 = 0x12345678;
    uint64_t val64 = 0x123456789ABCDEF0ULL;

    // These should be platform-dependent, but we can test round-trip
    uint16_t le16 = LE16(val16);
    uint16_t be16 = BE16(val16);
    TEST_ASSERT_EQUAL_INT(val16, LE16(le16), "LE16 round-trip test");
    TEST_ASSERT_EQUAL_INT(val16, BE16(be16), "BE16 round-trip test");

    uint32_t le32 = LE32(val32);
    uint32_t be32 = BE32(val32);
    TEST_ASSERT_EQUAL_INT(val32, LE32(le32), "LE32 round-trip test");
    TEST_ASSERT_EQUAL_INT(val32, BE32(be32), "BE32 round-trip test");

    uint64_t le64 = LE64(val64);
    uint64_t be64 = BE64(val64);
    TEST_ASSERT(val64 == LE64(le64), "LE64 round-trip test");
    TEST_ASSERT(val64 == BE64(be64), "BE64 round-trip test");
}

void test_unaligned_access(void) {
    // Test unaligned access functions
    uint8_t buffer[16];
    memset(buffer, 0, sizeof(buffer));

    uint16_t test_val16 = 0x1234;
    uint32_t test_val32 = 0x12345678;
    uint64_t test_val64 = 0x123456789ABCDEF0ULL;

    // Test little endian unaligned writes and reads
    PUT_UA16LE(buffer, test_val16);
    uint16_t read_val16 = GET_UA16LE(buffer);
    TEST_ASSERT_EQUAL_INT(test_val16, read_val16, "UA16LE write/read test");

    PUT_UA32LE(buffer, test_val32);
    uint32_t read_val32 = GET_UA32LE(buffer);
    TEST_ASSERT_EQUAL_INT(test_val32, read_val32, "UA32LE write/read test");

    PUT_UA64LE(buffer, test_val64);
    uint64_t read_val64 = GET_UA64LE(buffer);
    TEST_ASSERT(test_val64 == read_val64, "UA64LE write/read test");

    // Test big endian unaligned writes and reads
    memset(buffer, 0, sizeof(buffer));
    
    PUT_UA16BE(buffer, test_val16);
    read_val16 = GET_UA16BE(buffer);
    TEST_ASSERT_EQUAL_INT(test_val16, read_val16, "UA16BE write/read test");

    PUT_UA32BE(buffer, test_val32);
    read_val32 = GET_UA32BE(buffer);
    TEST_ASSERT_EQUAL_INT(test_val32, read_val32, "UA32BE write/read test");

    PUT_UA64BE(buffer, test_val64);
    read_val64 = GET_UA64BE(buffer);
    TEST_ASSERT(test_val64 == read_val64, "UA64BE write/read test");
}

void test_endian_consistency(void) {
    // Test that little endian and big endian are inverses
    uint32_t val = 0x12345678;
    
    // On little endian systems: LE32(val) == val, BE32(val) == BS32(val)
    // On big endian systems: BE32(val) == val, LE32(val) == BS32(val)
    
    uint32_t le_val = LE32(val);
    uint32_t be_val = BE32(val);
    
    // One should equal val, the other should be byte-swapped
    if (le_val == val) {
        // Little endian system
        TEST_ASSERT_EQUAL_INT(val, le_val, "LE32 on little endian system");
        TEST_ASSERT_EQUAL_INT(BS32(val), be_val, "BE32 on little endian system");
    } else {
        // Big endian system
        TEST_ASSERT_EQUAL_INT(val, be_val, "BE32 on big endian system");
        TEST_ASSERT_EQUAL_INT(BS32(val), le_val, "LE32 on big endian system");
    }
}

int test_endian_main(void) {
    TEST_INIT();
    
    RUN_TEST(test_byte_swap_functions);
    RUN_TEST(test_endian_conversions);
    RUN_TEST(test_unaligned_access);
    RUN_TEST(test_endian_consistency);
    
    TEST_SUMMARY();
}