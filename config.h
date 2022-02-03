
#define STATUS_LENGTH	64

char* SEPARATOR_SYMBOL = " | ";

static const Block BLOCKS[] = {
	//Command,					PrintTime,	Signal
	{"i3sb-pulseaudio-source", 	-1,			1},
	{"i3sb-pulseaudio", 		-1,			1},
	{"i3sb-date",				60,			0},
	// {"i3sb-battery",	-1,			0},
};

static const int BLOCKS_SIZE = LEN(BLOCKS);
