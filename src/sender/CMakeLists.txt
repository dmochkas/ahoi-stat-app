add_library(${SENDER_LIB_NAME} OBJECT "main.c" "stat_service.c")

target_include_directories(${SENDER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/commons")
target_include_directories(${SENDER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/sender")

target_sources(${SENDER_LIB_NAME} PRIVATE $<TARGET_OBJECTS:${COMMONS_LIB_NAME}>)

target_link_libraries(${SENDER_LIB_NAME} PUBLIC ${AHOI_LIB_NAME} ${LOG_LIB_NAME})