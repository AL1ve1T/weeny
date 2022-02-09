add_library(
        server
        server.h
        server.c
        socks/retranslator.h
        socks/retranslator.c
)

target_link_libraries(server PRIVATE "/usr/local/lib/libzlog.a")
