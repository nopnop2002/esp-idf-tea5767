set(srcs "main.c" "keyin.c" "radio.c" "hd44780.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")

