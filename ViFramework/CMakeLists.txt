project(ViFramework)

set(ViFramework_SRC_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/" CACHE PATH "The src directory for ViFramework.")

file(GLOB LIB_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB LIB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

include_directories(${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

add_library(ViFramework SHARED ${LIB_SRC_FILES} ${LIB_HEADER_FILES})
target_link_libraries(ViFramework ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} GLEW::GLEW)
set_target_properties(ViFramework PROPERTIES CXX_STANDARD 17)