set(LINK_LIB
    link_interface/link.h
    link_interface/serial_link.cpp
    link_interface/serial_link.h
<<<<<<< HEAD
)

set(PROTO_LIB
=======
>>>>>>> 501252ed713a16cd6d49f0f418f00a1771d803e0
    proto_interface/proto.h
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
PRIVATE
    managers
)

target_include_directories(link_manager
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    link_interface
    proto_interface
)