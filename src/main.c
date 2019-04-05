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
#include <errno.h>
#include "include/sockhelper.h"

int main(int argc, char const *argv[])
{
    char                irc_nick[] = "neminamonem";
    char                irc_user[] = "USER neminamonem 0 * :neminamonem\r\n";
    char                irc_password[44]; //oauth + PASS
    const char          irc_hostname[] = "irc.chat.twitch.tv";
    char*               join_string; //used for various concatenations
    char*               recv_buffer;
    const char          irc_port[] = "6667";
    int                 status = 0;
    int                 socketfd;
    int                 sockd;
    struct addrinfo*    serverinfo;
    
    //Initializing sockets
    if(sock_init()!=0)
    {
        fprintf(stderr, "Error: Unable to initialize socket.");
        return 1;
    }

    //Getting IP from host
    getip(&serverinfo, irc_hostname, irc_port, AF_UNSPEC, SOCK_STREAM);

    //Filling data for socket
    if(socketfd = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol) < 0) {
        fprintf(stderr, "Unable to initialize socket: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    //Establishing connection
    printf("Connecting ...\n");
    if(status = connect(socketfd, serverinfo->ai_addr, serverinfo->ai_addrlen) < 0) {
        fprintf(stderr, "Unable to connect to remote host: %s", strerror(errno));
    }
    printf("Connected.\n");

    printf("Logging in ...\n");

    //Fetching oauth token from file
    strcpy(irc_password, "PASS ");
    FILE* fpass;
    fpass = fopen("conf.dat","r");
    join_string = malloc(37*sizeof(char));
    fscanf(fpass, "%36s", join_string);
    strcat(strcat(irc_password, join_string), "\r\n");
    free(join_string);
    send(sockd, irc_password, strlen(irc_password), 0); //PASS

    join_string = malloc(sizeof(irc_nick)+7);
    strcat(strcat(strcpy(join_string, "NICK "), irc_nick), "\r\n");
    send(sockd, join_string, strlen(irc_nick)+7, 0); //NICK
    free(join_string);

    send(sockd, irc_user, strlen(irc_user), 0); //USER
    printf("Login successful.\n");

    status = 1;

    //Joining own channel
    join_string = malloc(sizeof(irc_nick)+8);
    strcat(strcat(strcpy(join_string, "JOIN #"), irc_nick), "\r\n");
    send(sockd, join_string, strlen(join_string), 0);
    free(join_string);

    printf("Joined #%s\n", irc_nick);
    recv_buffer = malloc(4096);
    while(status == 1) {
        recv(sockd, recv_buffer, 4096, 0);
        printf("DEBUG: %s\n", recv_buffer);
    }

    sock_free();
    return EXIT_SUCCESS;
}