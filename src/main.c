#include <stdio.h>

int main(int argc, char const *argv[])
{
    char irc_nick[] = "NICK neminamonem";
    char irc_user[] = "USER neminamonem 0 * neminamonem";
    char irc_password[100];

    //Fetching password from file
    FILE* fpass;
    fpass = fopen("conf.dat","r");
    fscanf(fpass, "%99s", irc_password);
    
    return 0;
}