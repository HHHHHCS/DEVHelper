set(LINK_ITF_DIR link_interface)
set(PROTO_ITF_DIR proto_interface)

add_library(link_manager
    link_manager.cpp
    link_manager.h
    ${LINK_ITF_DIR}/link.h
    ${PROTO_ITF_DIR}/proto.cpp
    ${PROTO_ITF_DIR}/proto.h
)

target_link_libraries(link_manager
PRIVATE
    dev_helper_src
)

target_include_directories(link_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)