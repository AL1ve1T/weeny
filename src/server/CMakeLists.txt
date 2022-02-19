add_library(
        server
        server.h
        server.c
        socks/retranslator.h
        socks/retranslator.c
        ../globals.h
)

target_link_libraries(server PRIVATE "/usr/local/lib/libzlog.a")
