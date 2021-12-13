add_executable(weeny-test
        test.c
        test.h
        )

target_link_libraries(weeny-test PRIVATE unity)
target_link_libraries(weeny-test PRIVATE server)