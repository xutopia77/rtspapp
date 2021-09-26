

/*从rtsp服务器接收数据，经过ffmpeg解码后，送入sdl显示
export LD_LIBRARY_PATH=/usr/local/lib
*/

#include "AppCfg.h"

#include "ffmpegParse.h"
#include <string>

#include "unistd.h"
#include <pthread.h>

#include "rtspApp.h"
#include <map>

using namespace std;

static void trim(std::string &s){
	int index = 0;
	if( !s.empty()){
		while( (index = s.find(' ',index)) != std::string::npos){
			s.erase(index,1);
		}
	}
}

static std::map<string, string> keyMap;

bool findInt     (std::string& desc, int&           val){
	std::string str(desc);  return findInt(str, val);
}
bool findStr     (std::string& desc, std::string&   val){
	std::string str(desc);  return findStr(str, val);
}
bool findInt     (const char*  desc, int&           val){
	std::map<std::string ,std::string>::iterator iter = keyMap.find(desc);
	if(iter == keyMap.end())
		return false;
	std::string strVal = iter->second;
	trim(strVal);
	val = std::atoi(strVal.c_str());
	return true;
}
bool findStr     (const char*  desc, std::string&   val){
	std::map<std::string ,std::string>::iterator iter = keyMap.find(desc);
	if(iter == keyMap.end())
		return false;
	std::string strVal = iter->second;
	trim(strVal);
	val = strVal;
	return true;
}

static AppCfg gAppCfg;
AppCfg* gpAppCfg = &gAppCfg;

static bool getParamFromMap(map<string, string>& paramMap){
	AppCfg& cfg = gAppCfg;
	gpAppCfg = &gAppCfg;
	
	user_assert(findInt("sourceType", cfg.sourceType));
	user_assert(findInt("sinkFileEnable", cfg.sinkFileEnable));
	user_assert(findInt("sinkSdlEnable", cfg.sinkSdlEnable));
	user_assert(findStr("inYuvFileName", cfg.inYuvFileName));
	user_assert(findStr("rtspUrl", cfg.rtspUrl));

	return true;
}

// "../dat/cfg.txt"
bool initParam(char* cfgFile){
	char bufLine[1024],bufDes[1024], bufVal[1024];
	FILE *fp = fopen(cfgFile, "r");
	if(fp==NULL){
		fatal("not find cfgFile %s\n", cfgFile);
	}
	
	while(!feof(fp)){
		fgets(bufLine, sizeof(bufLine) - 1, fp);
		if(bufLine[0] == '/' && (bufLine[1] == '/')){
			// LOG_INFO("comment: %s\n",bufLine);
			continue;
		}
		if(bufLine[0] == '\0'){
			continue;
		}
		if(strlen(bufLine) == 0){
			continue;
		}
		// LOG_DBG("line[%d]=%s\n", strlen(bufLine), bufLine);
		int ire = sscanf(bufLine,"%[^=]=%s",bufDes,bufVal);
		if( ire != 2 ){
			continue;
		}
		keyMap[bufDes] = bufVal;
	}
	return getParamFromMap(keyMap);
}

