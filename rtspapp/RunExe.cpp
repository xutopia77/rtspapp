

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_INFO	printf
#define LOG_ERR		printf
#define LOG_OFTEN	printf
#define LOG_DEBUG	printf
#define LOG_RUNLINE	LOG_DEBUG("run line %s#%d\n",__FILE__,__LINE__)

#define LOG_REALIZE	LOG_ERR("not realize\n")


#define fatal(s)	do{LOG_ERR("%s",s);exit(-1);}while(0)

void runSrv(){
    pid_t pid;
    if ((pid = fork()) < 0) {
            fatal("ERROR: can't create process: fork\n");
        }
    if (pid == 0) {
        int iret=execl("./live555MediaServer", "main", NULL);
        if(iret == 0){
            LOG_INFO("run %s succ\n", "live555MediaServer");
        }
        else{
            LOG_ERR("run %s err %d, %s\n", "live555MediaServer", errno, strerror(errno));
        }
        exit(0);
    }
}

void runClient(){
    pid_t pid;
    if ((pid = fork()) < 0) {
            fatal("ERROR: can't create process: fork\n");
        }
    if (pid == 0) {
        int iret=execl("./client", "client", NULL);
        if(iret == 0){
            LOG_INFO("run %s succ\n", "client");
        }
        else{
            LOG_ERR("run %s err %d, %s\n", "client", errno, strerror(errno));
        }
        exit(0);
    }
}











// void start_run_rtmp_publish(const char* runapp, const char* app, const char* stream, const char* pushFile){
//     pid_t pid;
//     if ((pid = fork()) < 0) {
//             fatal("ERROR: can't create process: fork\n");
//         }
//     if (pid == 0) {
//         int iret=execl("./main", "main","-c", runapp, "-a", app, "-s", stream, "-w", pushFile, NULL);
//         if(iret == 0){
//             LOG_INFO("run %s succ\n", runapp);
//         }
//         else{
//             LOG_ERR("run %s err %d, %s\n", runapp, errno, strerror(errno));
//         }
//         exit(0);
//     }
// }