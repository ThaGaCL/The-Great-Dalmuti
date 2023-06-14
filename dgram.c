#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTA 2000
#define MSGSIZE 1000

int create_socket(struct sockaddr_in* saddr,char* ip){
    int s;

    s = socket(AF_INET, SOCK_DGRAM, 0);  	//IPPROTO_UDP para loopback

    if (s == -1) {
        printf("Erro no Socket\n");
        exit(-1);
    }

    saddr->sin_family = AF_INET;
    saddr->sin_port = htons(PORTA);
    saddr->sin_addr.s_addr = inet_addr(ip);

    if(bind(s, (struct sockaddr*)saddr, sizeof(*saddr)) < 0){
        printf("Erro no bind\n");
        return -1;
    }


    return s;
}

int main(){

    struct sockaddr_in saddr,caddr;
    int s = create_socket(&saddr,"127.0.0.1");//ip da maquina atual
    char buffer[MSGSIZE];
    socklen_t len = (socklen_t )sizeof(caddr);

    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(PORTA);
    caddr.sin_addr.s_addr = inet_addr("127.0.0.1");//ip da maquina destino

    memset(buffer, '\0', sizeof(buffer));

    //para receber a mensagem por meio do buffer
    if (recvfrom(s, buffer, sizeof(buffer), 0,(struct sockaddr*)&caddr, &len) < 0){
        printf("Couldn't receive\n");
        return -1;
    }
    
    //para mandar a mensagem do buffer
    if (sendto(s, buffer, strlen(buffer), 0,(struct sockaddr*)&caddr, len) < 0){
        printf("Can't send\n");
        return -1;
    }
    
    close(s);


    return 0;

}
