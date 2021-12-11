//
// Created by al1ve1t on 10.12.21.
//

#include "unity.h"
#include "../main.c"

void test_Print_should_PrintExpectedString(void) {
    TEST_ASSERT_EQUAL_STRING("Hello, World", Print());
}
