
#define MAXPLAYERS 8
#define MSGSIZE 32
#define LINESIZE 16
#define IPSIZE 16
#define MARCINI 126
#define MARCFIM 94
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

typedef enum
{ 
    DISTRIBCARDS= 1,
    BASTAO,
    JOGADA,             //primeiro caractere numero de cartas e segundo valor da carta
    EMPTYHAND 
    
} type_t ;


typedef struct mensagem_t{
    char origem;
    char tipo;
    char jogada[LINESIZE];
    char cond_rec[MAXPLAYERS];

}mensagem_t;

typedef struct info_t{
    int numPlayers;
    char idIps[MAXPLAYERS][IPSIZE];
    char maxValue;
    char numCards;
    

}info_t;

int create_socket(char* ip,int door);

FILE* openFile(char* path,char* action);

int getHostIp(char** host_ip);

void replaceFirst(char* s,char c1, char c2);

int getInitInfo(char *hostId,char *destId,info_t* info,int* door);

struct sockaddr_in init_sockaddrin(info_t* info,char destId,int door);
