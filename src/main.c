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
    int                 socketfd;
    struct addrinfo*    serverinfo;


    // Initializing sockets
    if(sock_init()!=0)
    {
        fprintf(stderr, "Error: Unable to initialize socket.");
        return EXIT_FAILURE;
    }

    // Getting IP from host
    if(getip(&serverinfo, irc_hostname, irc_port, AF_UNSPEC, SOCK_STREAM) < 0)
    {
        return EXIT_FAILURE;
    }

    // Establishing connection
    if(make_sock(&serverinfo, &socketfd, 1) < 0)
    {
        return EXIT_FAILURE;
    }

    printf("Logging in ...\n");

    FILE* fpass;
    if((fpass = fopen("conf.dat","r")) == NULL)
    {
        fprintf(stderr, "Error while opening config file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    fscanf(fpass, "%36s", irc_password);

    login(socketfd, irc_user, irc_nick, irc_password);

    system("PAUSE");
    sock_free();
    return EXIT_SUCCESS;
}