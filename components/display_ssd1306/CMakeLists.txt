#set(component_srcs "../utilities/ssd1306.c" "../utilities/ssd1306_spi.c")
#set(EXTRA_COMPONENT_DIRS ../displays_ssd1306/display_api)

#file(GLOB display_api_lib display_api/*.c )

# get IDF version for comparison
#set(idf_version "${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}")

#if (idf_version VERSION_GREATER_EQUAL "5.2")
#    if (CONFIG_LEGACY_DRIVER)
#        list(APPEND component_srcs "/display_api/ssd1306_i2c_legacy.c")
#    else ()
#        list(APPEND component_srcs "/display_api/ssd1306_i2c_new.c")
#    endif ()
#else ()
#    list(APPEND component_srcs "/display_api/ssd1306_i2c_legacy.c")
#endif ()


#set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../display_api)
file(GLOB display_api_lib display_api/*.c)

set(dependencies
        driver
        common_variable_handler
)

set(private_dependencies

)

idf_component_register(SRCS "display_ssd1306.cpp" ${display_api_lib}
        INCLUDE_DIRS "include" "." "display_api"
        REQUIRES ${dependencies}
        PRIV_REQUIRES ${private_dependencies})



