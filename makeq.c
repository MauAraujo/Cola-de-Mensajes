#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

struct msg {
	long msg_type;
	char msg_text[100];
}mensaje;

int main(int argc, char *argv[]) {
	int qid;
	key_t key;
	key=1909;

	if((qid = msgget(key, IPC_CREAT | 0666)) < 0) {
		perror("msgget create");
		exit(EXIT_FAILURE);
	}
	printf("Cola de mensajes creada con id: %d\n", qid);
	exit(EXIT_SUCCESS);
}