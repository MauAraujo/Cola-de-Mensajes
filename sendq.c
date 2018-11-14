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
		puts("uso: sendq <id cola>");
		exit(EXIT_FAILURE);
	}

	qid = atoi(argv[1]);
	puts("Ingresa el pmsg a enviar\n");

	if(fgets((&pmsg) -> msg_text, 100, stdin) == NULL) {
		puts("No hay pmsg para enviar");
		exit(EXIT_SUCCESS);
	} 

	pmsg.msg_type = getpid();
	len = strlen(pmsg.msg_text);

	if(msgsnd(qid, &pmsg, len, 0) < 0) {
		perror("msgsend");
		exit(EXIT_FAILURE);
	}
	puts("pmsg enviado)");
	exit(EXIT_SUCCESS);
}
	