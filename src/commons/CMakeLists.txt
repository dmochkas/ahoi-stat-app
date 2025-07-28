add_library(${COMMONS_LIB_NAME} OBJECT
        "utils.c"
        "cli_helper.c")

target_include_directories(${COMMONS_LIB_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/commons")