#ifndef CONFIG
#define CONFIG "config.h"
#endif // CONFIG
#include CONFIG

#include "test_framework.h"
#include "types.h"
#include "crypto.h"
#include <string.h>

void test_XorBlock(void) {
    BYTE input1[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
                       0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    BYTE input2[16] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
                       0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
    BYTE expected[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                         0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    BYTE output[16];
    
    memcpy(output, input2, 16);
    XorBlock(input1, output);
    
    TEST_ASSERT_EQUAL_MEM(expected, output, 16, "XorBlock should XOR two 16-byte blocks correctly");
    
    // Test XOR with itself should produce zero
    BYTE zeros[16] = {0};
    memcpy(output, input1, 16);
    XorBlock(input1, output);
    TEST_ASSERT_EQUAL_MEM(zeros, output, 16, "XorBlock with itself should produce zeros");
}

void test_AES_key_initialization(void) {
    AesCtx ctx;
    BYTE key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    
    // Test AES key initialization for V5
    AesInitKey(&ctx, key, 0, 16);
    TEST_ASSERT_EQUAL_INT(10, ctx.rounds, "AES-128 should have 10 rounds");
    TEST_ASSERT_NOT_NULL(ctx.Key, "AES key should be initialized");
    
    // Test AES key initialization for V6
    AesInitKey(&ctx, key, 1, 16);
    TEST_ASSERT_EQUAL_INT(10, ctx.rounds, "AES-128 for V6 should have 10 rounds");
}

void test_AES_encrypt_decrypt_roundtrip(void) {
    AesCtx ctx;
    BYTE key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    BYTE plaintext[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                          0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    BYTE encrypted[16];
    BYTE decrypted[16];
    
    // Initialize AES key
    AesInitKey(&ctx, key, 0, 16);
    
    // Encrypt
    memcpy(encrypted, plaintext, 16);
    AesEncryptBlock(&ctx, encrypted);
    
    // Verify encryption changed the data
    TEST_ASSERT(!memcmp(plaintext, encrypted, 16) == 0, "AES encryption should change the data");
    
    // Decrypt
    memcpy(decrypted, encrypted, 16);
    AesDecryptBlock(&ctx, decrypted);
    
    // Verify round-trip
    TEST_ASSERT_EQUAL_MEM(plaintext, decrypted, 16, "AES decrypt should recover original plaintext");
}

void test_AES_known_vectors(void) {
    AesCtx ctx;
    // NIST test vector for AES-128
    BYTE key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    BYTE plaintext[16] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                          0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    BYTE expected_ciphertext[16] = {0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
                                    0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};
    BYTE ciphertext[16];
    
    AesInitKey(&ctx, key, 0, 16);
    memcpy(ciphertext, plaintext, 16);
    AesEncryptBlock(&ctx, ciphertext);
    
    TEST_ASSERT_EQUAL_MEM(expected_ciphertext, ciphertext, 16, "AES encryption should match NIST test vector");
}

void test_AES_builtin_keys(void) {
    // Test that the built-in AES keys are accessible
    TEST_ASSERT_NOT_NULL(AesKeyV5, "AesKeyV5 should be defined");
    TEST_ASSERT_NOT_NULL(AesKeyV6, "AesKeyV6 should be defined");
    
    // The keys should be different
    TEST_ASSERT(memcmp(AesKeyV5, AesKeyV6, 16) != 0, "AesKeyV5 and AesKeyV6 should be different");
}

int test_crypto_main(void) {
    TEST_INIT();
    
    RUN_TEST(test_XorBlock);
    RUN_TEST(test_AES_key_initialization);
    RUN_TEST(test_AES_encrypt_decrypt_roundtrip);
    RUN_TEST(test_AES_known_vectors);
    RUN_TEST(test_AES_builtin_keys);
    
    TEST_SUMMARY();
}