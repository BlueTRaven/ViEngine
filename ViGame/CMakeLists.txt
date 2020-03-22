project(ViGame)

file(GLOB GAME_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB GAME_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

include_directories(${ViFramework_SRC_DIRECTORY} ${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

add_executable(ViGame ${GAME_SRC_FILES} ${GAME_HEADER_FILES})
target_link_libraries(ViGame ViFramework ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} GLEW::GLEW)
set_target_properties(ViGame PROPERTIES CXX_STANDARD 17 VS_DEBUGGER_WORKING_DIRECTORY "$(SolutionDir)$(Configuration)\\")

add_custom_command(TARGET ViGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:ViGame>/Assets/)