# This cmake configuration was originally created by meemknight on github
# https://github.com/meemknight/cmakeSetup
# It has been modified for use in this repository by Colton Staiduhar

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

# Declare Project where "GSM_TEMPLATE" is the project name
project(Assignment4 LANGUAGES C CXX)

# Set C++ Standards
set(CMAKE_CXX_STANDARD 17)   
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Use FreeGlut on Macos (Requires X11 to be installed, but doesn't use it)
option(USE_FREEGLUT "Use FreeGLUT on Mac (ignored on Windows)" ON)

# Allow cmake to fetch web repositories
include(FetchContent)

# Set the location for downloaded libraries
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(FETCHCONTENT_BASE_DIR "${THIRDPARTY_DIR}")

# Fetch the miniaudio Repository and install it
FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG "master"
  GIT_SHALLOW TRUE
  GIT_PROGRESS ON
  SYSTEM
)
FetchContent_MakeAvailable(miniaudio)

# Fetch the STB-Image Repository and install it
FetchContent_Declare(
  stb_image
  GIT_REPOSITORY https://git.staiduhar.com/arizotaz/stb_image-CMake.git
  GIT_TAG "master"
  GIT_SHALLOW TRUE
  GIT_PROGRESS ON
  SYSTEM
)
FetchContent_MakeAvailable(stb_image)

# Fetch the miniaudio Repository and install it
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
  GIT_TAG "master"
  GIT_SHALLOW TRUE
  GIT_PROGRESS ON
  SYSTEM
)
FetchContent_MakeAvailable(freetype)


# If using the Windows compiler
if (MSVC OR USE_FREEGLUT)

  if (APPLE)

    find_path(XQUARTZ_INCLUDE_DIR X11/Xlib.h
      PATHS /opt/X11/include
      NO_DEFAULT_PATH
    )

    if(NOT XQUARTZ_INCLUDE_DIR)
      message(FATAL_ERROR "To use FreeGLUT on Mac, you need XQuartz installed.  Please install it from https://www.xquartz.org/ or set the USE_FREEGLUT cache option to OFF")
    endif()

  endif()

  set(CMAKE_POLICY_VERSION_MINIMUM 3.16)

  # Download freeglut
	FetchContent_Declare(
		freeglut
		GIT_REPOSITORY https://github.com/freeglut/freeglut.git
    GIT_TAG "v3.8.0"
		GIT_SHALLOW TRUE
		GIT_PROGRESS ON
	)

  # Force Freeglut to be a static library
	set(FREEGLUT_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(FREEGLUT_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

  # Force Cocca on Mac
  if(APPLE)
    set(FREEGLUT_USE_X11 OFF CACHE BOOL "" FORCE)
    set(FREEGLUT_COCOA ON CACHE BOOL "" FORCE)
  endif()

	FetchContent_MakeAvailable(freeglut)


  # Download Glad
	FetchContent_Declare(
		glad
		GIT_REPOSITORY https://github.com/Dav1dde/glad.git
		GIT_TAG "c"
		GIT_SHALLOW TRUE
		GIT_PROGRESS ON
	)

	FetchContent_MakeAvailable(glad)

endif()

# Tell Cmake to find the locations of libraries
find_package(OpenGL REQUIRED)

# Only require glut if on macos and not using freeglut
if (APPLE AND NOT USE_FREEGLUT)
  find_package(GLUT REQUIRED)
endif()
# Define MY_SOURCES to be a list of all the source files for my game 
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


add_executable("${CMAKE_PROJECT_NAME}")
add_executable("${CMAKE_PROJECT_NAME}_runner")

# Set the C++ standard to 17
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)
set_property(TARGET "${CMAKE_PROJECT_NAME}_runner" PROPERTY CXX_STANDARD 17)

#========================================================================================
# CHANGE RESOURCES DEFINITION
#========================================================================================
# You will change which line is commented depending
# on if the compiled binary is for development or
# production. 
#
# Uncomment the line below when in development, this uses the resources folder
# in the CMake project for the RESOURCES_PATH definition
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
#
# Uncomment the line below when in production, this uses a folder in the same working
# directory of the binary called "resources".  Effectively RESOURCES_PATH
# becomes ./resources/.  You would copy the resources folder of the cmake project
# and ship it will the compiled binary
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")
#




# Add our sources
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC EDITOR_BUILD=1)
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC EDITOR_BUILD=0)

