aux_source_directory(./link_interface LINK_ITF_DIR)
aux_source_directory(./proto_interface PROTO_ITF_DIR)

add_library(link_manager
    ${LINK_ITF_DIR}
    ${PROTO_ITF_DIR}
)

add_dependencies(link_manager
    dev_helper_lib
)

target_link_libraries(link_manager
PUBLIC
    Qt5::Core
    Qt5::Qml
    Qt5::Widgets
    dev_helper_lib
)

target_include_directories(link_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    link_interface
    proto_interface
)