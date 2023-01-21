#include "windowFenetrePersonnel.h"
#include <QApplication>
#include "Fichier.ini"

int idMsg;
MESSAGE	M;
int	Ligne;
char	Personnel[20];
windowFenetrePersonnel* w;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Trop ou trop peu d'argument(s)\n");
		exit(1);
	}
	Ligne = atoi(argv[2]);
	strcpy(Personnel,argv[1]);
	// Recuperation de la file de messages
	
	if ((idMsg = msgget(KEY,0)) == -1) {
		perror("erreur de msgget dans mainFenetrePersonnel.cpp\n");	
		exit(1);
	}

	// identification 
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = NEWPERSONNEL;
	strcpy(M.arg.Selection.S1, Personnel);
	
	
	if (msgsnd(idMsg,&M,TAILLESELECTION,0) == -1) {
		perror("erreur de msgsnd dans mainFenetrePersonnel.cpp\n");
		exit(1);
	}


	QApplication a(argc, argv);
	w = new windowFenetrePersonnel;
	w->show();

	return a.exec();
}
