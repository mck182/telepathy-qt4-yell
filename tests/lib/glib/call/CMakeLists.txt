if(ENABLE_TP_GLIB_TESTS)
    include_directories(${CMAKE_SOURCE_DIR}/tests/lib/glib)

    set(example_cm_call_SRCS
        call-channel.c
        call-channel.h
        call-content.c
        call-content.h
        call-manager.c
        call-manager.h
        call-stream.c
        call-stream.h
        cm.c
        cm.h
        conn.c
        conn.h
        protocol.c
        protocol.h)

    add_library(example-cm-call STATIC ${example_cm_call_SRCS})
    target_link_libraries(example-cm-call
        tp-glib-tests-extensions
        ${TELEPATHY_GLIB_LIBRARIES}
    )
endif(ENABLE_TP_GLIB_TESTS)
