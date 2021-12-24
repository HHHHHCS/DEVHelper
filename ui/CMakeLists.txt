add_library(ui_lib
  ./Resources/qml.qrc
)

target_compile_definitions(ui_lib
PRIVATE 
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(ui_lib
PRIVATE
  Qt5::Quick
  Qt5::Qml
  Qt5::Widgets
)