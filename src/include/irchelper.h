#ifndef _DEEPER_IRCHELPER_H_
    #define IRC_MAX_MSG_LEN 512
#define _DEEPER_IRCHELPER_H_
#endif

int login(int socketfd, const char* user, const char* nick, const char* password);
int recv_msg(int socketfd, char* msg);