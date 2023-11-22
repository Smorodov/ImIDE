# ----------------------------------------------------------------------------
# 
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.27)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}) 
set(CMAKE_C_FLAGS "/EHsc")
set(CMAKE_CXX_FLAGS "/EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
# ----------------------------------------------------------------------------
# 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:char8_t- /std:c++latest")


project(ImIde)

add_definitions(-D_USE_MATH_DEFINES -D_MBCS)
if(MSVC)
	add_definitions(/W4)
else()
	add_definitions(-Wall -Wextra -Wpedantic)
endif()

find_package(OpenMP)
if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_LIBRARY_ARCHITECTURE x64)
  else()
    set(CMAKE_LIBRARY_ARCHITECTURE x86)
  endif()
endif()

# ----------------------------------------------------------------------------   
# 
# ----------------------------------------------------------------------------
include_directories(./)
include_directories(../)
include_directories(eventpp)

INCLUDE_DIRECTORIES("../libconfig")
INCLUDE_DIRECTORIES("FileBrowser")
INCLUDE_DIRECTORIES("imgui/misc/freetype")
INCLUDE_DIRECTORIES("freetype/include")
INCLUDE_DIRECTORIES("glew/include")
INCLUDE_DIRECTORIES("imgui")
INCLUDE_DIRECTORIES("imgui/examples")
INCLUDE_DIRECTORIES("imgui/backends")
INCLUDE_DIRECTORIES("imgui/examples/libs/glfw/include")

add_definitions(-DGLEW_STATIC)
add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(glfw)
add_subdirectory(freetype)


set(SRC_MAIN
	main.cpp
	events.cpp
	events.h
	MainWindow.cpp
	MainWindow.h
   	glew/src/glew.c
	ImGuiPropertyInspector.cpp
)

SET(IMGUI_SOURCE
			   FileBrowser/ImGuiFileBrowser.cpp
			   FileBrowser/ImGuiFileBrowser.h
			   imgui/backends/imgui_impl_glfw.cpp
			   imgui/backends/imgui_impl_opengl3.cpp
			   imgui/imgui.cpp
			   imgui/misc/freetype/imgui_freetype.cpp
			   imgui/imgui_draw.cpp
			   imgui/imgui_demo.cpp
			   imgui/imgui_tables.cpp
			   imgui/imgui_widgets.cpp
			   imgui/misc/freetype/imgui_freetype.h
)

add_executable(
	${PROJECT_NAME}
	${IMGUI_SOURCE}
	${SRC_MAIN}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
					  optimized ${LIBRARY_OUTPUT_PATH}/Release/glfw3.lib
					  debug 	${LIBRARY_OUTPUT_PATH}/Debug/glfw3.lib				  
					  
					  opengl32.lib
					  freetype
					  Threads::Threads)
if(MSVC)
# ----------------------------------------------------------------------------
# 
# ----------------------------------------------------------------------------
set_target_properties( ${project_name} PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})
endif()

