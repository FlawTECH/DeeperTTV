#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501 //Windows XP
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
#endif

#include <stdio.h>
#include "include/sockhelper.h"
#include "include/irchelper.h"

int main(int argc, char const *argv[])
{
    char                irc_nick[] = "deeperttv";
    // char                irc_user[] = "USER deeperttv 0 * :deeperttv\r\n";
    char                irc_user[] = "deeperttv";
    char                irc_password[37];
    const char          irc_hostname[] = "irc.chat.twitch.tv";
    const char          irc_port[] = "6667";
    char*               recv_buffer;
    int                 status = 0;
    int                 socketfd;
    struct addrinfo*    serverinfo;
    
    //Initializing sockets
    if(sock_init()!=0)
    {
        fprintf(stderr, "Error: Unable to initialize socket.");
        return EXIT_FAILURE;
    }

    //Getting IP from host
    if(getip(&serverinfo, irc_hostname, irc_port, AF_UNSPEC, SOCK_STREAM) < 0)
    {
        return EXIT_FAILURE;
    }

    //Establishing connection
    if(make_sock(&serverinfo, &socketfd) < 0)
    {
        return EXIT_FAILURE;
    }

    printf("Logging in ...\n");

    FILE* fpass;
    fpass = fopen("conf.dat","r");
    fscanf(fpass, "%36s", irc_password);
    
    login(socketfd, irc_user, irc_nick, irc_password);

    system("PAUSE");
    sock_free();
    return EXIT_SUCCESS;

    //Fetching oauth token from file
    // join_string = malloc(37*sizeof(char));
    // strcpy(irc_password, "PASS ");
    // strcat(strcat(irc_password, join_string), "\r\n");
    // free(join_string);
    // send(socketfd, irc_password, strlen(irc_password), 0); //PASS

    // join_string = malloc(sizeof(irc_nick)+7);
    // strcat(strcat(strcpy(join_string, "NICK "), irc_nick), "\r\n");
    // send(socketfd, join_string, strlen(irc_nick)+7, 0); //NICK
    // free(join_string);

    // send(socketfd, irc_user, strlen(irc_user), 0); //USER
    // printf("Login successful.\n");

    // status = 1;

    // //Joining own channel
    // join_string = malloc(sizeof(irc_nick)+8);
    // strcat(strcat(strcpy(join_string, "JOIN #"), irc_nick), "\r\n");
    // send(socketfd, join_string, strlen(join_string), 0);
    // free(join_string);

    // printf("Joined #%s\n", irc_nick);
    // recv_buffer = malloc(4096);
    // while(status == 1) {
    //     recv(socketfd, recv_buffer, 4096, 0);
    //     printf("DEBUG: %s\n", recv_buffer);
    // }

}