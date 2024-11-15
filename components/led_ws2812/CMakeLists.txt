set(srcs
        #../display_api/display_ssd1306/display_ssd1306.cpp
)

set(dependencies
        log
        driver
        led_strip
)

set(private_dependencies

)

#set(EXTRA_COMPONENT_DIRS ../components/led_strip/ssd1306)

idf_component_register(SRCS "led_ws2812.c" ${srcs}
        INCLUDE_DIRS "." "include"
        REQUIRES ${dependencies}
        PRIV_REQUIRES ${private_dependencies})



