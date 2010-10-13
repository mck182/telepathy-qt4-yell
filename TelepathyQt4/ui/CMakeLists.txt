# Set source files
set(telepathy_qt4_ui_SRCS
    # abstract-tree-item.cpp
    account-model.cpp
    # contact-item.cpp
    # contacts-list-model.cpp
)

set(telepathy_qt4_ui_HEADERS
    # abstract-tree-item.h
    account-model.h
    # contact-item.h
    # contacts-list-model.h
)

# Create library
if (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-ui STATIC ${telepathy_qt4_ui_SRCS})
else (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-ui SHARED ${telepathy_qt4_ui_SRCS})
endif (ENABLE_COMPILER_COVERAGE)

# Link
target_link_libraries(telepathy-qt4-ui
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    telepathy-qt4)

# Set the correct version number
set_target_properties(telepathy-qt4-ui PROPERTIES
    SOVERSION ${TP_QT4_ABI_VERSION}
    VERSION ${TP_QT4_LIBRARY_VERSION})

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-ui RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT ui
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT ui_headers)
else (WIN32)
    install(TARGETS telepathy-qt4-ui LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT ui
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT ui_headers)
endif (WIN32)

# Install headers
install(FILES ${telepathy_qt4_ui_HEADERS} 
    DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4/ui
    COMPONENT ui_headers)
