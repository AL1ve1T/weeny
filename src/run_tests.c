//
// Created by al1ve1t on 11.12.21.
//
#include "unity.h"
#include "main_test.c"

void RunTests() {
    UNITY_BEGIN();
    RUN_TEST(test_Print_should_PrintExpectedString);
    UNITY_END();
}
