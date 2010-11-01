# qml view
set(examples_accounts_model_SRCS
    accounts.cpp
    accountwindow.cpp
    telepathy-initializer.cpp)

set(examples_accounts_model_MOC_SRCS
    accountwindow.h
    telepathy-initializer.h)

tpqt4_generate_mocs(${examples_accounts_model_MOC_SRCS})

add_executable(examples_accounts_model
    ${examples_accounts_model_SRCS} ${examples_accounts_model_MOC_SRCS})
target_link_libraries(examples_accounts_model
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)

# tree view
set(examples_treeview_SRCS
    tree-view-main.cpp
    telepathy-initializer.cpp
    tree-view.cpp)

set(examples_treeview_MOC_SRCS
    telepathy-initializer.h
    tree-view.h)

tpqt4_generate_mocs(${examples_treeview_MOC_SRCS})

add_executable(examples_treeview
    ${examples_treeview_SRCS} ${examples_treeview_MOC_SRCS})

target_link_libraries(examples_treeview
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)

