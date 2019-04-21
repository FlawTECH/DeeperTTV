#ifndef _DEEPER_IRCHELPER_H_
    #define IRC_MAX_MSG_LEN 512
#define _DEEPER_IRCHELPER_H_

    struct msg_list
    {
        char* msg;
        struct msg_list* next;
        struct msg_list* prev;
    };

    int login(int socketfd, const char* user, const char* nick, const char* password);
    int recv_msg(int socketfd, struct msg_list* messages, char* overflow);

#endif