# Set source files
set(telepathy_qt4_ui_SRCS
    account-model.cpp
    account-model-item.cpp
    contact-model-item.cpp
    conversation-item.cpp
    conversation-model.cpp
    flat-model-proxy.cpp
    tree-node.cpp
)

set(telepathy_qt4_ui_MOC_SRCS
    abstract-tree-item.h
    account-model.h
    account-model-item.h
    contact-model-item.h
    conversation-item.h
    conversation-model.h
    flat-model-proxy.h
    tree-node.h
)

set(telepathy_qt4_ui_HEADERS
    AccountModel
    ConversationModel
    FlatModelProxy
    account-model.h
    account-model-item.h
    contact-model-item.h
    conversation-item.h
    conversation-model.h
    flat-model-proxy.h
    tree-node.h
)

tpqt4_generate_mocs(${telepathy_qt4_ui_MOC_SRCS})

# Create library
if (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-ui STATIC ${telepathy_qt4_ui_SRCS} ${telepathy_qt4_ui_MOC_SRCS})
else (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-ui SHARED ${telepathy_qt4_ui_SRCS} ${telepathy_qt4_ui_MOC_SRCS})
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

add_subdirectory(examples)
