add_subdirectory(server)

add_executable(weeny main.c globals.h)

include_directories(/usr/local/include)

target_link_libraries(weeny PRIVATE server "/usr/local/lib/libzlog.a")
