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

    int s = create_socket(info.idIps[(int)hostId],door);
    struct sockaddr_in saddr = init_sockaddrin(&info, destId, door),raddr;

    // Cria o vetor de ints que representa o deck e o preenche
    int *deck = (int *)malloc(sizeof(int)*info.numPlayers*DECK_SIZE);
    int *playerDeck = (int *)malloc(sizeof(int)*DECK_SIZE);

    fillDeck(deck);
    
    socklen_t len = (socklen_t )sizeof(saddr);
    memset(buffer, '\0', sizeof(buffer));
    //---------------------abertura do arquivo com o resultado------------------------
    FILE *arq = openFile("dados2.txt","w");
    fprintf(arq,"%d\n",info.numPlayers);//imprime o num de players
    fprintf(arq,"%d\n",door);//imprime a porta

    if(hostId==0){
        bastao=1;
        //------------gerar cartas------------------------
        // Distribui as cartas do jogador 0
        distribuiCartas(deck, playerDeck, info.numPlayers);

        // Envia as cartas para o jogador 0
        for(int i=1;i<info.numPlayers;i++){
            getCondRecDistrib(i,cr);
            setMsgAttr(&m,hostId,DISTRIBCARDS, playerDeck,cr );
            size=fillBuffer(&m,  buffer,info.numPlayers);
            do{
                if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                    printf("erro no envio\n");
                    return -1;
                }

            }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,DISTRIBCARDS,info.numPlayers));//se hoube erro, reenvia

        }

    }

    int playersEnded=0,hostEnded=0;
    int jog[2];
    int action;
    do{

        if(bastao==1){
            /* tem carta e e possivel fazer uma jogada*/
            if(jogadorTemCarta(playerDeck) && jogadorPodeJogar(playerDeck, m2.jogada[1], jog[0])){
                //---------------------fazer a jogada de acordo com o permitido
                printf("1- Jogar\n2- Pular a vez\n");
                scanf("%d",&action);
                //SE VALOR MAXIMO DE CARTAS FOR O MAXIMO, IMPEDIR DE PULAR A VEZ
                if(jogadaEhValida(playerDeck, jog[1], jog[0], m2.jogada[1], m2.jogada[0])){
                    printf("Digite o numero de cartas e o valor das cartas jogadas: ");
                    scanf("%d %d",&jog[0],&jog[1]);

                    // jog[0]=/*numero de cartas jogadas*/
                    // jog[1]=/*valor das cartas jogadas*/
                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,JOGADA ,jog, cr);
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    do{
                    
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,JOGADA,info.numPlayers));
                    getCondRecBase(hostId,cr);
                    setMsgAttr(&m,hostId,BASTAO ,jog, cr);
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                }
            }
            if(!jogadorTemCarta(playerDeck)){
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

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,EMPTYHAND,info.numPlayers));
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
        size=recvfrom(s, buffer, sizeof(buffer), 0,(struct sockaddr*)&raddr, &len);
        if(size<0)
            perror("erro no recebimento:");

        if(verifyMsg(buffer,size)){
            separateMessage(&m2, buffer,info.numPlayers);
            switch(m2.tipo){

                case DISTRIBCARDS:
                    if(m2.cond_rec[(int)hostId]==0){
                        //getcards from m2.jogada
                        m2.cond_rec[(int)hostId]=1;
                        size=fillBuffer(&m2,  buffer,info.numPlayers);
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }
                    }
                break;

                case BASTAO:
                    bastao=1;
                    if(m2.origem==hostId){
                        //resetar numero de cartas e valor max
                        printf("Você ganhou a rodada");
                    }
                break;

                case JOGADA:
                    if(m2.cond_rec[(int)hostId]==0){
                        //salvar numero de cartas e valor max from m2.jogada
                        printf("Jogador %d jogou %d cartas de valor %d.\n",m2.origem,m2.jogada[0],m2.jogada[1]);
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

    

    //gameCtrl_t gameCtrl = new_gameCtrl(info.numPlayers);
    // print_gameCtrl(gameCtrl);
    //distribute_cards(gameCtrl);
    //print_jogadores(gameCtrl);
    // destroy_gameCtrl(gameCtrl);
    close(s);
    fclose(arq);
    return 0;
}
