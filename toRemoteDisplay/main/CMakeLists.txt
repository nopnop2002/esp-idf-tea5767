set(srcs "main.c" "keyin.c" "radio.c" "udp_client.c")

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")

