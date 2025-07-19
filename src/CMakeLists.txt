add_executable(${PROJECT_NAME} main.cpp )
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE vendored)
