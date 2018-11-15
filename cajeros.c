#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int colaClientesid, pid, i, status;

struct clientes {
	long msg_type;
	int tiempo[20];
}colaClientes;

typedef struct{
	int tiempo; //El tiempo que tarda con cada cliente
	int num_clientes; //Numero de clientes atendidos
	int tiempo_total;  //Toatl de tiempo invertido
}CAJERO;

int nfork(int np) {
	int i;
	for (i = 1; i <= np; i++)
	{
		if(fork() == 0) {
			return(i);
		}
	}
	return(0);
}

void asignarTiempos() {
	int time;
	for(i = 0; i < 20; i++) {
		time = rand()%(10-1 + 1) + 1;
		printf("Time = %d\n", time);
		colaClientes.msg_type = time;
		colaClientes.tiempo[i] = time;
		msgsnd(colaClientesid, &colaClientes, sizeof(colaClientes), 0);
	}

}

int eliminarCola() {
	if(msgctl (colaClientesid, IPC_RMID, NULL) < 0) {
		perror("msgctl");
		exit(EXIT_FAILURE);
	}
	printf("Cola de clientes con id %d eliminada\n", colaClientesid);
	exit(EXIT_SUCCESS);
}

int main() {

	key_t key;
	key=1909;

	srand(time(NULL));
	colaClientesid = msgget(key, IPC_CREAT | 0666);
	printf("Cola de mensajes creada con id: %d\n", colaClientesid);
	asignarTiempos();

	pid = nfork(6);

	switch(pid) {
		case 0:
			printf("Proceso %d\n", pid);
			for(i=1;i<5;i++) wait(&status);
			exit(1);

		case 1:
			printf("Proceso %d\n", pid);
			// for(i = 0; i < 50; i++) {
				msgrcv(colaClientesid, &colaClientes, sizeof(colaClientes), 0, 0);
				printf("Tiempo de cliente 1: %d\n", *colaClientes.tiempo);
			// }
			exit(0);

		case 2:
			printf("Proceso %d\n", pid);
			msgrcv(colaClientesid, &colaClientes, sizeof(colaClientes), 0, 0);
			printf("Tiempo de cliente 2: %d\n", *colaClientes.tiempo);
			exit(0);

		case 3:
			printf("Proceso %d\n", pid);
			msgrcv(colaClientesid, &colaClientes, sizeof(colaClientes), 0, 0);
			printf("Tiempo de cliente 3: %d\n", *colaClientes.tiempo);
			exit(0);

		case 4:
			printf("Proceso %d\n", pid);
			msgrcv(colaClientesid, &colaClientes, sizeof(colaClientes), 0, 0);
			printf("Tiempo de cliente 4: %d\n", *colaClientes.tiempo);
			exit(0);
	}
}
