idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")

spiffs_create_partition_assets(
    lottie
    "../assets"
    FLASH_IN_PROJECT
    MMAP_FILE_SUPPORT_FORMAT ".json"
)
