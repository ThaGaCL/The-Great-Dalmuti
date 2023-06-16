#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include "dgram.h"
#include "msgManip.h"


int create_socket(char* ip,int door){
    struct sockaddr_in saddr;
    int s;

    s = socket(AF_INET, SOCK_DGRAM, 0);  	//IPPROTO_UDP para loopback

    if (s == -1) {
        printf("Erro no Socket\n");
        exit(-1);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(door);
    saddr.sin_addr.s_addr = inet_addr(ip);

    if(bind(s, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
        printf("Erro no bind\n");
        return -1;
    }

   
    return s;
}

FILE* openFile(char* path,char* action){

    FILE *arq;    
    arq = fopen(path,action);
    if(!arq)
        return NULL;
    
    return arq;

}

int getHostIp(char** host_ip){

    char hostbuffer[256];

	struct hostent *host_entry;
	gethostname(hostbuffer, sizeof(hostbuffer));

	host_entry = gethostbyname(hostbuffer);

	*host_ip= inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

    return 0;

}

int init_info(info_t* info,int numPlayers){

    info->numPlayers =numPlayers;
    info->maxValue=13;
    info->numCards=0;

    return 0;

}

void replaceFirst(char* s,char c1, char c2){

    char *p=strchr(s,c1);
    *p=c2;

}



int getInitInfo(char *hostId,char *destId,info_t* info,int* door){

    FILE *arq = openFile("dados.txt","r");
    char line[LINESIZE+1];
    char *host_ip;
    int numPlayers;

    fgets(line, LINESIZE, arq);
    numPlayers = atoi(line);

    init_info(info,numPlayers);

    fgets(line, LINESIZE, arq);
    *door=atoi(line);

    getHostIp(&host_ip);    

    for(int i=0;i<numPlayers;i++){

        fgets(line, LINESIZE, arq);
        replaceFirst(line,'\n','\0');
        strcpy(info->idIps[i],line);
        if(strcmp(line,host_ip)==0)
            *hostId = i;

    }

    *destId=(*hostId+1)%numPlayers;
        

    fclose(arq);

    return 0;
}

struct sockaddr_in init_sockaddrin(info_t* info,char destId,int door){

    struct sockaddr_in saddr;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(door);
    saddr.sin_addr.s_addr = inet_addr(info->idIps[(int)destId]);

    return saddr;

}

int main_2(){    

    char hostId,destId;
    int door;
    info_t info;

    getInitInfo(&hostId,&destId,&info,&door);

    for(int i=0;i<info.numPlayers;i++)
        printf("id %d tem ip %s\n",i,info.idIps[i]);
    
    printf("meu ip:%s\n",info.idIps[(int)hostId]);
    printf("ip da maquina destino:%s\n",info.idIps[(int)destId]);
   

    //---------------------------------------------------------------------------------/
    //essa parte esta escrevendo o arquivo dados2.txt com os ips em ordem invertida, apenas para teste
    FILE *arq = openFile("dados2.txt","w");

    fprintf(arq,"%d\n",info.numPlayers);//imprime o num de players
    fprintf(arq,"%d\n",door);//imprime a porta
    
    for(int i=info.numPlayers-1;i>=0;i--)//imprime os ids em ordem invertida
        fprintf(arq,"%s\n",info.idIps[i]);
    
     fclose(arq);

    //---------------------------------------------------------------------------------/

    char buffer[MSGSIZE];
    int jog[13],size;
    for(int i=0;i<13;i++)
        jog[i]=i;
    

    mensagem_t m,m2;
    setMsgAttr(&m,hostId,DISTRIBCARDS,jog , "1101");

    size=fillBuffer(&m,  buffer,info.numPlayers);
    //printf("buffer: <%s>\n",buffer);
    for(int i=0;i<size;i++)
        printf("%d ",buffer[i]);
    printf("\n");

    separateMessage(&m2,buffer,info.numPlayers);

    
    //--------------------------------------------------------------------------------/
    int s = create_socket(info.idIps[(int)hostId],door);
    struct sockaddr_in saddr = init_sockaddrin(&info, destId, door),raddr;
    
    socklen_t len = (socklen_t )sizeof(saddr);
    memset(buffer, '\0', sizeof(buffer));


    if(hostId==0){
        fgets(buffer,MSGSIZE,stdin);
        if (sendto(s, buffer, strlen(buffer), 0,(struct sockaddr*)&saddr, len) < 0){
            printf("erro no envio\n");
            return -1;
        }

    }

    //para receber a mensagem por meio do buffer
    if (recvfrom(s, buffer, sizeof(buffer), 0,(struct sockaddr*)&raddr, &len) < 0){
        printf("erro no recebimento\n");
        return -1;
    }

    printf("recebido:%s\n",buffer);
    
    //para mandar a mensagem do buffer
    if (sendto(s, buffer, strlen(buffer), 0,(struct sockaddr*)&saddr, len) < 0){
        printf("erro no envio\n");
        return -1;
    }

    printf("mandado:%s\n",buffer);
        
    //----------------------------------------------------------------------------------------
    close(s);


    return 0;

}
