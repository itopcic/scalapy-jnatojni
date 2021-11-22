################################################################
# A minimal CMake file that is compatible with sbt-jni         #
#                                                              #
# All settings required by sbt-jni have been marked so, please #
# add/modify/remove settings to build your specific library.   #
################################################################

cmake_minimum_required(VERSION 3.12)

option(SBT "Set if invoked from sbt-jni" ON)

# Define project and related variables
# (required by sbt-jni) please use semantic versioning
#
project (scalapy)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 5)
set(PROJECT_VERSION_PATCH 1)

# Setup JNI
find_package(JNI REQUIRED)
if (JNI_FOUND)
    message (STATUS "JNI include directories: ${JNI_INCLUDE_DIRS}")
endif()

# Include directories
include_directories(.)
include_directories(include)
include_directories(${JNI_INCLUDE_DIRS})

find_package (Python COMPONENTS Development REQUIRED)
add_library(cpython SHARED ${Python_LIBRARIES})
set_target_properties(cpython PROPERTIES LINKER_LANGUAGE C)
include_directories(${PYTHON_INCLUDE_DIRS})
# include_directories(/home/topcic/Documents/SemProj/cpython/)
# include_directories(/home/topcic/Documents/SemProj/cpython/Include)

message(STATUS "Python libraries: ${Python_LIBRARIES}")
message(STATUS "Python includes: ${PYTHON_INCLUDE_DIRS}")



# Sources
file(GLOB LIB_SRC
  "*.c"
  "*.cc"
  "*.cpp"
)

# Setup installation targets
# (required by sbt-jni) major version should always be appended to library name
#
set (LIB_NAME ${PROJECT_NAME}${PROJECT_VERSION_MAJOR})
add_library(${LIB_NAME} SHARED ${LIB_SRC})
install(TARGETS ${LIB_NAME} LIBRARY DESTINATION .)
