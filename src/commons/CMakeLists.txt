add_library(${COMMONS_LIB_NAME} OBJECT
        "utils.c"
        "cli_helper.c"
        "log_service.c")

target_include_directories(${COMMONS_LIB_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/commons")

target_link_libraries(${COMMONS_LIB_NAME} PRIVATE ${LOG_LIB_NAME})

target_compile_definitions(${COMMONS_LIB_NAME} PUBLIC LOG_CONFIG_FILE="./config/log.conf")