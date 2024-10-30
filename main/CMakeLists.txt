idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES modem_interface uart_forwarder common_variable_handler
                    PRIV_REQUIRES freertos )

