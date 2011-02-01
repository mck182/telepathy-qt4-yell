file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_gen")

# Set the required flags found in TelepathyDefaults
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")
set(LD_FLAGS "${LD_FLAGS} ${VISIBILITY_HIDDEN_FLAGS} ${COMPILER_COVERAGE_FLAGS} ${DEPRECATED_DECLARATIONS_FLAGS}")

# We are building telepathy-qt4-yell
add_definitions(-DBUILDING_TELEPATHY_QT4_YELL)

# Sources for Tp-Qt4
set(telepathy_qt4_yell_SRCS
    call-channel.cpp
    call-channel-internal.h
    channel.cpp
    channel-class-spec.cpp
    channel-factory.cpp
    types.cpp)

# Exported headers for Tp-Qt4
set(telepathy_qt4_yell_HEADERS
    CallChannel
    CallContent
    CallStream
    Channel
    ChannelClassSpec
    ChannelFactory
    Constants
    Global
    PendingCallContent
    Types
    call-channel.h
    channel.h
    channel-class-spec.h
    channel-factory.h
    constants.h
    global.h
    types.h)

# Headers file moc will be run on
set(telepathy_qt4_yell_MOC_SRCS
    call-channel.h
    call-channel-internal.h
    channel.h)

# Generated headers which will be installed and exported
set(telepathy_qt4_yell_gen_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-channel.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
    ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)

# The escape character in MSVC is ^
if(MSVC)
    set(TYPES_INCLUDE ^<TelepathyQt4Yell/Types^> )
    set(GLOBAL_INCLUDE ^<TelepathyQt4Yell/Global^> )
    set(CHANNEL_EXTRA_INCLUDES '^<TelepathyQt4Yell/Types^>,^<TelepathyQt4/Channel^>' )
else(MSVC)
    set(TYPES_INCLUDE '<TelepathyQt4Yell/Types>' )
    set(GLOBAL_INCLUDE '<TelepathyQt4Yell/Global>' )
    set(CHANNEL_EXTRA_INCLUDES '<TelepathyQt4Yell/Types>,<TelepathyQt4/Channel>' )
endif(MSVC)

# Generate the spec files
set(gen_stable_spec_xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/stable-spec.xml)

tpqt4_xincludator(stable-ifaces-includator ${CMAKE_CURRENT_SOURCE_DIR}/stable-interfaces.xml ${gen_stable_spec_xml})

tpqt4_constants_gen(stable-constants ${gen_stable_spec_xml} ${CMAKE_CURRENT_BINARY_DIR}/_gen/constants.h
                    --namespace=Tpy
                    --define-prefix=TP_QT4_YELL_
                    --must-define=IN_TELEPATHY_QT4_YELL_HEADER
                    DEPENDS stable-ifaces-includator)

tpqt4_types_gen(stable-typesgen ${gen_stable_spec_xml}
                ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp
                Tpy TelepathyQt4Yell/types.h TelepathyQt4Yell/Types
                --must-define=IN_TELEPATHY_QT4_YELL_HEADER
                --visibility=TELEPATHY_QT4_YELL_EXPORT
                --extraincludes=${GLOBAL_INCLUDE}
                DEPENDS stable-constants)

# Add the generated types to the library's sources
list(APPEND telepathy_qt4_yell_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types.h)
list(APPEND telepathy_qt4_yell_SRCS ${CMAKE_CURRENT_BINARY_DIR}/_gen/types-body.hpp)

# For each spec, generate a cli file and add it to the sources (including mocs).
set(SPECS
    channel)
foreach(spec ${SPECS})
    tpqt4_xincludator(${spec}-spec-xincludator ${CMAKE_CURRENT_SOURCE_DIR}/${spec}.xml ${CMAKE_CURRENT_BINARY_DIR}/_gen/spec-${spec}.xml
                      DEPENDS stable-typesgen)
    set(NEW_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}-body.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/_gen/cli-${spec}.moc.hpp)
    list(APPEND telepathy_qt4_yell_SRCS ${NEW_FILES})
    list(APPEND telepathy_qt4_yell_generated_specs_mocs "moc-cli-${spec}.moc.hpp")
    set_source_files_properties(${NEW_FILES} PROPERTIES GENERATED true)
endforeach(spec ${SPECS})

# Use the client generator for generating headers out of specs
tpqt4_client_generator(${gen_stable_spec_xml} channel clientchannel TelepathyQt4Yell/channel.h TelepathyQt4Yell/Channel Tpy::Client Tpy IN_TELEPATHY_QT4_YELL_HEADER TELEPATHY_QT4_YELL_EXPORT --mainiface=Tp::Client::ChannelInterface --extraincludes=${CHANNEL_EXTRA_INCLUDES} DEPENDS channel-spec-xincludator)

# Create the library
add_library(telepathy-qt4-yell STATIC ${telepathy_qt4_yell_SRCS})

# generate client moc files
foreach(moc_src ${telepathy_qt4_yell_MOC_SRCS})
    set(generated_file _gen/${moc_src})
    string(REPLACE ".h" ".moc.hpp" generated_file ${generated_file})
    tpqt4_generate_moc_i_target_deps(${CMAKE_CURRENT_SOURCE_DIR}/${moc_src} ${CMAKE_CURRENT_BINARY_DIR}/${generated_file}
                                     ${telepathy_qt4_yell_generated_specs_mocs})
    list(APPEND telepathy_qt4_yell_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${generated_file})
    string(REPLACE ".h" ".moc.hpp" moc_src ${moc_src})
    add_dependencies(telepathy-qt4-yell "moc-${moc_src}")
endforeach(moc_src ${telepathy_qt4_yell_MOC_SRCS})

# Link
target_link_libraries(telepathy-qt4-yell
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${TELEPATHY_QT4_LIBRARIES})

if (ENABLE_COMPILER_COVERAGE)
    target_link_libraries(telepathy-qt4-yell gcov)
endif (ENABLE_COMPILER_COVERAGE)

# Set the correct version number
set_target_properties(telepathy-qt4-yell PROPERTIES
                      SOVERSION ${TP_QT4_YELL_ABI_VERSION}
                      VERSION ${TP_QT4_YELL_LIBRARY_VERSION})

# Install header files
install(FILES ${telepathy_qt4_yell_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Yell COMPONENT headers)
install(FILES ${telepathy_qt4_yell_gen_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/telepathy-1.0/TelepathyQt4Yell/_gen COMPONENT headers)

# Install the library - watch out for the correct components
if (WIN32)
    install(TARGETS telepathy-qt4-yell RUNTIME DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
                                  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
else (WIN32)
    install(TARGETS telepathy-qt4-yell LIBRARY DESTINATION ${LIB_INSTALL_DIR} COMPONENT mainlibrary
                                  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT headers)
endif (WIN32)

# pkg-config file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4Yell.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Yell.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4Yell-uninstalled.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Yell-uninstalled.pc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4Yell.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig COMPONENT headers)

add_subdirectory(Models)

# If Farstream was found, install its pkg-config files as well, and go into the subdirectory
if(FARSTREAM_COMPONENTS_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4YellFarstream.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4YellFarstream.pc)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelepathyQt4YellFarstream-uninstalled.pc.in ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4YellFarstream-uninstalled.pc)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/TelepathyQt4YellFarstream.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig COMPONENT farstream_headers)
endif(FARSTREAM_COMPONENTS_FOUND)

add_subdirectory(Farstream)
