#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    /*<Setting up socket>*/

    int server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sv_addr;
    sv_addr.sin_family=AF_INET;
    sv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    sv_addr.sin_port=htons(9000);

    if(bind(server, (struct sockaddr*)&sv_addr, sizeof(sv_addr)))
    {
        perror("Bind address to socket failed.\n");
        return 1;
    }

    /*<>*/


    /*<Waiting and accepting connection>*/

    if(listen(server, 5))
    {
        perror("Connect failed.\n");
        return 1;
    }

    struct sockaddr_in cl_addr;
    int cl_addr_length=sizeof(cl_addr);

    int client=accept(server, (struct sockaddr*)&cl_addr, &cl_addr_length);
    printf("Accepted connection: %s:%d\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));

    /*<>*/


    /*<Communicating>*/

    char *buf; unsigned int buf_size=0;

    read(client, &buf_size, sizeof(unsigned int));
    read(client, buf, buf_size);

    int index=1;

    char name[64];
    while(1)
    {
        char temp_char=*(buf+index-1);
        name[index++-1]=temp_char;
        if(temp_char=='\0') {break;}
    }
    printf("Computer's name: %s\n", name);

    while(index<buf_size-2)
    {
        char disk_name=buf[index];
        unsigned short disk_size;

        printf("%c - %d", disk_name, disk_size);
        index+=3;
    }

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    close(server);
    return 1;

    /*<>*/

}