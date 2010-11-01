set(examples_conversation_SRCS
    chatwindow.cpp
    conversation.cpp)

set(examples_conversation_MOC_SRCS
    chatwindow.h)

tpqt4_generate_mocs(${examples_conversation_MOC_SRCS})

add_executable(examples_conversation
    ${examples_conversation_SRCS} ${examples_conversation_MOC_SRCS})

target_link_libraries(examples_conversation
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)

