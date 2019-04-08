#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sockhelper.h"
#define MAX_CMD_LENGTH 512

// Concatenates commands for sending
// TODO fix ugly USER cmd
char* concat_cmd(const char* cmd, const char* args, const char* suffix)
{
    int     space_delim = 3;
    int     output_length;

    if(suffix[0] != '\0')
    {
        space_delim = 4 + strlen(args);
    }

    output_length = strlen(cmd)+strlen(args)+strlen(suffix)+space_delim+1;

    char* output = malloc(output_length);   // All chars + spaces + end of char
    memset(output, 0, output_length);
    strcat(strcat(strcpy(output, cmd), " "), args);

    if(suffix[0] != '\0')
    {
        strcat(strcat(strcat(output, " "), suffix), args);
    }

    strcat(output, "\r\n");

    return output;
}

// Login to IRC
int login(int socketfd, const char* user, const char* nick, const char* password)
{
    char*   full_user;
    char*   full_nick;
    char*   full_password;
    char*   full_login;
    char    response_holder[MAX_CMD_LENGTH];
    char    cap_msg[] = "CAP LS 302\r\n";
    int     login_length;

    full_password = concat_cmd("PASS", password, "");
    full_nick = concat_cmd("NICK", nick, "");
    full_user = concat_cmd("USER", user, "0 * :");

    login_length = strlen(full_password)+strlen(full_nick)+strlen(full_user)+1;
    full_login = malloc(login_length);
    memset(full_login, 0, login_length);
    strcat(strcat(strcpy(full_login, full_password), full_nick), full_user);

    send_info(socketfd, cap_msg, 12);
    // recv_info(socketfd, response_holder, MAX_CMD_LENGTH);
    // printf("Response: %s", response_holder);
    send_info(socketfd, full_login, login_length-1);    // -1 To remove \0
    // recv_info(socketfd, response_holder, MAX_CMD_LENGTH);
    // printf("Response: %s", response_holder);

    free(full_password);
    free(full_nick);
    free(full_user);
    free(full_login);

    return 0;
}