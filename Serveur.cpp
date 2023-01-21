#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <mysql.h>

#include "Fichier.ini"
#include "Ecran.h"
#include "FctSem.h"

bool etreDans(int vecteur[8], int element) {
	for (int i=0;i<8;i++) {
		if (vecteur[i] == element) return true;
	}
	return false;
}

int idMsg;
int idShm;
int idSem;
char* p;
int CptCommandeEnCours = 0;
int CptCommandeTransmise = 0;

MESSAGE	M;

TABSERVEUR*	s;
int hdPipe[2];

void AfficheTab();
MYSQL		*Connexion;
MYSQL_RES	*Resultat;
MYSQL_ROW	Ligne;

void handlerInt(int);

struct sigaction sig;
int afficher = 0;

int main(int argc,char* argv[]) {
	int	rc;
	int i=0;
	char lectPipe[5];
	sig.sa_handler = handlerInt;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sigaction(SIGINT, &sig, NULL);
	
//	s = (TABSERVEUR*)malloc(sizeof(TABSERVEUR));
	// Creation des ressources
	
	if ((idSem = semget(KEY, 1, 0)) == -1) {
		if ((idSem = semget(KEY, 1, IPC_CREAT|IPC_EXCL|0600)) == -1) {
			perror("\x1b[31merreur de creation de creation du semaphores\x1b[0m\n");
			exit(1);
		}
	}
	Trace("semaphores cree\n");
	if ((idShm = shmget(KEY,0, 0)) == -1) {
		if ((idShm = shmget(KEY, 512, IPC_CREAT|IPC_EXCL|0600)) == -1) {
			perror("\x1b[31merreur de creation de la memoire partage\x1b[0m\n");
			exit(1);
		}
	}
	Trace("memoire cree\n");
	
	if ((idMsg = msgget(KEY,0)) == -1) {
		if ((idMsg = msgget(KEY,IPC_CREAT|IPC_EXCL|0600)) == -1) {
			perror("\x1b[31merreur de creation de la file de message\x1b[0m\n");
			exit(1);
		}
	}

	SemWait(1);
	if ((s = (TABSERVEUR*)shmat(idShm, NULL, 0)) == (TABSERVEUR*)-1) {
		perror("Err de shmat()");
		exit(1);
	}
	Trace("le poiteur de TABSERVEUR pointe la memoire partage\n");
	if (s->Serveur1 == 0) {
		s->Serveur1 = getpid();
	}
	else if (s->Serveur2 == 0) {
		s->Serveur2 = getpid();
	}
	else {
		if (kill(s->Serveur1, 0)) {	//on test si le serveur1 n'a pas ete arrete de maniere inhabituelle
			s->Serveur1 = getpid();
		}
		else if (kill(s->Serveur2, 0)) {//on test si le serveur2 n'a pas ete arrete de maniere inhabituelle
			s->Serveur2 = getpid();
		}
		else {
			Trace("\033[31mIl y a deja 2 serveurs !\033[0m");
			exit(1);
		}
	}
	SemSignal(0);
		
	// Creation des processus FacturationTransfert
	
	if (s->idFils == 0 || kill(s->idFils, 0)) {		//si il n'y a pas encore de fils
		if (pipe(hdPipe)) {
			perror("erreur de pipe\n");
			exit(1);
		}
		if ((s->idFils = fork()) == -1) {
			perror("erreur de fork\n");
			exit(1);
		}
		if (s->idFils == 0) {
		s->idFils = getpid();
		//proccessur intermediaire
			int facturation;
			if ((facturation = fork()) == -1) {
				perror("\033[31merreur de fork\033[0m\n");
				exit(1);
			}
			if (!facturation) {
				//processus facturation
				if (close(hdPipe[1])) {
					perror("\terreur de close(hdPipe[1])\n");
					exit(1);
				}
				sprintf(lectPipe, "%d", hdPipe[0]);
				execl("./FacturationTransfert", "FacturationTransfert", lectPipe, (char*)NULL);
			}
			if (facturation) {
			//processus intermediaire
				if (close(hdPipe[0])) {
					perror("\terreur de close\n");
					exit(1);
				}
				while(1) {
					if (msgrcv(idMsg, &M, TAILLESELECTION, getpid(), 0) == -1) {	//processus intermediaire recoit le nom de la commande termine
						perror("erreur de msgrcv\n");
						exit(1);
					}
					write(hdPipe[1], M.arg.Selection.S2, sizeof(M.arg.Selection.S2));
				}
			}
		}
		//processus Pere
		if (close(hdPipe[0])) {
			perror("erreur fermeture lecture pipe pere");
			exit(1);	
		}
		if (close(hdPipe[1])) {
			perror("erreur fermeture ecriture pipe pere");
			exit(1);
		}
	}

	// connexion a la base Dossier2020
	Connexion = mysql_init(NULL);

	if (!mysql_real_connect(Connexion,"localhost","student","PassStudent","PourStudent",0,0,0)) { 
		Trace("Err de connexion  : %s\n",mysql_error(Connexion));
		exit(1);
	}

	for (int i=0;i<8;i++) s->etapeEnCours[i] = -1;
//	system("clear");
	AfficheTab();
	   
	char* sql;
	while(1) {
		if ((rc = msgrcv(idMsg,&M,TAILLECOMPLET,1L,0)) == -1) {
			TraceErreur("\x1b[31mErr de msgrcv()\x1b[0m");
			exit(1);
		}
		SemWait(1);
		system("clear");

		switch(M.Requete) {
			case NEWADMIN:	
				Trace("Message NEWADMIN %d",rc);
				s->Administrateur1 = M.idPid;
				mysql_query(Connexion, "SELECT NomCommande, id FROM Dossier2020 WHERE AEffectuer = 1;");
				Resultat = mysql_store_result(Connexion);
				for (i=0;i<4 && (Ligne = mysql_fetch_row(Resultat)) ;i++) {
					s->etapeEnCours[i] = atoi(Ligne[1]);
					strcpy(M.arg.Selection.S1, Ligne[0]);
					M.Type = s->Administrateur1;
					M.Requete = NEWCOMMANDE;
					if (msgsnd(idMsg, &M, TAILLESELECTION, 0)) {
						perror("\x1b[31merreur d'envoi\x1b[0m\n");
						exit(1);
					}
					CptCommandeTransmise++;
				}
				mysql_free_result(Resultat);
				sleep(1);
				kill(s->Administrateur1, SIGUSR1);
				
				break;

			case NEWPERSONNEL:			
				Trace("Message NEWPERSONNEL (%s) %d ",M.arg.Selection.S1,sizeof(M));
				i=0;
				while(s->Tab[i].idPid) { i++; }
				s->Tab[i].idPid = M.idPid;
				strcpy(s->Tab[i].Nom, M.arg.Selection.S1);
				
				M.Type = s->Administrateur1;
				M.Requete = NEWPERSONNEL;
				
				if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
					perror("\x1b[31merreur de send dans serveur.cpp\x1b[0m");
					exit(1);
				}
				kill(s->Administrateur1, SIGUSR1);	
				break;
			
			case AJOUTCOMMANDE:
				Trace("Reception AJOUTCOMMANDE");
				sql = (char*)malloc(190);
				
				sprintf(sql, "INSERT INTO Dossier2020 VALUES(NULL, \'%s\', \'%s\', %d, \'%s\', %d, \'%s\', %d, \'%s\', %d, 1, 1);",
				M.arg.C.NomC, M.arg.C.E[0], M.arg.C.T[0], M.arg.C.E[1], M.arg.C.T[1], M.arg.C.E[2], M.arg.C.T[2], M.arg.C.E[3], M.arg.C.T[3]);

				mysql_query(Connexion, sql);
				free(sql);
				mysql_query(Connexion, "SELECT id FROM Dossier2020 ORDER BY id DESC;");
				Resultat = mysql_store_result(Connexion);
				Ligne = mysql_fetch_row(Resultat);

				M.Type = s->Administrateur1;
				M.Requete = NUMEROCOMMANDE;
				M.arg.N = atoi(Ligne[0]);
				mysql_free_result(Resultat);
				if (msgsnd(idMsg, &M, TAILLEBASE, 0) == -1) {
					perror("\x1b[31merreur de send dans serveur.cpp\x1b[0m");
					exit(1);
				}
				if (CptCommandeTransmise < 4) {
					mysql_query(Connexion, "SELECT NomCommande, id FROM Dossier2020 WHERE AEffectuer = 1 ORDER BY id DESC;");
					Resultat = mysql_store_result(Connexion);
					Ligne = mysql_fetch_row(Resultat);
					M.Type = s->Administrateur1;
					M.Requete = NEWCOMMANDE;
					strcpy(M.arg.Selection.S1, Ligne[0]);
					i=0;
					while (s->etapeEnCours[i] != -1) { i++; }
					s->etapeEnCours[i] = atoi(Ligne[1]);
					mysql_free_result(Resultat);
					if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
						perror("\x1b[31merreur de send dans serveur.cpp\x1b[0m");
						exit(1);
					}
					CptCommandeTransmise++;
				}
				
				kill(s->Administrateur1, SIGUSR1);
					
				break;

			case TRANSMISSIONCOMMANDE:
				Trace("Message TRANSMISSIONCOMMANDE");
				//envoi a fenetrePersonnel la commande
				sql = (char*)malloc(120);
				sprintf(sql, "SELECT NomCommande, C1, C2, C3, C4, T1, T2, T3, T4 FROM Dossier2020 WHERE NomCommande = \'%s\';", M.arg.Selection.S1);
				mysql_query(Connexion, sql);
				free(sql);
				Resultat = mysql_store_result(Connexion);
				Ligne=mysql_fetch_row(Resultat);
				
				i=0;
				while (strcmp(s->Tab[i].Nom, M.arg.Selection.S2)) i++;
				M.Type = s->Tab[i].idPid;
				strcpy(M.arg.C.NomC, Ligne[0]);
				for (i=1;i<5;i++) {
					strcpy(M.arg.C.E[i-1], Ligne[i]);		//1 2 3 4
					M.arg.C.T[i-1] = atoi(Ligne[i+4]);		//5 6 7 8
				}
				mysql_free_result(Resultat);
				if (msgsnd(idMsg, &M, TAILLECOMPLET, 0) == -1) {
					perror("\x1b[31merreur de send\x1b[0m\n");
					exit(1);
				}
				CptCommandeTransmise--;
				kill(M.Type, SIGUSR1);

				//envoi d'une nouvelle commande a administrateur
				mysql_query(Connexion, "SELECT NomCommande, id FROM Dossier2020 WHERE AEffectuer = 1;");
				Resultat = mysql_store_result(Connexion);
				while ((Ligne = mysql_fetch_row(Resultat)) && etreDans(s->etapeEnCours, atoi(Ligne[1])));
				if (Ligne) {
					i=0;
					while (s->etapeEnCours[i] != -1) { i++; }
					s->etapeEnCours[i] = atoi(Ligne[1]);
					M.Type = s->Administrateur1;
					M.Requete = NEWCOMMANDE;
					strcpy(M.arg.Selection.S1, Ligne[0]);
					Trace("%s", M.arg.Selection.S1);
					mysql_free_result(Resultat);
					if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
						perror("\x1b[31merreur de send dans serveur.cpp\x1b[0m");
						exit(1);
					}
					CptCommandeTransmise++;
					kill(M.Type, SIGUSR1);
				}
	
				break;

			case SUIVICOMMANDE:			
				Trace("Message SUIVICOMMANDE");
				sql = (char*)malloc(140);
				M.Type = M.idPid;
				sprintf(sql, "SELECT C1, C2, C3, C4, T1, T2, T3, T4, ALivre FROM Dossier2020 WHERE NomCommande = \'%s\';", M.arg.Selection.S1);
				mysql_query(Connexion, sql);
				free(sql);
				Resultat = mysql_store_result(Connexion);
				if ((Ligne=mysql_fetch_row(Resultat)) == NULL) {		//si la commande entre n'existe pas
					M.Requete = 0;		//on lui envoie 0 pour une "code d'erreur"
				}
				else {
					i=0;
					while(s->Suivi[i].idPid) { i++; }
					s->Suivi[i].idPid = M.idPid;
					strcpy(s->Suivi[i].Nom, M.arg.Selection.S1);
				
					if (atoi(Ligne[8]) == 0) M.Requete = COMMANDELIVREE;
					for (i=0;i<4;i++) {
						strcpy(M.arg.C.E[i], Ligne[i]);		//0 1 2 3
						M.arg.C.T[i] = atoi(Ligne[i+4]);	//4 5 6 7
					}
				}
				mysql_free_result(Resultat);			
			
				if (msgsnd(idMsg, &M, TAILLECOMPLET, 0) == -1) {
					perror("\x1b[31merreur de send dans serveur.cpp\x1b[0m");
					exit(1);
				}
				break;

			case FINSUIVICOMMANDE:			
				Trace("Message FINSUIVICOMMANDE");
				i=0;
				while(s->Suivi[i].idPid != M.idPid) i++;
				s->Suivi[i].idPid = 0;
				strcpy(s->Suivi[i].Nom, "");
				break;

			case ETAPEEFFECTUEE:			
				Trace("Message ETAPEEFFECTUEE");
				sql = (char*)malloc(200);

				sprintf(sql, "UPDATE Dossier2020 set T%d = 0 WHERE NomCommande = \'%s\';", M.arg.C.T[0]+1, M.arg.C.NomC);
				mysql_query(Connexion, sql);
				free(sql);

				break;

			case COMMANDELIVREE:			
				Trace("Message COMMANDELIVREE");
				sql = (char*)malloc(200);

				sprintf(sql, "UPDATE Dossier2020 set ALivre = 0 WHERE NomCommande = \'%s\';", M.arg.Selection.S1);
				mysql_query(Connexion, sql);
				free(sql);
				break;

			case EVOLUTIONTRAVAIL:
				Trace("Message EVOLUTIONTRAVAIL");
				break;

			case TRAVAILTERMINER:
				Trace("Travail termine");
				sql = (char*)malloc(200);
				//supprimer la commande de etapeEnCours
				sprintf(sql, "SELECT id FROM Dossier2020 WHERE NomCommande = \'%s\';", M.arg.C.NomC);
				mysql_query(Connexion, sql);
				free(sql);
				Resultat = mysql_store_result(Connexion);
				Ligne=mysql_fetch_row(Resultat);
				i=0;
				while(s->etapeEnCours[i] != atoi(Ligne[0])) { i++; }
				s->etapeEnCours[i] = -1;
				mysql_free_result(Resultat);

				//mettre a jour la table (AEffectuer=0)
				sprintf(sql, "UPDATE Dossier2020 set AEffectuer = 0 WHERE NomCommande = \'%s\';", M.arg.C.NomC);
				mysql_query(Connexion, sql);
				free(sql);
				//dire au serveur que le personnel est disponible
				M.Type = s->Administrateur1;
				M.Requete = TRAVAILTERMINER;
				strcpy(M.arg.Selection.S2, M.arg.C.NomC);
				i=0;
				while (s->Tab[i].idPid != M.idPid) { i++; }
				strcpy(M.arg.Selection.S1, s->Tab[i].Nom);
				
				if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
					perror("\x1b[31merreur de send\x1b[0m\n");
					exit(1);
				}
				kill(s->Administrateur1, SIGUSR1);
				
				//transmettre a facturation
				M.Type = s->idFils;
				Trace("l'id du fils : %d\n", s->idFils);
				if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
					perror("erreur de send\n");
					exit(1);
				}
				break;

			case FINPERSONNEL:
				Trace("Message FINPERSONNEL");
				i=0;
				while (s->Tab[i].idPid != M.idPid) { i++; }
				M.Type = s->Administrateur1;
				strcpy(M.arg.Selection.S1, s->Tab[i].Nom);
				s->Tab[i].idPid = 0;
				strcpy(s->Tab[i].Nom, "");
				if (msgsnd(idMsg, &M, TAILLESELECTION, 0) == -1) {
					perror("\x1b[31merreur de send\x1b[0m\n");
					exit(1);
				}
				
				kill(s->Administrateur1, SIGUSR1);

				break;
			case FINADMINISTRATEUR:
				Trace("Message FINADMINISTRATEUR");
				s->Administrateur1 = 0;
		}
		SemSignal(0);
		AfficheTab();
	}

