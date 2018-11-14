#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSZ 100

struct mensaje {
	long msg_type;
	char msg_text[BUFSZ];
}pmsg;

int main(int argc, char *argv[]) {
	int qid;
	int len;

	if(argc != 2) {
		puts("uso: readq <id cola>");
		exit(EXIT_FAILURE);
	}

	qid = atoi(argv[1]);

	len = msgrcv(qid, &pmsg, BUFSZ, 0, 0);

	if(len > 0) {
		(&pmsg)-> msg_text[len] = '\0';
		printf("Leyendo id de la cola: %05d\n", qid);
		printf("Tipo de mensaje: %05ld\n", (&pmsg)->msg_type);
		printf("Tamaño del mensaje: %d bytes \n", len);
		printf("Texto del mensaje: %s \n", (&pmsg)->msg_text);
	}
	else {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
	