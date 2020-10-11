
static const string BACKGROUND = "#2a2f3a";
static const string FOREGROUND = "#ffffff";

string COLORS[16] = {
    "#30,          03030",
    "#e1321a",
    "#6ab017",
    "#ffc005",
    "#004f9e",
    "#ec0048",
    "#2aa7e7",
    "#f2f2f2",

    "#5d5d5d",
    "#e1321a",
    "#7bc91f",
    "#ffd00a",
    "#0071ff",
    "#ff1d62",
    "#4bb8fd",
    "#a020f0"
};

static const bool PYWAL = true;

static const Block BLOCKS[] = {
    //Command,                  ColNum         	PrinTime    Signal
    {"i3sb-nettraf",          	5,	            2,          0},
    {"i3sb-pulseaudio",         2,	            -1,         1},
    {"i3sb-wifi",          	    1,	            60,         0},
    {"i3sb-date",          	    3,	            60,         0},
    {"i3sb-battery",          	4,	            10,         0},
};
