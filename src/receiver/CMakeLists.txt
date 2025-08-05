add_library(${RECEIVER_LIB_NAME} OBJECT "main.c" "stat_service.c")

target_include_directories(${RECEIVER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/commons")
target_include_directories(${RECEIVER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/receiver")

target_sources(${RECEIVER_LIB_NAME} PRIVATE $<TARGET_OBJECTS:${COMMONS_LIB_NAME}>)

target_link_libraries(${RECEIVER_LIB_NAME} PUBLIC ${AHOI_LIB_NAME} ${LOG_LIB_NAME})

target_compile_definitions(${RECEIVER_LIB_NAME} PUBLIC RECEIVER_MODEM_ID=${RECEIVER_MODEM_ID})
