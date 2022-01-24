find_package(Qt5
REQUIRED COMPONENTS
  Core
  SerialBus
  SerialPort
)

add_library(q_serial
    q_serial.cpp
    q_serial.h
)

target_link_libraries(q_serial
PUBLIC
    Qt5::SerialBus
    Qt5::SerialPort
)

target_include_directories(q_serial
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)