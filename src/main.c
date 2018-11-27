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
    char                irc_user[] = "USER neminamonem 0 * neminamonem";
    char                irc_password[42]; //oauth + PASS
    char                irc_hostname[] = "irc.chat.twitch.tv";
    char*               join_string;
    unsigned int        irc_port = 6667;
    int                 connected = 0;
    int                 sockd;
    struct hostent*     host;
    struct sockaddr_in  addr;

    if(sock_init()!=0)
    {
        fprintf(stderr, "Error: Unable to initialize Winsock.");
        return -1;
    }

    //Fetching oauth token from file
    strcpy(irc_password, "PASS ");
    FILE* fpass;
    fpass = fopen("conf.dat","r");
    char* tmp_pass;
    tmp_pass = malloc(37*sizeof(char));
    fscanf(fpass, "%36s", tmp_pass);
    strcat(irc_password, tmp_pass);
    free(tmp_pass);
    
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
    int answer = send(sockd, irc_password, sizeof(irc_password), 0); //PASS

    join_string = malloc(sizeof(irc_nick)+5);
    strcpy(join_string, "NICK ");
    strcat(join_string, irc_nick);
    answer = send(sockd, join_string, sizeof(join_string), 0); //NICK
    free(join_string);

    answer = send(sockd, irc_user, sizeof(irc_user), 0); //USER
    printf("Connected !\n");

    //Joining own channel
    join_string = malloc(sizeof(irc_nick)+5);
    strcpy(join_string, "JOIN ");
    strcat(join_string, irc_nick);
    answer = send(sockd, join_string, sizeof(join_string), 0);
    free(join_string);

    printf("Joined #%s\n", irc_nick);
    system("pause");
    sock_free();
    return 0;
}