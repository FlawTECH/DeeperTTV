#include <stdio.h>
#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501 //Windows XP
    #endif
    #include <winsock2.h>
#else
    #include <sys/socket.h>
#endif

int main(int argc, char const *argv[])
{
    char            irc_nick[] = "NICK neminamonem";
    char            irc_user[] = "USER neminamonem 0 * neminamonem";
    char            irc_password[100];
    char            irc_hostname[] = "irc.chat.twitch.tv";
    unsigned int    irc_port = 6667;
    struct hostent* host;
    int             connected = 0;

    //Fetching password from file
    FILE* fpass;
    fpass = fopen("conf.dat","r");
    fscanf(fpass, "%99s", irc_password);
    
    host = gethostbyname(irc_hostname);

    return 0;
}