cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

project (ICG)

set ( PROJECT_DIR A1 )

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
find_package (GLEW REQUIRED)

include_directories (${OPENGL_INCLUDE_DIRS}
                     ${GLUT_INCLUDE_DIRS}
                     ${GLEW_INCLUDE_DIRS}
		     ${PROJECT_SOURCE_DIR}
		     ${PROJECT_SOURCE_DIR}/DaveUtils)

add_subdirectory (InitShader)
add_subdirectory (DaveUtils)

add_executable (Main ${PROJECT_DIR}/main.cpp)

set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

target_link_libraries (Main
                       ${OPENGL_LIBRARIES}
                       ${GLUT_LIBRARY}
                       ${GLEW_LIBRARY}
                       InitShader
		       DaveUtils)
