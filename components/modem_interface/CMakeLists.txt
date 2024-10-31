file(GLOB modem_api_lib modem_api/*.c )
idf_component_register(SRCS "modem_interface.c" ${modem_api_lib}
                    INCLUDE_DIRS "include" "modem_api" "."
                    REQUIRES driver common_variable_handler )

