cmake_minimum_required(VERSION 3.10)
project(ahoi_serial C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(config.cmake)
include(GitAddSubmodule)

#set(COMMONS_LIB_NAME "commons-lib")
set(SENDER_LIB_NAME "sender-stat-objects")
set(SENDER_APP_NAME "sender")
set(RECEIVER_LIB_NAME "receiver-stat-objects")
set(RECEIVER_APP_NAME "receiver")

#add_subdirectory(src/ahoi_commons)

if (WITH_SENDER)
    add_subdirectory(src/sender)
    add_executable(${SENDER_APP_NAME} $<TARGET_OBJECTS:${SENDER_LIB_NAME}>)
endif ()


if (WITH_RECEIVER)
    add_subdirectory(src/receiver)
    add_executable(${RECEIVER_APP_NAME} $<TARGET_OBJECTS:${RECEIVER_LIB_NAME}>)
endif ()

if (ENABLE_TESTS)
    enable_testing()
    option(WITH_EXAMPLES "" OFF)
#    add_git_submodule(DIRECTORY "thirdparty/cmocka"
#            PROPAGATE ON)
#    add_subdirectory(tests)
endif ()