// Fin du processus Serveur1
}

void AfficheTab() {
	int i = 0;
	SemWait(1);
	printf("\tpid du Serveur1 :\t %d\n",s->Serveur1);
	printf("\tpid du Serveur2 :\t %d\n",s->Serveur2);
	printf("\tpid du Administrateur1 : %d\n",s->Administrateur1);
	printf("\tTab Utilisateurs:\n");
	while (i < NBMAX) { 
		if (s->Tab[i].idPid != 0) {
			printf("\t\t(i : %d)",i); 
			printf("\t%5d -%s- \n", s->Tab[i].idPid, s->Tab[i].Nom);
		}
		i++;
	}
	printf("\tTab Suivi:\n");
	i = 0;
	while (i < NBMAX)  { 
	   if (s->Suivi[i].idPid != 0) {
			printf("\t\t(i : %d)",i); 
			printf("\t%5d -%s- \n", s->Suivi[i].idPid,s->Suivi[i].Nom);
		}
	   i++;
	}
	i=0;
	printf("\tCommande en cours :\n");
	printf("\t[");
	while (i<7) {
		printf("%d, ", s->etapeEnCours[i]);
		i++;
	}
	printf("%d]\n", s->etapeEnCours[i]);
	SemSignal(0);
	return;
}


void handlerInt(int sig) {
	SemWait(1);
	if (s->Serveur1 == getpid()) {
		s->Serveur1 = 0;
	}
	else if(s->Serveur2 == getpid()){
		s->Serveur2 = 0;
	}
	if (shmdt(s) == -1 ) {
		perror("erreur de detachement de la memoire\n");
	}
	SemSignal(0);
	exit(0);
	
}












