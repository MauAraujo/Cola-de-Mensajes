#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int colaClientesid, pid, i, status, len, salir;
double promedio, promedio_total;

struct msqid_ds qStatus;

struct msgbuf {
	long msg_type;
	int tiempo;
}colaClientes;

struct cajeros{
	int tiempo; //El tiempo que tarda con cada cliente
	int num_clientes; //Numero de clientes atendidos
	int tiempo_total;  //Toatl de tiempo invertido
}cajero;

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
		colaClientes.msg_type = i+1;
		colaClientes.tiempo = time;
		msgsnd(colaClientesid, &colaClientes, sizeof(colaClientes.tiempo), 0);
	}
}

void transaccion(int pid) {
  len = msgrcv(colaClientesid, &colaClientes, sizeof(colaClientes.tiempo), 0, 0);
  printf("Tiempo de cliente en caja %d: %d\n", pid, colaClientes.tiempo);
  cajero.tiempo = colaClientes.tiempo;
  cajero.num_clientes++;
  cajero.tiempo_total += cajero.tiempo;
  sleep(cajero.tiempo);
	// sleep(1);
  cajero.tiempo = 0;
  salir++;
}

int eliminarCola() {
	if(msgctl (colaClientesid, IPC_RMID, NULL) < 0) {
		perror("msgctl");
		exit(EXIT_FAILURE);
	}
	printf("Cola de clientes con id %d eliminada\n", colaClientesid);
	exit(EXIT_SUCCESS);
}

void prom(){
	promedio = (double)cajero.tiempo_total/(double)cajero.num_clientes;
	printf("Tiempo promedio caja %d: %f\n", pid, promedio);
}

int main() {
	system("clear");
  int estado;
	key_t key;
	key=1909;

	srand(time(NULL));
	colaClientesid = msgget(key, IPC_CREAT | 0666);
	printf("Cola de mensajes creada con id: %d\n", colaClientesid);
	asignarTiempos();

	//Inicializa cajero
	cajero.num_clientes = 0;
	cajero.tiempo_total = 0;

	pid = nfork(6);

	switch(pid) {
		case 0:
			for(i=1;i<5;i++) wait(&status);

			eliminarCola();
			exit(1);

		case 1:
      do {
        transaccion(pid);
        estado = msgctl(colaClientesid, IPC_STAT, &qStatus);
				printf("Mensajes restantes: %d\n", qStatus.msg_qnum);
				if(estado != 0) {
					printf("%d\n", estado);
					break;
				}
      } while(qStatus.msg_qnum > 0);
      prom();
      exit(0);

		case 2:
      do {
        transaccion(pid);
        estado = msgctl(colaClientesid, IPC_STAT, &qStatus);
				printf("Mensajes restantes: %d\n", qStatus.msg_qnum);
				if(estado != 0) {
					break;
				}
      } while(qStatus.msg_qnum > 0);
      prom();
			exit(0);

		case 3:
      do {
        transaccion(pid);
        estado = msgctl(colaClientesid, IPC_STAT, &qStatus);
				printf("Mensajes restantes: %d\n", qStatus.msg_qnum);
				if(estado != 0) {
					break;
				}
      } while(qStatus.msg_qnum > 0);
      prom();
			exit(0);

  case 4:
    do {
      transaccion(pid);
      estado = msgctl(colaClientesid, IPC_STAT, &qStatus);
			printf("Mensajes restantes: %d\n", qStatus.msg_qnum);
			if(estado != 0) {
				printf("Error actualizando en proceso %d con estado %d\n", pid, estado);
				break;
			}
    } while(qStatus.msg_qnum > 0);
		prom();
    exit(0);
	}
}
