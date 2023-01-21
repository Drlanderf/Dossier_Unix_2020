#include "windowadministrateur.h"
#include "ui_windowadministrateur.h"
#include <QMessageBox>
#include "Fichier.ini"
#include "Ecran.h"
#include <signal.h>
#include <string.h>

extern int idMsg;
extern MESSAGE	M;
extern WindowAdministrateur *w;

void HNouvelUtilisateur(int);
struct sigaction s;
WindowAdministrateur::WindowAdministrateur(QWidget *parent) : QMainWindow(parent), ui(new Ui::WindowAdministrateur) {
	ui->setupUi(this);
	move(10,175);

	// Armement des signaux
	s.sa_handler = HNouvelUtilisateur;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGUSR1, &s, NULL);


	int i = 0;
	while (i < 4) { 
		lineCommande[i] = new QLineEdit(this);
		lineCommande[i]->setGeometry(QRect(20,50 + i * 30,120,20));
		lineCommande[i]->setReadOnly(true);
		lineDisponible[i] = new QLineEdit(this);
		lineDisponible[i]->setGeometry(QRect(200,50 + i * 30,120,20));
		lineDisponible[i]->setReadOnly(true);
		i++;
	}

	i = 0;
	while (i < 4) {
		linePersonnel[i] = new QLineEdit(this);
		linePersonnel[i]->setGeometry(QRect(450,50 + i * 30,120,20));
		linePersonnel[i]->setReadOnly(true);      
		i++;
	}

	lineCommandeN[0] = new QLineEdit(this);
	lineCommandeN[0]->setGeometry(QRect(40,345 ,120,20));
	lineCommandeT[0] = new QLineEdit(this);
	lineCommandeT[0]->setGeometry(QRect(170,345 ,40,20));
	lineCommandeN[1] = new QLineEdit(this);
	lineCommandeN[1]->setGeometry(QRect(280,345 ,120,20));
	lineCommandeT[1] = new QLineEdit(this);
	lineCommandeT[1]->setGeometry(QRect(410,345 ,40,20));
	lineCommandeN[2] = new QLineEdit(this);
	lineCommandeN[2]->setGeometry(QRect(40,375,120,20));
	lineCommandeT[2] = new QLineEdit(this);
	lineCommandeT[2]->setGeometry(QRect(170,375 ,40,20));
	lineCommandeN[3] = new QLineEdit(this);
	lineCommandeN[3]->setGeometry(QRect(280,375 ,120,20));
	lineCommandeT[3] = new QLineEdit(this);
	lineCommandeT[3]->setGeometry(QRect(410,375,40,20));

//	setCommande(0,"Commande1");
//	setCommande(1,"Commande2");
//	setCommande(2,"Commande3");
//	setCommande(3,"Commande4");
//	setDisponible(2,"MerceD");
//	setDisponible(3,"Defooz");

}

#include "FctUtilesAdministrateur.cpp"

WindowAdministrateur::~WindowAdministrateur() {
    delete ui;
}

void WindowAdministrateur::on_ButtonAccepteSelection_clicked() {
	Trace("Dans on_ButtonAccepteSelection_clicked");
	if (!getSelectionCommande()) return;
	if (!getSelectionPersonnel()) return;
	Trace("Lu --%s--%s-- (%d)\n",getSelectionCommande(), getSelectionPersonnel(),idMsg);

	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = TRANSMISSIONCOMMANDE;
	strcpy(M.arg.Selection.S1, getSelectionCommande());
	strcpy(M.arg.Selection.S2, getSelectionPersonnel());
	
	if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
		perror("message send\n");
		exit(1);
	}


	setSelectionCommande("");
	setSelectionPersonnel("");

}

void WindowAdministrateur::on_ButtonAnnulerSelection_clicked() {
	Trace("Dans on_ButtonAnnulerSelection_clicked");
	const char* buff;
	int i = 0;
	if ((buff=getSelectionCommande())) {
		setSelectionCommande("");
		while(getCommande(i)) { i++; }
		setCommande(i, buff);	
	}
	
	if ((buff = getSelectionPersonnel())) {
		setSelectionPersonnel("");
		i=0;
		while(getDisponible(i)) { i++; }
		setDisponible(i, buff);
	}			


}


void WindowAdministrateur::on_ButtonAccepterCommande_clicked() {
	Trace("Dans on_ButtonAccepterCommande_clicked");
	int i=0;
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = AJOUTCOMMANDE;
	
	if (!getNomCommande()) {
		MessageInformation("Avertissement","Il vous faut un nom de commande !");
		return;
	}
	
	strcpy(M.arg.C.NomC, getNomCommande());
	for (i=0;i<4;i++) {
		if (getCommandeN(i))
			strcpy(M.arg.C.E[i], getCommandeN(i));
		else
			strcpy(M.arg.C.E[i], "");
		M.arg.C.T[i] = getCommandeT(i);
	}

	
	if (msgsnd(idMsg, &M, TAILLECOMPLET, 0) == -1) {
		perror("erreur d'envoi\n");
		exit(1);
	}
	
	setNomCommande("");

	

	
}

void WindowAdministrateur::on_ButtonAnnulerCommande_clicked() {
	printf("Dans on_ButtonAnnulerCommande_clicked\n");
}

