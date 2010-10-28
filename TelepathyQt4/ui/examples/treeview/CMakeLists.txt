set(telepathy_qt4_ui_examples_treeview_SRCS
    main.cpp
    telepathy-initializer.cpp
    tree-view.cpp)

set(telepathy_qt4_ui_examples_treeview_MOC_SRCS
    telepathy-initializer.h
    tree-view.h)

tpqt4_generate_mocs(${telepathy_qt4_ui_examples_treeview_MOC_SRCS})

add_executable(telepathy_qt4_ui_examples_treeview
    ${telepathy_qt4_ui_examples_treeview_SRCS} ${telepathy_qt4_ui_examples_treeview_MOC_SRCS})

target_link_libraries(telepathy_qt4_ui_examples_treeview
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    telepathy-qt4
    telepathy-qt4-ui)

