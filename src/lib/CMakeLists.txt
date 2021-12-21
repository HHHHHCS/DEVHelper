cmake_minimum_required(VERSION 3.0)

project(dev_helper_lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5
REQUIRED COMPONENTS
  Core
  SerialBus
  SerialPort
)

aux_source_directory(./serial SERIAL_SRC_LIST)

add_library(${PROJECT_NAME} STATIC
    ${SERIAL_SRC_LIST}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  Qt5::Core
  Qt5::SerialBus
  Qt5::SerialPort
)