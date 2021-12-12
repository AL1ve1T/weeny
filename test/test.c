//
// Created by al1ve1t on 11.12.21.
//
#include "unity.h"
#include "../src/server/server.h"

void setUp() {

}

void test_Print_should_PrintExpectedString(void) {
    TEST_ASSERT_EQUAL_STRING("Hello, World!\n", Print());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_Print_should_PrintExpectedString);
    UNITY_END();
}

void tearDown() {

}
