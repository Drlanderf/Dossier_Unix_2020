#include <unistd.h>
#include "windowsuivicommande.h"
#include "ui_windowsuivicommande.h"
#include "Fichier.ini"
#include "Ecran.h"

extern int idMsg;
extern MESSAGE	M;
extern char Commande[20];

WindowSuiviCommande::WindowSuiviCommande(QWidget *parent) : QMainWindow(parent), ui(new Ui::WindowSuiviCommande) {
    ui->setupUi(this);
    ui->label->setText(Commande);
// demande d'un suivi
	int i;
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = SUIVICOMMANDE;
	strcpy(M.arg.Selection.S1, Commande);
	
	if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
		perror("\x1b[31mErreur d'envoi dans suiviCommande\x1b[0m");
		exit(1);
	}

// recuperation d'un messages
	if (msgrcv(idMsg, &M, TAILLECOMPLET, getpid(), 0) == -1) {
		perror("erreur de msgrcv dans suivi");
		exit(1);
	}
	if (M.Requete == 0) {
		Trace("\033[31mLe nom de la commande entree n'existe pas !\033[0m");
		exit(1);
	}
	i = 0;
	while (strlen(M.arg.C.E[i])) {
		printf("reception SUIVICOMMANDE -%s-   %d\n",M.arg.C.E[i],M.arg.C.T[i]);
		setE1(i,M.arg.C.E[i]);
		if (!M.arg.C.T[i]) setCheck(i,true);
			i++;
	}
	if (M.Requete == COMMANDELIVREE)
		setCheck(5,true);
	
	return;	
}

WindowSuiviCommande::~WindowSuiviCommande() {
    delete ui;
}
#include "fctUtileSuivCommande.cpp"

void WindowSuiviCommande::on_ButtonTerminer_clicked() {
    M.Type = 1L;
	M.idPid = getpid();
	M.Requete = FINSUIVICOMMANDE;
	if (msgsnd(idMsg,&M,TAILLEBASE,0) == -1) { 
		perror("\x1b[31mErreur d'envoi de fin de suiviCommande\x1b[0m");
		exit(1);
	}
	exit(0);
}












