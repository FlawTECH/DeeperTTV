#include <stdio.h>
#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501 //Windows XP
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
#endif

int sock_init(void)
{
    #ifdef _WIN32
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(2,2), &wsa_data);
    #else
        return 0;
    #endif
}

int sock_free(void)
{
    #ifdef _WIN32
        return WSACleanup();
    #else
        return 0;
    #endif
}

int main(int argc, char const *argv[])
{
    char                irc_nick[] = "neminamonem";
    char                irc_user[] = "USER neminamonem 0 * :neminamonem\r\n";
    char                irc_password[44]; //oauth + PASS
    char                irc_hostname[] = "irc.chat.twitch.tv";
    char*               join_string; //used for various concatenations
    char*               recv_buffer;
    unsigned int        irc_port = 6667;
    int                 connected = 0;
    int                 sockd;
    struct hostent*     host;
    struct sockaddr_in  addr;

    if(sock_init()!=0)
    {
        fprintf(stderr, "Error: Unable to initialize socket.");
        return -1;
    }

    //Fetching oauth token from file
    strcpy(irc_password, "PASS ");
    FILE* fpass;
    fpass = fopen("conf.dat","r");
    join_string = malloc(37*sizeof(char));
    fscanf(fpass, "%36s", join_string);
    strcat(irc_password, join_string);
    strcat(irc_password, "\n\r");
    free(join_string);
    
    //Getting IP from host
    host = gethostbyname(irc_hostname);

    //Filling data for socket
    addr.sin_addr.s_addr = *(unsigned long*)host->h_addr_list[0];
    addr.sin_family = host->h_addrtype;
    addr.sin_port = htons((unsigned short)irc_port);
    sockd = socket(AF_INET, SOCK_STREAM, 0);

    //Establish connection
    printf("Connecting ...\n");
    connected = connect(sockd, (struct sockaddr*)&addr, sizeof(addr));
    printf("Logging in ...\n");
    send(sockd, irc_password, sizeof(irc_password)-1, 0); //PASS

    join_string = malloc(sizeof(irc_nick)+7);
    strcat(strcat(strcpy(join_string, "NICK "), irc_nick), "\r\n");
    send(sockd, join_string, sizeof(irc_nick)+6, 0); //NICK
    free(join_string);

    send(sockd, irc_user, sizeof(irc_user)-1, 0); //USER
    printf("Connected !\n");

    connected = 1;

    //Joining own channel
    join_string = malloc(sizeof(irc_nick)+8);
    strcat(strcat(strcpy(join_string, "JOIN #"), irc_nick), "\r\n");
    send(sockd, join_string, sizeof(join_string)-1, 0);
    free(join_string);

    printf("Joined #%s\n", irc_nick);
    recv_buffer = malloc(4096);
    while(connected == 1) {
        recv(sockd, recv_buffer, 4096, 0);
        printf("DEBUG: %s\n", recv_buffer);
    }
    sock_free();
    system("pause");
    return 0;
}