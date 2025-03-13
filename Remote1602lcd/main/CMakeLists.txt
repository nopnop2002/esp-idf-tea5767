set(srcs "main.c" "udp_listener.c" "hd44780.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
