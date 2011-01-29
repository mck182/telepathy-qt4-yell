if(FARSIGHT_COMPONENTS_FOUND)
    include_directories(${TELEPATHY_FARSIGHT_INCLUDE_DIR}
                        ${GSTREAMER_INCLUDE_DIR}
                        ${GLIB2_INCLUDE_DIR}
                        ${LIBXML2_INCLUDE_DIR}
                        ${DBUS_INCLUDE_DIR})

    # It gets inherited from the previous directory, hence it has to be removed explicitely
    remove_definitions(-DBUILDING_TELEPATHY_QT4_YELL)
    # We are building Telepathy-Qt4-Yell-Farsight
    add_definitions(-DBUILDING_TELEPATHY_QT4_YELL_FARSIGHT -DQT_NO_KEYWORDS)

    set(telepathy_qt4_yell_farsight_SRCS
        channel.cpp)

    set(telepathy_qt4_yell_farsight_HEADERS
        Channel
        channel.h
        global.h)

    set(telepathy_qt4_yell_farsight_MOC_SRCS
        channel.h)

    # generate client moc files
    tpqt4_generate_mocs(${telepathy_qt4_yell_farsight_MOC_SRCS})

    # Create the library
    if (ENABLE_COMPILER_COVERAGE)
        add_library(telepathy-qt4-yell-farsight STATIC ${telepathy_qt4_yell_farsight_SRCS} ${telepathy_qt4_yell_farsight_MOC_SRCS})
    else (ENABLE_COMPILER_COVERAGE)
        add_library(telepathy-qt4-yell-farsight SHARED ${telepathy_qt4_yell_farsight_SRCS} ${telepathy_qt4_yell_farsight_MOC_SRCS})
    endif (ENABLE_COMPILER_COVERAGE)
    # Link
    target_link_libraries(telepathy-qt4-yell-farsight
        ${QT_QTDBUS_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${TELEPATHY_FARSIGHT_LIBRARIES}
        ${GSTREAMER_INTERFACE_LIBRARY}
        telepathy-qt4-yell)

    # Set the correct version number
    set_target_properties(telepathy-qt4-yell-farsight PROPERTIES
                                                      SOVERSION ${TP_QT4_YELL_ABI_VERSION}
                                                      VERSION ${TP_QT4_YELL_LIBRARY_VERSION})

    # Install the library - watch out for the correct components
    if (WIN32)
        install(TARGETS telepathy-qt4-yell-farsight RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT farsight
                                                    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT farsight_headers)
    else (WIN32)
        install(TARGETS telepathy-qt4-yell-farsight LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT farsight
                                                    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT farsight_headers)
    endif (WIN32)

    # Install headers
    install(FILES ${telepathy_qt4_yell_farsight_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Yell/Farsight
            COMPONENT farsight_headers)
endif(FARSIGHT_COMPONENTS_FOUND)
