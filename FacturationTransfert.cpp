#include <stdio.h>
#include <unistd.h>
#include "Fichier.ini"
int idMsg;
MESSAGE	M;
#include "Ecran.h"
char Buff[255];
int rc;

int main(int argc,char*	argv[]) {
	Trace("Debut");
	if (argc != 2) {
		perror("trop ou trop peu d'arguments\n");
		exit(1);
	}
	int lectPipe = atoi(argv[1]);
	if ((idMsg = msgget(KEY,0)) == -1) {
		perror("Err. de msgget()");
		exit(1);
	}
	while(1) {
		//	Lecture dans le pipe
		if ((rc = read(lectPipe, Buff, sizeof(Buff))) < 0) {
			perror("erreur de lecture\n");
			exit(1);
		}
		Buff[rc] = '\0';
		
		
		printf("debut du sleep 10\n");
		sleep(10);
		// signaler que la commande est livree
		M.Type = 1L;
		M.idPid = getpid();
		M.Requete = COMMANDELIVREE;
		strcpy(M.arg.Selection.S1, Buff);
		if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
			perror("\033[31merreur de send\033[0m");
			exit(1);
		}
	}
	exit(0);
}














