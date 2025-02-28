set(srcs "main.c")

if (CONFIG_TEACHING)
    list(APPEND srcs "teaching.c")
elseif (CONFIG_RADIO)
    list(APPEND srcs "rcswitch.c" "radio.c" "udp_client.c")
endif()

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")

