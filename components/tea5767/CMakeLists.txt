set(srcs "tea5767.c")

idf_component_register(SRCS "${srcs}" PRIV_REQUIRES driver INCLUDE_DIRS ".")