void WindowAdministrateur::on_ButtonTerminer_clicked() {
	printf("Dans on_ButtonTerminer_clicked\n");
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = FINADMINISTRATEUR;
	if (msgsnd(idMsg, &M, TAILLEBASE, 0) == -1) {
		perror("erreur d'envoi\n");
		exit(1);
	}
	exit(0);
}

void WindowAdministrateur::on_ButtonCommande1_clicked() {
	Trace("Dans on_ButtonCommande1_clicked");
	if (getSelectionCommande()) {
		MessageInformation("Avertissement","Il y a déjà une commande");
		return;
	}
	setSelectionCommande(getCommande(0));
	setCommande(0,"");
}

void WindowAdministrateur::on_ButtonCommande2_clicked() {
	Trace("Dans on_ButtonCommande2_clicked");
	if (getSelectionCommande()) {
		MessageInformation("Avertissement","Il y a déjà une commande");
		return;
	}
	setSelectionCommande(getCommande(1));
	setCommande(1,"");
}

void WindowAdministrateur::on_ButtonCommande3_clicked() {
	Trace("Dans on_ButtonCommande3_clicked");
	if (getSelectionCommande()) {
		MessageInformation("Avertissement","Il y a déjà une commande");
		return;
	}

	setSelectionCommande(getCommande(2));
	setCommande(2,"");
}

void WindowAdministrateur::on_ButtonCommande4_clicked() {
	Trace("Dans on_ButtonCommande4_clicked");
	if (getSelectionCommande()) {
		MessageInformation("Avertissement","Il y a déjà une commande");
		return;
	}
	setSelectionCommande(getCommande(3));
	setCommande(3,"");
}

void WindowAdministrateur::on_ButtonPersonnel1_clicked() {
	Trace("Dans on_ButtonPersonnel1_clicked");
	if (getSelectionPersonnel()) {
		MessageInformation("Avertissement","Il y a  déjà un personnel.");
		return;
	}
	setSelectionPersonnel(getDisponible(0));
	setDisponible(0,"");
}

void WindowAdministrateur::on_ButtonPersonnel2_clicked() {
	Trace("Dans on_ButtonPersonnel2_clicked");
	if (getSelectionPersonnel()) {
		MessageInformation("Avertissement","Il y a  déjà un personnel.");
		return;
	}
	setSelectionPersonnel(getDisponible(1));
	setDisponible(1,"");
}

void WindowAdministrateur::on_ButtonPersonnel3_clicked() {
	Trace("Dans on_ButtonPersonnel3_clicked");
	if (getSelectionPersonnel()) {
		MessageInformation("Avertissement","Il y a  déjà un personnel.");
		return;
	}
	setSelectionPersonnel(getDisponible(2));
	setDisponible(2,"");
}

void WindowAdministrateur::on_ButtonPersonnel4_clicked() {
	Trace("Dans on_ButtonPersonnel4_clicked");
	if (getSelectionPersonnel()) {
		MessageInformation("Avertissement","Il y a  déjà un personnel.");
		return;
	}
	setSelectionPersonnel(getDisponible(3));
	setDisponible(3,"");
}


void HNouvelUtilisateur(int Sig) {
	Trace("Reception d'un signal (%d)",Sig);
	char	Buff[80];
	int i=0;
	while (1) {
		//   reception d'un message
		if (msgrcv(idMsg, &M, TAILLESELECTION, getpid(), IPC_NOWAIT) == -1) {
			if (errno == ENOMSG) return;
			perror("erreur de rcv dans windowadministrateur.cpp");
			exit(1);
		}
		switch(M.Requete) {
			case NEWPERSONNEL:
				Trace("reception NEWPERSONNEL --%s--\n",M.arg.Selection.S1);
				i=0;
				while(w->getPersonnel(i)) { i++; }
				w->setPersonnel(i, M.arg.Selection.S1);
				i=0;
				while(w->getDisponible(i)) { i++; }
				w->setDisponible(i, M.arg.Selection.S1);
				break;

			case TRAVAILTERMINER:
				Trace("reception TRAVAILTERMINER --%s--\n",M.arg.Selection.S1);
				sprintf(Buff,"%s a termine la commande %s ",M.arg.Selection.S1, M.arg.Selection.S2);
				QMessageBox::information(w,"Avertissement",Buff); 
				i=0;
				while(w->getDisponible(i)) { i++; }
				w->setDisponible(i, M.arg.Selection.S1);
				
				break;

			case NEWCOMMANDE:
				Trace("reception NEWCOMMANDE --%s--\n",M.arg.Selection.S1);
				i=0;
				while(w->getCommande(i)) { i++; }
				w->setCommande(i, M.arg.Selection.S1);
				break;

			case NUMEROCOMMANDE:
				Trace("Reception NUMEROCOMMANDE");
				sprintf(Buff,"Commande enregistrée sous le numéro %d",M.arg.N);
				QMessageBox::information(w,"Avertissement",Buff);   
				break;
			case FINPERSONNEL:
				Trace("Reception FINPERSONNEL");
				i=0;
				while(strcmp(w->getPersonnel(i), M.arg.Selection.S1)) { i++; }
				w->setPersonnel(i, "");
				while(strcmp(w->getDisponible(i), M.arg.Selection.S1)) { i++; }
				w->setDisponible(i, "");
				
				break;
		}
	}
	return;
}
