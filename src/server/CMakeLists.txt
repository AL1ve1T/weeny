add_library(
        server
        server.h
        server.c
)

target_link_libraries(server PRIVATE "/usr/local/lib/libzlog.a")
