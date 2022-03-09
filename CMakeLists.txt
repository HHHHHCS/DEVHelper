cmake_minimum_required(VERSION 3.0)

project(DEVHelper)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS ON)  # only find release libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
# set(Boost_DEBUG ON) # for debug
set(BOOST_ROOT "D:/source_code/boost_1_78_0/") #"F:/DataBank/source_code/boost/boost_1_78_0")
set(BOOST_INCLUDEDIR "D:/source_code/boost/include/boost-1_78/boost") #"F:/DataBank/source_code/boost/boost/include/boost-1_78/boost")
set(BOOST_LIBRARYDIR "D:/source_code/boost/lib") #"F:/DataBank/source_code/boost/boost/lib")
set(Boost_NO_SYSTEM_PATHS ON)

set(QT_RESOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/Resources/qml.qrc
)

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

add_subdirectory(src)

add_executable(${PROJECT_NAME}
  ${QT_RESOURCES}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  dev_helper_src
)
