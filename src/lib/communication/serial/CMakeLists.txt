find_package(Qt5
REQUIRED COMPONENTS
  Core
  SerialBus
  SerialPort
)

add_library(serial_lib
    q_serial/q_serial.cpp
    q_serial/q_serial.h
)

target_link_libraries(serial_lib
PUBLIC
    Qt5::SerialBus
    Qt5::SerialPort
)

target_include_directories(serial_lib
PUBLIC
    q_serial
)