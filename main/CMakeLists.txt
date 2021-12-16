set(component_srcs "main.c" "tea5767.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS ".")

