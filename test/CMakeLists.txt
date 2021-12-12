add_executable(test
        test.c
        test.h
        )

target_link_libraries(test PRIVATE unity)
target_link_libraries(test PRIVATE server)