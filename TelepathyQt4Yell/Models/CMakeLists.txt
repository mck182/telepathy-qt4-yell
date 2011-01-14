# It gets inherited from the previous directory, hence it has to be removed explicitely
remove_definitions(-DBUILDING_TELEPATHY_QT4_YELL)
# We are building Telepathy-Qt4-Models
add_definitions(-DBUILDING_TELEPATHY_QT4_YELL_MODELS)

# Set source files
set(telepathy_qt4_models_SRCS
    accounts-model.cpp
    accounts-model-item.cpp
    avatar-image-provider.cpp
    contact-model-item.cpp
    conversation-item.cpp
    conversation-model.cpp
    flat-model-proxy.cpp
    tree-node.cpp
)

set(telepathy_qt4_models_MOC_SRCS
    accounts-model.h
    accounts-model-item.h
    avatar-image-provider.h
    contact-model-item.h
    conversation-item.h
    conversation-model.h
    flat-model-proxy.h
    tree-node.h
)

set(telepathy_qt4_models_HEADERS
    AccountsModel
    AccountsModelItem
    AvatarImageProvider
    ContactModelItem
    ConversationItem
    ConversationModel
    FlatModelProxy
    Global
    TreeNode
    accounts-model.h
    accounts-model-item.h
    avatar-image-provider.h
    contact-model-item.h
    conversation-item.h
    conversation-model.h
    flat-model-proxy.h
    global.h
    tree-node.h
)

tpqt4_generate_mocs(${telepathy_qt4_models_MOC_SRCS})

# Create library
if (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-models STATIC ${telepathy_qt4_models_SRCS} ${telepathy_qt4_models_MOC_SRCS})
else (ENABLE_COMPILER_COVERAGE)
    add_library(telepathy-qt4-models SHARED ${telepathy_qt4_models_SRCS} ${telepathy_qt4_models_MOC_SRCS})
endif (ENABLE_COMPILER_COVERAGE)

# Link
target_link_libraries(telepathy-qt4-models
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4)

# Set the correct version number
set_target_properties(telepathy-qt4-models PROPERTIES
    SOVERSION ${TP_QT4_ABI_VERSION}
    VERSION ${TP_QT4_LIBRARY_VERSION})

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-models RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
else (WIN32)
    install(TARGETS telepathy-qt4-models LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
endif (WIN32)

# Install headers
install(FILES ${telepathy_qt4_models_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Yell/Models
    COMPONENT models_headers)

