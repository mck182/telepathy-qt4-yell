set(telepathy_qt4_ui_examples_accounts_SRCS
    accounts.cpp)

set(telepathy_qt4_ui_examples_accounts_MOC_SRCS)

tpqt4_generate_mocs(${telepathy_qt4_ui_examples_accounts_MOC_SRCS})

add_executable(telepathy_qt4_ui_examples_accounts
    ${telepathy_qt4_ui_examples_accounts_SRCS} ${telepathy_qt4_ui_examples_accounts_MOC_SRCS})
target_link_libraries(telepathy_qt4_ui_examples_accounts
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)
