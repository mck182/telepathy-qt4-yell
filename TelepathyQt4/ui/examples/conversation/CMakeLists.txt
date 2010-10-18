set(telepathy_qt4_ui_examples_conversation_SRCS
    chatwindow.cpp
    conversation.cpp)

set(telepathy_qt4_ui_examples_conversation_MOC_SRCS
    chatwindow.h)

tpqt4_generate_mocs(${telepathy_qt4_ui_examples_conversation_MOC_SRCS})

add_executable(telepathy_qt4_ui_examples_conversation
    ${telepathy_qt4_ui_examples_conversation_SRCS} ${telepathy_qt4_ui_examples_conversation_MOC_SRCS})

target_link_libraries(telepathy_qt4_ui_examples_conversation
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)

