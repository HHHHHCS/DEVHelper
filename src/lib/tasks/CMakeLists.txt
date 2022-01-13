add_library(tasks_lib
    task.hpp
    tasks_queue.hpp
)

target_link_libraries(tasks_lib
INTERFACE
    ${Boost_LIBRARIES}

    util_lib    # TOOD(huangchsh): 此库应该在tasks_lib单独实现
)

target_include_directories(tasks_lib
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)