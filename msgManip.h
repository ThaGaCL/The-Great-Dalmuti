void setMsgAttr(mensagem_t* m,char id,int tipo,int *jogada , char* cond_rec);

int fillBuffer(mensagem_t* m, char* buffer,int numPlayers);

int getJogadaSizeByTipo(char tipo);

void separateMessage(mensagem_t* m, char* buffer,int numPlayers);

int verifyReceb(mensagem_t* m, int numPlayers);

char* getCondRecDistrib(int id,int numPlayers);

char* getCondRecBase(int id,int numPlayers);

int recvMensagem(int s,char* buffer, int buffer_size,struct sockaddr_in* raddr,socklen_t len,char tipo,int numPlayers);
