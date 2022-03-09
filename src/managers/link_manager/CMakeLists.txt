set(LINK_LIB
    link_interface/link.h
    link_interface/serial_link.cpp
    link_interface/serial_link.h
)

set(PROTO_LIB
    proto_interface/proto.h
    proto_interface/devlink2_proto.cpp
    proto_interface/devlink2_proto.h
)

add_library(link_manager
    link_manager.cpp
    link_manager.h
    ${LINK_LIB}
    ${PROTO_LIB}
)

target_link_libraries(link_manager
INTERFACE
    tasks

    q_serial

    ymodem
    devlink2
PRIVATE
    managers
)

target_include_directories(link_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    link_interface
    proto_interface
)