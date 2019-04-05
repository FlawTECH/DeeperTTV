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

void inithints(struct addrinfo* hints, int family, int type)
{
    memset(&hints, 0, sizeof(hints));
    hints->ai_family = family;
    hints->ai_socktype = type;
}

int getip(struct addrinfo** servinfo, const char* hostname, const char* port, int ipfamily, int socktype)
{
    struct addrinfo     hints;
    int                 status;
    
    inithints(&hints, ipfamily, socktype);
    if((status = getaddrinfo(hostname, port, &hints, servinfo)) != 0) {
        fprintf(stderr, "Unable to resolve Twitch's IRC IP address: %s", gai_strerror(status));
        return 1;
    }
    return 0;
}