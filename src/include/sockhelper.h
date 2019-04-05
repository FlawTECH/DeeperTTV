#ifndef _DEEPER_SOCKHELPER_H_
#define _DEEPER_SOCKHELPER_H_

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

int sock_init();
int sock_free();
int getip(struct addrinfo** servinfo, const char* hostname, const char* port, int ipfamily, int socktype);

#endif