

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define LLAVE 34856

typedef struct mensaje{
	long tipo;
	int m1_fil;
	int m1_col;
	int m2_fil;
	int m2_col;
	
	int tipo_mensaje;
} mensaje;

char matriz1[4][4];
char matriz2[4][4];
int i, j;
int fil, col;

int cont = 0;
int bandera = 0;

void mostrarMatrices(void);

int main(){
	//variables necesarias
	int id01;
	int tam;

	//estructuras de mensajes
	mensaje enviar;
	mensaje recibir;

	//inicializando matrices
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			matriz1[i][j]='z';
			matriz2[i][j]='z';
		}
	}

	//muestra los tableros vacios
	mostrarMatrices();
	

	//indicar que jugador1 ha iniciado
	enviar.tipo = 1;
	enviar.m1_fil = 0;
	enviar.m1_col = 0;
	enviar.m2_fil = 0;
	enviar.m2_col = 0;
	enviar.tipo_mensaje = 0;

	//longitud del mensaje
	tam = sizeof(mensaje) - sizeof(long);

	//inicializando la cola de mensajes
	id01 = msgget(LLAVE,0600|IPC_CREAT);

	//envia el mensaje al jugador2
	msgsnd(id01,&enviar,tam,0);

	//esperando a jugador2...
	printf("\nesperando a jugador 2...\n");
	msgrcv(id01,&recibir,tam,2,0); //recibe el mensaje de jugador2
	if(recibir.tipo_mensaje != 0){ //fallo confirmacion de jugador2
		printf("\nerror al iniciar, intente de nuevo....\n");
		msgctl(id01,IPC_RMID,0); //elimina la cola de mensajes
		return 0;
	}
	
	//muestra los tableros
	mostrarMatrices();

	//inicio del juego
	while(1){
		printf("\nIngrese las coordenadas \n");
		printf("fila: ");
		scanf("%d",&fil);		
		printf("columna: ");
		scanf("%d",&col);	
		if(fil >= 0 && fil <=3 && col >= 0 && col <=3){
			if(matriz1[fil][col] == 'X' || matriz1[fil][col] == 'O'){
				printf("casilla ocupada!!!\n");
			}
			else{	
				
				if(bandera == 0){
					matriz1[fil][col]='O'; //ataque fallido
				}
				bandera = 0;
				mostrarMatrices();
			
				//verificando si gano el jugador1
				//verificacion de filas
				for(int i=0; i<3;i++){
					if(matriz1[i][0]== 'O' && matriz1[i][1]== 'O' && matriz1[i][2]== 'O'){
					printf("\n¡¡ganador jugador 1!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 2;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
				}
				//verificacion de columnas
						for(int i=0; i<3;i++){
					if(matriz1[0][i]== 'O' && matriz1[1][i]== 'O' && matriz1[2][i]== 'O'){
					printf("\n¡¡ganador jugador 1!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 2;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
				}
				//diagonal
				 if(matriz1[0][0]== 'O' && matriz1[1][1]== 'O' && matriz1[2][2]== 'O'){
					printf("\n¡¡ganador jugador 1!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 2;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}else if(matriz1[2][0]== 'O' && matriz1[1][1]== 'O' && matriz1[0][2]== 'O'){
					printf("\n¡¡ganador jugador 1!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 2;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
		

				//turno de jugador 1
				enviar.tipo = 1;
				enviar.m2_fil = fil;
				enviar.m2_col = col;
				enviar.tipo_mensaje = 1;
					
				msgsnd(id01,&enviar,tam,0);
		
				//esperando al jugador 2
				printf("\nesperando a jugador 2...\n");
				msgrcv(id01,&recibir,tam,0,0);

				
					matriz1[recibir.m1_fil][recibir.m1_col]='X'; //ataque fallido
				
				mostrarMatrices();

				//verificando si gano jugador 2
				if(recibir.tipo_mensaje == 3){
					printf("\n¡¡ganador jugador 2!!\n");
					msgctl(id01, IPC_RMID, 0);
					return 0;
				}
			} //fin del else
		}else{
			printf("¡la casilla (%d,%d) no existe!\n",fil, col);
		}
	} //fin del while
}

void mostrarMatrices(void){
	system("clear");
	printf("JUGADOR 1 - Tic Tac Toe\n");
	printf("---------------------------------------------\n");
	printf("Mi tablero                \n");
	printf("    0   1   2   \n");
	for(i=0; i<3; i++){
		//Mi tablero
		printf("%d ", i);
		printf("| ");
		for(j=0; j<3; j++){
			if(matriz1[i][j]=='z'){
				printf("  | ");
			}
			else{
				printf("%c | ",matriz1[i][j]);
			}
		}
	
		printf("\n");
	}
}