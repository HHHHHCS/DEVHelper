add_library(link_manager
    link_manager.cpp
    link_manager.h
    link_interface/link.h
    link_interface/serial_link.cpp
    link_interface/serial_link.h
    proto_interface/proto.cpp
    proto_interface/proto.h
)

target_link_libraries(link_manager
PRIVATE
    dev_helper_src

    serial_lib
)

target_include_directories(link_manager
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    link_interface
    proto_interface
)