target_sources("${CMAKE_PROJECT_NAME}_runner" PRIVATE ${MY_SOURCES})
set_target_properties("${CMAKE_PROJECT_NAME}_runner" PROPERTIES
    OUTPUT_NAME "game_runner"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

# If on mac
if (APPLE)

# FreeGlut for mac
if (USE_FREEGLUT)


target_sources("${CMAKE_PROJECT_NAME}" PRIVATE "${glad_SOURCE_DIR}/src/glad.c")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${glad_SOURCE_DIR}/include")
target_sources("${CMAKE_PROJECT_NAME}_runner" PRIVATE "${glad_SOURCE_DIR}/src/glad.c")
target_include_directories("${CMAKE_PROJECT_NAME}_runner" PUBLIC "${glad_SOURCE_DIR}/include")

# Add our include files
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/include/"
"${freeglut_SOURCE_DIR}/include"
"${glad_SOURCE_DIR}/include"
)
target_include_directories("${CMAKE_PROJECT_NAME}_runner" PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/include/"
"${freeglut_SOURCE_DIR}/include"
"${glad_SOURCE_DIR}/include"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC use_freeglut=TRUE)
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC use_freeglut=TRUE)

target_link_libraries("${CMAKE_PROJECT_NAME}" 
  PRIVATE 
  OpenGL::GL
  freeglut_static
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreVideo"
  "-lpthread"
  stb_image
  miniaudio
  freetype
)
target_link_libraries("${CMAKE_PROJECT_NAME}_runner" 
  PRIVATE 
  OpenGL::GL
  freeglut_static
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreVideo"
  "-lpthread"
  stb_image
  miniaudio
  freetype
)

# Native GLUT for Mac
else()

# Add our include files
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}_runner" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Disable different warning (Glut is really old, we already know that)
target_compile_options("${CMAKE_PROJECT_NAME}"
  PRIVATE
  -Wno-deprecated-declarations
	-Wdelete-incomplete
  -Wno-write-strings
	$<$<CXX_COMPILER_ID:Clang>:-Wno-pedantic -Wno-macro-redefined>
)
target_compile_options("${CMAKE_PROJECT_NAME}_runner"
  PRIVATE
  -Wno-deprecated-declarations
	-Wdelete-incomplete
  -Wno-write-strings
	$<$<CXX_COMPILER_ID:Clang>:-Wno-pedantic -Wno-macro-redefined>
)

# Include the libraries loaded earlier
target_link_libraries("${CMAKE_PROJECT_NAME}" 
  PRIVATE 
  OpenGL::GL 
  "-framework GLUT" 
  "-lpthread"
  "-lm"
  stb_image
  miniaudio
  freetype
)
target_link_libraries("${CMAKE_PROJECT_NAME}_runner" 
  PRIVATE 
  OpenGL::GL 
  "-framework GLUT" 
  "-lpthread"
  "-lm"
  stb_image
  miniaudio
  freetype
)
endif()


# If on windows
elseif (MSVC)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#remove console
	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set_property(TARGET "${CMAKE_PROJECT_NAME}_runner" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
set_property(TARGET "${CMAKE_PROJECT_NAME}_runner" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


target_sources("${CMAKE_PROJECT_NAME}" PRIVATE "${glad_SOURCE_DIR}/src/glad.c")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${glad_SOURCE_DIR}/include")
target_sources("${CMAKE_PROJECT_NAME}_runner" PRIVATE "${glad_SOURCE_DIR}/src/glad.c")
target_include_directories("${CMAKE_PROJECT_NAME}_runner" PUBLIC "${glad_SOURCE_DIR}/include")


# Add our include files
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/include/"
"${freeglut_SOURCE_DIR}/include"
"${glad_SOURCE_DIR}/include"
)
target_include_directories("${CMAKE_PROJECT_NAME}_runner" PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/include/"
"${freeglut_SOURCE_DIR}/include"
"${glad_SOURCE_DIR}/include"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC use_freeglut=TRUE)
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC FREEGLUT_STATIC)
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC use_freeglut=TRUE)
target_compile_definitions("${CMAKE_PROJECT_NAME}_runner" PUBLIC FREEGLUT_STATIC)

target_link_libraries("${CMAKE_PROJECT_NAME}" 
  PRIVATE 
  OpenGL::GL
  freeglut_static
  opengl32
  winmm
  gdi32
  stb_image
  miniaudio
  freetype
)
target_link_libraries("${CMAKE_PROJECT_NAME}_runner" 
  PRIVATE 
  OpenGL::GL
  freeglut_static
  opengl32
  winmm
  gdi32
  stb_image
  miniaudio
  freetype
)

elseif(UNIX AND NOT APPLE)

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE "${glad_SOURCE_DIR}/src/glad.c")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${glad_SOURCE_DIR}/include")

# Add our include files
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/include/"
"${freeglut_SOURCE_DIR}/include"
"${glad_SOURCE_DIR}/include"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC use_freeglut=TRUE)

target_link_libraries("${CMAKE_PROJECT_NAME}" 
  PRIVATE 
  OpenGL::GL
  freeglut_static
  stb_image
  miniaudio
  freetype
)

endif()