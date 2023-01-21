#include "windowFenetrePersonnel.h"
#include "ui_windowFenetrePersonnel.h"
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "Fichier.ini"
#include "Ecran.h"

extern int Ligne;
extern char	Personnel[20];
extern int idMsg;
extern MESSAGE	M;
extern windowFenetrePersonnel* w;

void HandlerSigusr1(int);
void HandlerAlarme(int);

struct sigaction s;

windowFenetrePersonnel::windowFenetrePersonnel(QWidget *parent) : QMainWindow(parent), ui(new Ui::windowFenetrePersonnel) {
	// armement des signaux
	s.sa_handler = HandlerSigusr1;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGUSR1, &s, NULL);
	
	s.sa_handler = HandlerAlarme;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGALRM, &s, NULL);



	ui->setupUi(this);
	this->move(Ligne,650);
	this->setWindowTitle(Personnel);

}

windowFenetrePersonnel::~windowFenetrePersonnel() {
	delete ui;
}

void windowFenetrePersonnel::on_BoutonTerminer_clicked() {
	Trace("Terminer\n");
	// signaler la fin de ce personnel
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = FINPERSONNEL;
	M.arg.N = 0;
	if (msgsnd(idMsg, &M, TAILLEBASE, 0) == -1) {
		perror("erreur de send\n");
		exit(1);
	}

	exit(0);
}

#include "fctUtileFenetre.cpp"
int i,Pos;
bool pass;
void HandlerSigusr1(int Sig) {
	Trace("Reception d'un signal SIGUSER1 (%d)",Sig);
	i = 0;
	pass=0;
	if (msgrcv(idMsg, &M, TAILLECOMPLET, getpid(), 0) == -1) {
		perror("erreur de reception\n");
		exit(1);
	}
	
	w->setNomC(M.arg.C.NomC);
	w->setE1(M.arg.C.E[0]);
	w->setE2(M.arg.C.E[1]);
	w->setE3(M.arg.C.E[2]);
	w->setE4(M.arg.C.E[3]);
	
	
	alarm(M.arg.C.T[0]);
	
	return;
}

void HandlerAlarme(int Sig) {
	Trace("Reception d'un SIGALRM (%d) %deme etape",Sig,i);
	w->setCheck(i,TRUE);
	if (!pass) {
		M.Type = 1L;
		M.idPid = getpid();
		M.Requete = ETAPEEFFECTUEE;
		M.arg.C.T[0] = i;
		if (msgsnd(idMsg, &M, TAILLECOMPLET, 0) == -1) {
			perror("erreur de send\n");
			exit(1);
		}
	
		i++;
	}
	if (pass) {			//lorsque c'est fini afin de supprimer les infos graphiques
		w->setNomC("***");
		w->setE1("");
		w->setE2("");
		w->setE3("");
		w->setE4("");
		for(i=0;i<4;i++)
			w->setCheck(i,FALSE);
		return;
	}
	if (i > 3 || M.arg.C.T[i] == -1) {
		pass = TRUE;
		M.Requete = TRAVAILTERMINER;
		if (msgsnd(idMsg, &M, TAILLECOMPLET, 0) == -1) {
			perror("erreur de send dans handlerAlarme\n");
			exit(1);
		}
		alarm(3);		//attente de 3 sec avant effacement des informations
		
	}
	else
		alarm(M.arg.C.T[i]);

}






