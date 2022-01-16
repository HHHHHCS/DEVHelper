cmake_minimum_required(VERSION 3.0)

project(DEVHelper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS ON)  # only find release libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
# set(Boost_DEBUG ON) # for debug
set(BOOST_ROOT "F:/DataBank/source_code/boost/boost_1_78_0") # "D:/source_code/boost_1_78_0/")
set(BOOST_INCLUDEDIR "F:/DataBank/source_code/boost/boost/include/boost-1_78/boost") # "D:/source_code/boost/include/boost-1_78/boost")
set(BOOST_LIBRARYDIR "F:/DataBank/source_code/boost/boost/lib") # "D:/source_code/boost/lib")
set(Boost_NO_SYSTEM_PATHS ON)

find_package(Qt5
REQUIRED COMPONENTS
  Core
  Qml
  Quick
  QuickControls2
  QuickWidgets
  Widgets
)

find_package(Boost
REQUIRED COMPONENTS
  system
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
