cmake_minimum_required (VERSION 2.6)
project (ICG_A0)

find_package (OpenGL REQUIRED)
find_package (GLUT REQUIRED)
find_package (GLEW REQUIRED)

include_directories (${OPENGL_INCLUDE_DIRS}
                     ${GLUT_INCLUDE_DIRS}
                     ${GLEW_INCLUDE_DIRS})
add_library (InitShader InitShader.cpp)

add_executable (Main helloWorld432.cpp)

target_link_libraries (Main
                       ${OPENGL_LIBRARIES}
                       ${GLUT_LIBRARY}
                       ${GLEW_LIBRARY}
                       InitShader)
