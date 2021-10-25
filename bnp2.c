

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


	//longitud del mensaje
	tam = sizeof(mensaje) - sizeof(long);

	//inicializando la cola de mensajes
	id01 = msgget(LLAVE,0600|IPC_CREAT);
	
	//esperando a jugador1...
	printf("\nesperando a jugador 1...\n");
	msgrcv(id01,&recibir,tam,1,0); //recibe el mensaje de jugador1
	if(recibir.tipo_mensaje != 0){ //fallo confirmacion de jugador1
		printf("\nerror al iniciar, intente de nuevo....\n");
		msgctl(id01,IPC_RMID,0); //elimina la cola de mensajes
		return 0;
	}

	//indicar que player2 ha iniciado
	enviar.tipo = 2;
	enviar.m1_fil = 0;
	enviar.m1_col = 0;
	enviar.m2_fil = 0;
	enviar.m2_col = 0;
	enviar.tipo_mensaje = 0;

	//envia el mensaje al jugador1
	msgsnd(id01,&enviar,tam,0);
	
	//muestra los tableros
	mostrarMatrices();	

	//esperando al jugador 1
	printf("\nturno de jugador 1...\n");
	msgrcv(id01,&recibir,tam,0,0);
	
	if(matriz2[recibir.m2_fil][recibir.m2_col] == '0'){
		matriz2[recibir.m2_fil][recibir.m2_col]='x'; 
	}else{
		matriz2[recibir.m2_fil][recibir.m2_col]='O';
	}
	mostrarMatrices();
		
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
					matriz2[fil][col]='X'; 
				}
				bandera = 0;
				mostrarMatrices();
				
	//verificando si gano el jugador2
				//verificacion de filas
				for(int i=0; i<3;i++){
					if(matriz2[i][0]== 'X' && matriz2[i][1]== 'X' && matriz2[i][2]== 'X'){
					printf("\n¡¡ganador jugador 2!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 3;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
				}
				//verificacion de columnas
						for(int i=0; i<3;i++){
					if(matriz2[0][i]== 'X' && matriz2[1][i]== 'X' && matriz2[2][i]== 'X'){
					printf("\n¡¡ganador jugador 2!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 3;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
				}
				//diagonal
				 if(matriz2[0][0]== 'X' && matriz2[1][1]== 'X' && matriz2[2][2]== 'X'){
					printf("\n¡¡ganador jugador 2!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 3;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}else if(matriz2[2][0]== 'X' && matriz2[1][1]== 'X' && matriz2[0][2]== 'X'){
					printf("\n¡¡ganador jugador 1!!\n");
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 3;
					msgsnd(id01,&enviar,tam,0);
					return 0;
				}
		
				//turno de jugador 2
				enviar.tipo = 1;
				enviar.m1_fil = fil;
				enviar.m1_col = col;
				enviar.tipo_mensaje = 1;
				msgsnd(id01,&enviar,tam,0);
			
				//esperando al jugador 1
				printf("\nturno de jugador 1...\n");
				msgrcv(id01,&recibir,tam,0,0);
				printf("%c",recibir.tipo_mensaje);
				switch(recibir.tipo_mensaje){
					case 1:	
							matriz2[recibir.m2_fil][recibir.m2_col]='O'; 
						mostrarMatrices();
						break;
					case 2:
						matriz2[recibir.m2_fil][recibir.m2_col]='O'; 
						mostrarMatrices();
						printf("\n¡¡ganador jugador 1!!\n");
						msgctl(id01, IPC_RMID, 0);
						return 0;
						break;
				}
			
			}
		}
		else{
			printf("¡la casilla (%d,%d) no existe!\n",fil, col);
		}
	}
	
}

void mostrarMatrices(void){
	system("clear");
	printf("JUGADOR 2 - Tic Tac Toe\n");
	printf("---------------------------------------------\n");
	printf("tablero               \n");
	printf("    0   1   2       \n");
	for(i=0; i<3; i++){
		//Mi tablero
		printf("%d ", i);
		printf("| ");
		for(j=0; j<3; j++){
			if(matriz2[i][j]=='z'){
				printf("  | ");
			}
			else{
				printf("%c | ",matriz2[i][j]);
			}
		}
			printf("\n");
		
	}
}