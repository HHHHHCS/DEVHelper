cmake_minimum_required(VERSION 3.0)

project(DEVHelper LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5
REQUIRED COMPONENTS
  Qml
  Widgets
)

aux_source_directory(./src SRCS_DIR)

add_subdirectory(./src/lib dev_helper_lib)
add_subdirectory(./src/managers managers)
add_subdirectory(./ui ui_lib)

add_executable(${PROJECT_NAME}
  ${SRCS_DIR}
)

add_dependencies(${PROJECT_NAME}
  dev_helper_lib  
  managers
  ui_lib
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  Qt5::Qml
  Qt5::Widgets
  dev_helper_lib
  managers
  ui_lib
)
