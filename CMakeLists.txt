cmake_minimum_required(VERSION 3.0)

project(DEVHelper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5
REQUIRED COMPONENTS
  Core
  Qml
  Quick
  QuickControls2
  QuickWidgets
  Widgets
)

set(RESOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/Resources/qml.qrc
)

add_subdirectory(src)
# add_subdirectory(ui)

add_executable(${PROJECT_NAME}
  ${RESOURCES}
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  dev_helper_src
  # ui_lib
)
