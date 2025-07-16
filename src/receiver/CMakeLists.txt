add_library(${RECEIVER_LIB_NAME} OBJECT "main.c")

#target_include_directories(${RECEIVER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/ahoi_serial")
#target_include_directories(${RECEIVER_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/priv")

#target_sources(${SENDER_LIB_NAME} PRIVATE $<TARGET_OBJECTS:${COMMONS_LIB_NAME}>)