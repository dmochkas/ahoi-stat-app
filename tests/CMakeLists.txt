set(UTILS_TEST_NAME "utils_test")

add_executable(${UTILS_TEST_NAME} utils_test.c)

target_link_libraries(${UTILS_TEST_NAME} cmocka ${LOG_LIB_NAME})

target_include_directories(${UTILS_TEST_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/commons")
target_sources(${UTILS_TEST_NAME} PRIVATE $<TARGET_OBJECTS:${COMMONS_LIB_NAME}>)

target_link_libraries(${UTILS_TEST_NAME})

add_test(NAME ${UTILS_TEST_NAME} COMMAND ${UTILS_TEST_NAME})