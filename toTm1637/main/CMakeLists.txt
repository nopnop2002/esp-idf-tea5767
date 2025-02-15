set(srcs "main.c" "keyin.c" "radio.c" "led.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")

