#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "dgram.h"
#include "msgManip.h"

void setMsgAttr(mensagem_t* m,char id,int tipo,int *jogada , char* cond_rec){

    if(!m)
        return;

    m->origem = id;
    //printf("origem:%d\n",m->origem);
    m->tipo = tipo;
    //printf("tipo:%d\n",m->tipo);

    int aux=getJogadaSizeByTipo(tipo);

    for(int i=0;i<aux;i++){
        m->jogada[i]=jogada[i];
        //printf("jogada:%d ",m->jogada[i]);
    }
    //printf("\n");
    strcpy((char*)m->cond_rec, (char*)cond_rec);

}

int fillBuffer(mensagem_t* m,  char* buffer,int numPlayers){

    if(m==NULL||buffer==NULL)
        return 0;

    int size;

    buffer[0]=MARCINI;
    buffer[1]=m->origem;
    buffer[2]=m->tipo;

    size = getJogadaSizeByTipo(m->tipo);

    memcpy(&buffer[3], m->jogada, size);
    size+=3;
    memcpy(&buffer[size],m->cond_rec,numPlayers);
    size+=numPlayers;
    buffer[size]=MARCFIM;    
    
    /*for(int i=0;i<size;i++)
        printf("%d ",buffer[i]);
    printf("\n");*/
   
    return size+1;
}

int getJogadaSizeByTipo(char tipo){

    if(tipo==DISTRIBCARDS)
        return 13;
    else if(tipo==JOGADA)
        return 2;
    else if(tipo==EMPTYHAND)
        return 1;

    return 0;


}

int verifyReceb(mensagem_t* m, int numPlayers){

    for(int i=0;i<numPlayers;i++)
        if(m->cond_rec==0)
            return 0;

    return 1;

}

void getCondRecDistrib(int id,char *v){

    //char v[numPlayers];
    memset(v, 1, MAXPLAYERS);
    v[id]=0;

}

void getCondRecBase(int id,char* v){

    //char v[numPlayers];
    memset(v, 0, MAXPLAYERS);
    v[id]=1;

}

void separateMessage(mensagem_t* m,char* buffer,int numPlayers){

    if(m==NULL||buffer==NULL)
        return;

    int aux;

    m->origem=buffer[1];
    m->tipo=buffer[2];

    aux=getJogadaSizeByTipo(m->tipo);
    memcpy(m->jogada,&buffer[3],aux);
    memcpy(m->cond_rec,&buffer[3+aux],numPlayers);    

}

int verifyMsg(char* buffer,int size){

    if(!buffer)
        return 0;
    if(size>4)
        if(buffer[0]==MARCINI&&buffer[size-1]==MARCFIM)
            return 1;

    return 0;

}

int protocoloValido(char* buffer, int buffer_size,char tipo,int numPlayers) {
    if (buffer_size <= 0) 
        return 0; 
    mensagem_t m;   
    if(verifyMsg(buffer,buffer_size)) {
        separateMessage(&m, buffer,numPlayers);
        if(m.tipo==tipo&&verifyReceb(&m,numPlayers))
            return 1;
        
    }

    return 0;
}

int recvMensagem(int s,char* buffer, int buffer_size,struct sockaddr_in* raddr,socklen_t len,char tipo,int numPlayers) {

    int bytes_lidos = recvfrom(s, buffer,buffer_size, 0,(struct sockaddr*)raddr, &len);
    if (bytes_lidos < 0){
        printf("erro no recebimento\n");
        return -1;
    }


    if(protocoloValido(buffer, bytes_lidos,tipo,numPlayers))
        return bytes_lidos; 
    return -1;
}
