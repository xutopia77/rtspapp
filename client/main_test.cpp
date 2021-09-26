
#include "xlog.h"

using namespace std;



extern int XAvframeTest(int argc, char const *argv[]);
extern int ffmpegParseTest(int argc, char const *argv[]);
extern int RtspSourceTest(int argc, char const *argv[]);

int mainTest(int argc, char const *argv[]) {
	if(1) return 0;
	
	// XAvframeTest(argc, argv);
	// ffmpegParseTest(argc, argv);
	RtspSourceTest(argc, argv);

	exit(0);
	return 0;
}