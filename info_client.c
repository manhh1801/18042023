#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{

    /*<Setting up socket and connection>*/

    int client=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in cl_addr;
    cl_addr.sin_family=AF_INET;
    cl_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cl_addr.sin_port=htons(9000);

    if(connect(client, (struct sockaddr*)&cl_addr, sizeof(cl_addr)))
    {
        perror("Connect failed.\n");
        return 1;
    }
    printf("Connected: %s:%d\n", inet_ntoa(cl_addr.sin_addr), ntohs(cl_addr.sin_port));

    /*<>*/


    /*<Communicating>*/

    char *buf; unsigned int buf_size=0;

    char name[64];
    printf("Computer's name: ");
    scanf("%s", name);
    strcat(buf, name);
    strcat(buf, "\0");
    buf_size+=(strlen(name)+1);

    unsigned short disk_count;
    printf("Number of disk: ");
    scanf("%d", &disk_count);
    getchar();

    for(int index=0; index<disk_count; index++)
    {
        char disk_name;
        printf("Type name of disk %d: ", index+1);
        scanf("%c", disk_name);
        strncat(buf, &disk_name, 1);

        unsigned short disk_size; char disk_size_str[2];
        printf("Type size of disk %d: ", index+1);
        scanf("%d", disk_size);
        disk_size_str[0]=(disk_size>>8)&0xFF;
        disk_size_str[1]=(disk_size>>0)&0xFF;
        strcat(buf, disk_size_str);

        buf_size+=3;
        getchar();
    }

    write(client, &buf_size, sizeof(unsigned int));
    write(client, buf, buf_size);

    printf("\n");

    /*<>*/


    /*<Close connection>*/

    close(client);
    return 1;

    /*<>*/

}