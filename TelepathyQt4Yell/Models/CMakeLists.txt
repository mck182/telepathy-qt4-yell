# It gets inherited from the previous directory, hence it has to be removed explicitely
remove_definitions(-DBUILDING_TELEPATHY_QT4_YELL)
# We are building Telepathy-Qt4-Yell-Models
add_definitions(-DBUILDING_TELEPATHY_QT4_YELL_MODELS)

# Set source files
set(telepathy_qt4_yell_models_SRCS
    accounts-model.cpp
    accounts-model-item.cpp
    avatar-image-provider.cpp
    contact-model-item.cpp
    conversation-item.cpp
    conversation-model.cpp
    flat-model-proxy.cpp
    tree-node.cpp
)

set(telepathy_qt4_yell_models_MOC_SRCS
    accounts-model.h
    accounts-model-item.h
    avatar-image-provider.h
    contact-model-item.h
    conversation-item.h
    conversation-model.h
    flat-model-proxy.h
    tree-node.h
)

set(telepathy_qt4_yell_models_HEADERS
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

tpqt4_generate_mocs(${telepathy_qt4_yell_models_MOC_SRCS})

# Create the library
add_library(telepathy-qt4-yell-models STATIC ${telepathy_qt4_yell_models_SRCS} ${telepathy_qt4_yell_models_MOC_SRCS})

# Link
target_link_libraries(telepathy-qt4-yell-models
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    ${TELEPATHY_QT4_LIBRARIES})

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-qt4-yell-models gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-qt4-yell-models PROPERTIES
    SOVERSION ${TP_QT4_YELL_ABI_VERSION}
    VERSION ${TP_QT4_YELL_LIBRARY_VERSION})

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-yell-models RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
else (WIN32)
    install(TARGETS telepathy-qt4-yell-models LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT models
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT models_headers)
endif (WIN32)

# Install headers
install(FILES ${telepathy_qt4_yell_models_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Yell/Models
    COMPONENT models_headers)

