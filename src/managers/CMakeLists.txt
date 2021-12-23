cmake_minimum_required(VERSION 3.0)

project(managers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5
REQUIRED COMPONENTS
  Core
  Qml
  Widgets
)

add_subdirectory(link_manager)

aux_source_directory(. MAN_SRC_DIR)
aux_source_directory(./main_manager MAIN_MAN_SRC_DIR)
aux_source_directory(./parameters_manager PARAMETERS_MAN_SRC_DIR)
aux_source_directory(./upgrade_manager UPGRADE_MAN_SRC_DIR)

add_library(${PROJECT_NAME} STATIC
    ${MAN_SRC_DIR}
    ${MAIN_MAN_SRC_DIR}
    ${PARAMETERS_MAN_SRC_DIR}
    ${UPGRADE_MAN_SRC_DIR}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  Qt5::Core
  Qt5::Qml
  Qt5::Widgets
  dev_helper_lib

  link_manager
)