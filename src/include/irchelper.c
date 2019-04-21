#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "irchelper.h"
#include "sockhelper.h"

free_messages(struct msg_list* messages)
{
    if(messages->next == NULL)
    {
        free(messages->msg);
    }
    while(messages->next != NULL)
    {
        messages = messages->next;
        messages->prev = 0;
        free(messages->msg);        
        free(messages->prev);
    }
}

// Translates raw data into IRC messages
int recv_msg(int socketfd, struct msg_list* messages, char* overflow)
{
    char                tmp_message[IRC_MAX_MSG_LEN+1] = "";
    char*               eol_pointer;
    struct msg_list*    msg_walker;
    int                 msg_length;
    int                 msg_count = 0;

    msg_walker = messages;

    if(recv_info(socketfd, tmp_message, IRC_MAX_MSG_LEN)>0)
    {
        // Looking for EOL according to the IRC spec (\r\n)
        eol_pointer = strchr(tmp_message, '\r');
        while(eol_pointer != NULL)
        {
            if(eol_pointer-tmp_message < IRC_MAX_MSG_LEN)
            {
                // Found EOL
                if(eol_pointer+1 == '\n')
                {
                    // TODO: malloc messages
                    msg_count+=1;
                    
                    if(msg_count > 1)
                    {
                        malloc(msg_walker->next);
                        msg_walker->next->prev = msg_walker;
                        msg_walker = msg_walker->next;
                    }

                    //TODO determine msg length and malloc msg
                }
            }
            eol_pointer = strchr(eol_pointer+1, '\r');
        }
        

    }

}

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
    char*               full_user;
    char*               full_nick;
    char*               full_password;
    char*               full_login;
    char*               overflow;
    int                 login_length;
    struct msg_list*    messages; 

    full_password = concat_cmd("PASS", password, "");
    full_nick = concat_cmd("NICK", nick, "");
    full_user = concat_cmd("USER", user, "0 * :");

    login_length = strlen(full_password)+strlen(full_nick)+strlen(full_user)+1;
    full_login = malloc(login_length);
    memset(full_login, 0, login_length);
    strcat(strcat(strcpy(full_login, full_password), full_nick), full_user);

    if(send_info(socketfd, full_login, login_length-1) < 0)    // -1 To remove \0
    {
        fprintf(stderr, "Unable to login.\n");
        return -1;
    }

    free(full_password);
    free(full_nick);
    free(full_user);
    free(full_login);

    // Printing server welcome message
    messages = malloc(sizeof(struct msg_list));
    if(recv_msg(socketfd, messages, overflow) > -1)
    {
        printf("%s", messages->msg);
    }

    return 0;
}