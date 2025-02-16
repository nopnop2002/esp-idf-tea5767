idf_component_register(SRCS "main.c" "radio.c"
    INCLUDE_DIRS "."
    EMBED_FILES "../html/error.html"
		"../html/favicon.ico"
		"../html/seven_segment_display.js"
		"../html/speaker.png"
		"../html/main.js"
		"../html/root.html"
		"../html/main.css")
