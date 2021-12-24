set(SERIAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/serial)

add_library(libs
    ${SERIAL_DIR}/q_serial.cpp
    ${SERIAL_DIR}/q_serial.h
)

target_link_libraries(libs
PUBLIC
  Qt5::SerialBus
  Qt5::SerialPort

  dev_helper_src
)

target_include_directories(libs
INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
PUBLIC
  ${SERIAL_DIR}
)