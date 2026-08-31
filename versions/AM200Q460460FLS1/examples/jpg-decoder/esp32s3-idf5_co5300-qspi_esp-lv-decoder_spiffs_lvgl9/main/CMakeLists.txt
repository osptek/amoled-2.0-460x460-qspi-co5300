idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")

spiffs_create_partition_assets(
    material
    "../assets"
    FLASH_IN_PROJECT
    MMAP_FILE_SUPPORT_FORMAT ".jpg"
    MMAP_SUPPORT_SJPG
    MMAP_SPLIT_HEIGHT 16
)