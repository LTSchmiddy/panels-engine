cmake_minimum_required(VERSION 3.0.0)
set(TARGET_NAME trove)
project(${TARGET_NAME} VERSION 0.1.0)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

cmake_policy(SET CMP0076 NEW)
set (CMAKE_CXX_STANDARD 17)

if (WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    # set stuff for windows
    add_executable(${TARGET_NAME} WIN32)
  
else()
    # set stuff for other systems
    add_executable(${TARGET_NAME})

endif()

add_subdirectory("./src/cpp")
include_directories("./src/cpp")

# Adding Python
find_package(Python 3.10 COMPONENTS Development Interpreter REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Python::Python)

find_package(pybind11 CONFIG REQUIRED)
# Note: 9 target(s) were omitted.
target_link_libraries(${TARGET_NAME} PRIVATE pybind11::lto pybind11::embed pybind11::module)

message(STATUS PY-SOABI: ${Python_SOABI})

# Adding Boost
find_package(Boost 1.70.0 COMPONENTS filesystem REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Boost::filesystem)

# Once again, boost-python proves to be more trouble than it's worth.

# Adding SDL2
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Adding SDL2-Image
find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2_image)

# Adding SDL2-Net
find_package(sdl2-net CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2_net)

# Adding SDL2-TFF
find_package(sdl2-ttf CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2_ttf)

# Adding IMGUI
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE imgui::imgui)

# Adding OpenGL
find_package(OpenGL COMPONENTS OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE OpenGL::GL)

find_package(tmx CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME}  PRIVATE tmx)

set_target_properties(${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "./arc/"
    LIBRARY_OUTPUT_DIRECTORY "./lib/"
    RUNTIME_OUTPUT_DIRECTORY "./bin/"
)

# Copying Python Data
if(NOT DEFINED PYTHON_LIB_LOCATION)
    set (PYTHON_LIB_LOCATION ${Python_EXECUTABLE}/../Lib)
endif()

if(NOT DEFINED PYTHON_DLLS_LOCATION)
    set (PYTHON_DLLS_LOCATION ${Python_EXECUTABLE}/../DLLs)
endif()

message(STATUS "Copying Python assets...")
file(COPY ${Python_EXECUTABLE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY ${PYTHON_LIB_LOCATION} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

message(STATUS " -> Using native modules from ${PYTHON_DLLS_LOCATION}")
file(COPY ${PYTHON_DLLS_LOCATION} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

message(STATUS "Installing Python/Pip dependencies...")
# Collecting Python dependencies
exec_program(
    ${Python_EXECUTABLE}
    ARGS -m pip install --no-warn-script-location --target=\"${CMAKE_CURRENT_BINARY_DIR}/bin/Lib/site-packages\" -r ${CMAKE_SOURCE_DIR}/requirements.txt 
)

# Copying Python source code. To avoid needing different commands depending 
# on the platform, we'll have the python interpreter that we loaded to copy the files.
add_custom_command(
    TARGET ${TARGET_NAME}
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/bin/python
    ARGS ${CMAKE_SOURCE_DIR}/py_build.py ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
