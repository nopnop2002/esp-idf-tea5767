typedef struct {
	char ch;
	float frequency;
} COMMAND_t;

typedef struct {
	double current_freq;
	int stereo;
	int signal_level;
	int mute;
} STATUS_t;
