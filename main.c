#include "gameCtrl.h"
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
#include <time.h>
// TODO 
// 1. Implementar o jogador.h e jogador.c - Done
// 2. Implementar a função put_card - Done
// 3. Implementar a função check_round_winner - Done
// 4. Implementar a função check_game_winner - 
// 5. Guardar qual jogador jogou cada carta na rodada (usar o indice do deck da mesa) - Done
// 6. Implementar os turnos

int main(){
    srand(time(NULL));

    mensagem_t m,m2;
    char hostId,destId;
    int door,bastao=0,size;
    char buffer[MSGSIZE];
    char cr[MAXPLAYERS];
    info_t info;

    getInitInfo(&hostId,&destId,&info,&door);//cria o vetor com os ips e retorna os ids

    /*for(int i=0;i<info.numPlayers;i++)
        printf("id %d tem ip %s\n",i,info.idIps[i]);
    
    printf("meu ip:%s\n",info.idIps[(int)hostId]);
    printf("ip da maquina destino:%s\n",info.idIps[(int)destId]);impressao dos ips importantes*/

    

    int s = create_socket(info.idIps[(int)hostId],door);
    struct sockaddr_in saddr = init_sockaddrin(&info, destId, door),raddr;
    // Cria o vetor de ints que representa o deck e o preenche
    int *deck = (int *)malloc(sizeof(int)*info.numPlayers*DECK_SIZE);
    int *playerDeck = (int *)malloc(sizeof(int)*DECK_SIZE);
    int *playerDeckGive = (int *)malloc(sizeof(int)*DECK_SIZE);

    printf("%s============Jogo inicado!!============%s\n",CYN,RESET);

    fillDeck(deck);
    socklen_t len = (socklen_t )sizeof(saddr);
    memset(buffer, '\0', sizeof(buffer));

    //---------------------abertura do arquivo com o resultado------------------------
    FILE *arq = openFile("dados.txt","w");
    fprintf(arq,"%d\n",info.numPlayers);//imprime o num de players
    fprintf(arq,"%d\n",door);//imprime a porta

    if(hostId==0){
        bastao=1;
        distribuiCartas(deck, playerDeck, info.numPlayers);


        for(int i=1;i<info.numPlayers;i++){
            getCondRecDistrib(i,cr);
            distribuiCartas(deck, playerDeckGive, info.numPlayers);
            setMsgAttr(&m,hostId,DISTRIBCARDS, playerDeckGive,cr );
            size=fillBuffer(&m,  buffer,info.numPlayers);
          
            do{
                if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                    printf("erro no envio\n");
                    return -1;
                }

            }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,DISTRIBCARDS,info.numPlayers)<0);//se hoube erro, reenvia
        }


    }


    int playersEnded=0,hostEnded=0;
    int jog[2];
    int action;
    do{

        if(bastao==1){
            if(jogadorTemCarta(playerDeck) && jogadorPodeJogar(playerDeck, info.numCards,info.maxValue)){

                printf("\n");
                printf("%s-------------Sua mao:-------------\n",GRN);
                for(int i=0;i<13;i++)
                    if(playerDeck[i]>0)
                        printf("%s%d%s cartas de valor %s%d%s\n",RESET,playerDeck[i],GRN,RESET,i+1,GRN);

                printf("%s\n",RESET);
                if(info.numCards > 0){
                    printf("%sQuantidade de Cartas:%s %d\n",MAG,RESET, info.numCards);
                }
                printf("%sValor Maximo: %s%d\n",MAG,RESET, info.maxValue);

                printf("\n");

                if(info.maxValue==13)
                    action=1;
                else{
                    do{
                        printf("1- Jogar        2- Pular a vez:\n");
                        scanf("%d",&action);
                    }while(action!=1&&action!=2);
                }
                if(action==1){

                    do{
                        if(info.numCards==0){
                            printf("%sDigite o numero de cartas: %s",MAG,RESET);
                            scanf("%d",&jog[0]);    
                        }else{
                            jog[0]=info.numCards;

                        }
                        printf("%sDigite o valor das cartas jogadas: %s",MAG,RESET);
                        scanf("%d",&jog[1]);
                
                    }while(!jogadaEhValida(playerDeck, jog[0], jog[1], info.numCards, info.maxValue));
                    

                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,JOGADA ,jog, cr);
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    do{
                    
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,JOGADA,info.numPlayers)<0);

          

                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,BASTAO ,jog, cr);
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                }
            }
            if(!jogadorTemCarta(playerDeck)||info.numPlayers-playersEnded==1){
                if(!hostEnded){
                    jog[0]=hostId;
                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,EMPTYHAND ,jog, cr);
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    do{
                    
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,EMPTYHAND,info.numPlayers)<0);
                    fprintf(arq,"%s\n",info.idIps[(int)hostId]);
                    playersEnded++;
                    hostEnded=1;
                    
                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,BASTAO ,jog,cr );
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    bastao=0;

                }else{
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    bastao=0;
                }

            }else{
                if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                    printf("erro no envio\n");
                    return -1;
                }
                bastao=0;
            }


        }
        if(playersEnded<info.numPlayers){
            size=recvfrom(s, buffer, sizeof(buffer), 0,(struct sockaddr*)&raddr, &len);
            if(size<0)
                perror("erro no recebimento:");
        }

        if(verifyMsg(buffer,size)){
            separateMessage(&m2, buffer,info.numPlayers);
            switch(m2.tipo){

                case DISTRIBCARDS:
                    if(m2.cond_rec[(int)hostId]==0){
                        

                        for(int i=0;i<13;i++)
                            playerDeck[i]=m2.jogada[i];

                        printf("\n");
                        printf("%s-------------Sua mao:-------------\n",GRN);
                        for(int i=0;i<13;i++)
                            if(playerDeck[i]>0)
                                printf("%s%d%s cartas de valor %s%d%s\n",RESET,playerDeck[i],GRN,RESET,i+1,GRN);

                        printf("%s\n",RESET);


                        m2.cond_rec[(int)hostId]=1;
                        size=fillBuffer(&m2,  buffer,info.numPlayers);
                    }
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    
                break;

                case BASTAO:
                    bastao=1;
                    if(m2.origem==hostId){
                        info.numCards=0;
                        info.maxValue=13;
                        printf("Você ganhou a rodada!\n");
                    }
                break;

                case JOGADA:
                    if(m2.cond_rec[(int)hostId]==0){
                        
                        info.numCards=m2.jogada[0];
                        info.maxValue=m2.jogada[1];
                        printf("Jogador %d jogou %d cartas de valor %d.\n",m2.origem+1,m2.jogada[0],m2.jogada[1]);
                        m2.cond_rec[(int)hostId]=1;
                        size=fillBuffer(&m2,  buffer,info.numPlayers);
                    }
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    

                break;

                case EMPTYHAND:
                    if(m2.cond_rec[(int)hostId]==0){
                        fprintf(arq,"%s\n",info.idIps[(int)m2.jogada[0]]);
                        printf("Jogador %d jogou todas as suas cartas.\n",m2.origem);
                        playersEnded++;
                        m2.cond_rec[(int)hostId]=1;
                        size=fillBuffer(&m2,  buffer,info.numPlayers);
                    }
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    
                break;                   

            }               

        }
    }while(playersEnded<info.numPlayers); 

    
    close(s);
    fclose(arq);
    return 0;
}
