set(srcs "main.c" "radio.c")

if (CONFIG_NETWORK_HTTP)
	list(APPEND srcs "http_server.c")
elseif (CONFIG_NETWORK_MQTT)
	list(APPEND srcs "mqtt_sub.c")
endif()

idf_component_register(SRCS "${srcs}" INCLUDE_DIRS ".")

