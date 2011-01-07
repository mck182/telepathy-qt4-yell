include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${TELEPATHY_GLIB_INCLUDE_DIR}
    ${GLIB2_INCLUDE_DIR}
    ${GOBJECT_INCLUDE_DIR}
    ${DBUS_INCLUDE_DIR})

if(ENABLE_TP_GLIB_TESTS)
    add_subdirectory(extensions)
    add_subdirectory(call)
endif(ENABLE_TP_GLIB_TESTS)
