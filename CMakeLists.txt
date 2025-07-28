cmake_minimum_required(VERSION 3.10)
project(ahoi_stat_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_VERBOSE_MAKEFILE ON)


include(config.cmake)
include(GitAddSubmodule)

set(AHOI_LIB_NAME "ahoi-serial-lib")
set(COMMONS_LIB_NAME "stat-commons")
set(SENDER_LIB_NAME "sender-stat-objects")
set(SENDER_APP_NAME "stat-sender")
set(RECEIVER_LIB_NAME "receiver-stat-objects")
set(RECEIVER_APP_NAME "stat-receiver")

add_git_submodule(DIRECTORY "thirdparty/ahoi-serial-lib"
                  PROPAGATE ON
                  CMP_TARGET ${AHOI_LIB_NAME})

add_subdirectory(src/commons)

if (WITH_SENDER)
    add_subdirectory(src/sender)
    add_executable(${SENDER_APP_NAME} $<TARGET_OBJECTS:${SENDER_LIB_NAME}> $<TARGET_OBJECTS:${COMMONS_LIB_NAME}>)
    target_link_libraries(${SENDER_APP_NAME} PUBLIC ${AHOI_LIB_NAME})
endif ()


if (WITH_RECEIVER)
    add_subdirectory(src/receiver)
    add_executable(${RECEIVER_APP_NAME} $<TARGET_OBJECTS:${RECEIVER_LIB_NAME}>)
endif ()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13.0")
    target_link_options(${SENDER_APP_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif ()

if (ENABLE_TESTS)
    enable_testing()
    include(CTest)

    option(WITH_EXAMPLES "" OFF)
    add_git_submodule(DIRECTORY "thirdparty/cmocka"
            PROPAGATE ON
            CMP_TARGET cmocka)
    add_subdirectory(tests)
endif ()