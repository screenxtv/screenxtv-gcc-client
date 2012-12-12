struct ScanMsg{const char*key,*msg,*value;};
ScanMsg SCAN_SETTINGS[]={
	{"url","Create a new URL. If given \"foo\", your URL will be \"http://screenx.tv/foo\".",""},
	{"screen",NULL,"screenxtv"},
	{"color","Terminal Color [BLACK/white/green/novel]","black"},
	{"title","Title","no title"},
	{"urlhash",NULL,""},
	{NULL,NULL,NULL}
};
