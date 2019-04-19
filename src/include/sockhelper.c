#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0501 //Windows XP
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>

#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
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
    memset(hints, 0, sizeof(*hints));
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
        return -1;
    }
    return 0;
}

int make_sock(struct addrinfo** servinfo, int* socketfd, u_long nonblocking)
{
    int     status = 0;
    // Filling data for socket
    if((*socketfd = socket((*servinfo)->ai_family, (*servinfo)->ai_socktype, (*servinfo)->ai_protocol)) < 0)
    {
        fprintf(stderr, "Unable to initialize socket: %s", strerror(errno));
        return -1;
    }

    // Non blocking socket
    if(nonblocking == 1)
    {
        #ifdef _WIN32
            ioctlsocket(socketfd, FIONBIO, &nonblocking);
        #else
            fcntl(socketfd, F_SETFL, O_NONBLOCK);
        #endif
    }

    // Establishing connection
    if((status = connect(*socketfd, (*servinfo)->ai_addr, (*servinfo)->ai_addrlen)) < 0)
    {
        fprintf(stderr, "Unable to connect to remote host: %s", strerror(errno));
        return -1;
    }
    return 0;
}

int send_info(int socketfd, void* info, int length)
{
    char*   data = (char*) info;
    int     i;

    while(length > 0)
    {
        i = send(socketfd, data, length, 0);
        if(i < 1)
        {
            return -1;
        }
        data+=i;
        length-=i;
    }
    return 0;
}

int recv_info(int socketfd, char* response, int expected_length)
{
    int i;
    int original_length = expected_length;

    while(expected_length > 0)
    {
        i = recv(socketfd, response, expected_length, 0);
        if(i < 1)
        {
            fprintf(stderr, "No data.\n");
            return -1;
        }

        response+=i;
        expected_length-=i;
    }

    response-=original_length;
    return 0;
}