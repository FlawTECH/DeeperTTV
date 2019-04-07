#include <stdlib.h>
#include <string.h>
#include "sockhelper.h"

// Concatenates commands for sending
// TODO fix ugly USER cmd
char* concat_cmd(const char* cmd, const char* args, const char* suffix)
{
    int     space_delim = 3;

    if(suffix[0] != '\0')
    {
        space_delim = 4 + strlen(args);
    }

    char* output = malloc(strlen(cmd)+strlen(args)+strlen(suffix)+space_delim+1);   // All chars + spaces + end of char
    strcat(strcat(strcpy(output, cmd), " "), args);

    if(suffix[0] != '\0')
    {
        strcat(strcat(strcat(output, " "), suffix), args);
    }

    strcat(output, "\r\n\0");

    return output;
}

// Login to IRC
int login(int socketfd, const char* user, const char* nick, const char* password)
{
    char*   full_user;
    char*   full_nick;
    char*   full_password;

    full_password = concat_cmd("PASS", password, "");
    full_nick = concat_cmd("NICK", nick, "");
    full_user = concat_cmd("USER", user, "0 * :");

    send_info(socketfd, full_password, strlen(full_password));
    send_info(socketfd, full_nick, strlen(full_nick));
    send_info(socketfd, full_user, strlen(full_user));

    free(full_password);
    free(full_nick);
    free(full_user);

    return 0;
}