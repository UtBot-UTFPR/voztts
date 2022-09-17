### 1 - Required CMake Version
cmake_minimum_required(VERSION 2.8.3)
### 2- Package Name
project(voztts)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # add extra warnings
### 3 - Find other CMake/Catkin packages needed for build
SET(DEPS  roscpp  roslib  std_msgs)
find_package(catkin REQUIRED COMPONENTS ${DEPS})
# find all the libraries needed by libnotify
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake/Modules")
find_package(GDK-PixBuf REQUIRED)
find_package(Glib REQUIRED)
find_package(GObject REQUIRED)
find_package(Libnotify REQUIRED)
# nothing to do in this package
### 4 - Message/Service/Action Generators (add_message_files(), add_service_files(), add_action_files())
# nothing to do in this package
### 5 - Invoke message/service/action generation (generate_messages())
# nothing to do in this package
### 6 - Specify package build info export (catkin_package())
catkin_package(
  INCLUDE_DIRS   include
  CATKIN_DEPENDS ${DEPS}
  DEPENDS
)
### 7 - Libraries/Executables to build (add_library()/add_executable()/target_link_libraries())
SET(PKG_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/voztts)
include_directories(include
  ${catkin_INCLUDE_DIRS}
  ${GDK-PixBuf_INCLUDE_DIRS}
  ${Glib_INCLUDE_DIRS}
  ${GOBJECT_INCLUDE_DIR}
  # ${LIBNOTIFY_INCLUDE_DIR}
)

add_executable       (voztts.exe  src/voztts.cpp
  ${PKG_INC_DIR}/cached_files_map.h
  ${PKG_INC_DIR}/utils.h)
target_link_libraries(voztts.exe  ${catkin_LIBRARIES})

install(DIRECTORY include/${PROJECT_NAME}/
   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   FILES_MATCHING PATTERN "*.h"
   PATTERN ".svn" EXCLUDE
)
