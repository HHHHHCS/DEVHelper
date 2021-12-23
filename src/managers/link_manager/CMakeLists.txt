aux_source_directory(./link_interface LINK_ITF_DIR)
aux_source_directory(./proto_interface PROTO_ITF_DIR)

add_library(
    ${LINK_ITF_DIR}
    ${PROTO_ITF_DIR}
)

target_link_libraries(link_manager
PUBLIC
    dev_helper_lib
)

target_include_directories(link_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)