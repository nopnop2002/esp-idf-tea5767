set(srcs "main.c" "udp_listener.c" "led.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")
