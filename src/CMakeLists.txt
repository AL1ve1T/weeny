add_subdirectory(server)

add_executable(weeny main.c)

target_link_libraries(weeny PRIVATE server)