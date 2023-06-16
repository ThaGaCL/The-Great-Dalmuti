#include "gameCtrl.h"
#include "dgram.h"
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
    info_t info;

    getInitInfo(&hostId,&destId,&info,&door);//cria o vetor com os ips e retorna os ids

    int s = create_socket(info.idIps[(int)hostId],door);
    struct sockaddr_in saddr = init_sockaddrin(&info, destId, door),raddr;
    
    socklen_t len = (socklen_t )sizeof(saddr);
    memset(buffer, '\0', sizeof(buffer));
    //---------------------abertura do arquivo com o resultado------------------------
    FILE *arq = openFile("dados2.txt","w");
    fprintf(arq,"%d\n",info.numPlayers);//imprime o num de players
    fprintf(arq,"%d\n",door);//imprime a porta

    if(hostId==0){
        bastao=1;
        //------------gerar cartas------------------------
        //player 0 pega suas cartas
        for(int i=1;i<info.numPlayers;i++){
            setMsgAttr(&m,hostId,DISTRIBCARDS,/*vetor int com cartas para ser entregue*/, getCondRecDistrib(i,info.numPlayers));
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
    do{

        if(bastao==1){
            if(/* tem carta e e possivel fazer uma jogada*/){
                //---------------------fazer a jogada de acordo com o permitido
                //SE VALOR MAXIMO DE CARTAS FOR O MAXIMO, IMPEDIR DE PULAR A VEZ
                if(/*jogada realizada*/){
                    jog[0]=/*numero de cartas jogadas*/;
                    jog[1]=/*valor das cartas jogadas*/
                
                    setMsgAttr(&m,hostId,JOGADA ,jog, getCondRecBase(hostId,info.numPlayers));
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    do{
                    
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,JOGADA,info.numPlayers));
                    setMsgAttr(&m,hostId,BASTAO ,jog, getCondRecBase(hostId,info.numPlayers));
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                }
            }
            if(/*mao esta vazia*/){
                if(!hostEnded){
                    jog[0]=hostId;
                    setMsgAttr(&m,hostId,EMPTYHAND ,jog, getCondRecBase(hostId,info.numPlayers));
                    size=fillBuffer(&m,  buffer,info.numPlayers);
                    do{
                    
                        if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                            printf("erro no envio\n");
                            return -1;
                        }

                    }while(recvMensagem(s,buffer, sizeof(buffer),&raddr,len,EMPTYHAND,info.numPlayers));
                    playersEnded++;
                    hostEnded=1;

                    setMsgAttr(&m,hostId,BASTAO ,jog, getCondRecBase(hostId,info.numPlayers));
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
                    if(m2.cond_rec[hostId]==0){
                        //getcards from m2.jogada
                        m2.cond_rec[hostId]==1;
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
                    if(m2.cond_rec[hostId]==0){
                        //salvar numero de cartas e valor max from m2.jogada
                        printf("Jogador %d jogou %d cartas de valor %d.\n",m2.origem,m2.jogada[0],m2.jogada[1]);
                        m2.cond_rec[hostId]==1;
                        size=fillBuffer(&m2,  buffer,info.numPlayers);
                    }
                    if (sendto(s, buffer, size, 0,(struct sockaddr*)&saddr, len) < 0){
                        printf("erro no envio\n");
                        return -1;
                    }
                    

                break;

                case EMPTYHAND:
                    if(m2.cond_rec[hostId]==0){
                        fprintf(arq,"%s\n",info.idIps[m2.jogada[0]]);
                        printf("Jogador %d jogou todas as suas cartas.\n",m2.origem);
                        playersEnded++;
                        m2.cond_rec[hostId]==1;
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

    destroy_gameCtrl(gameCtrl);
    close(s);
    fclose(arq);
    return 0;
}
