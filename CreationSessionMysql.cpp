#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>

MYSQL			*Connexion;
MYSQL_RES	*Resultat;
MYSQL_ROW	Ligne;
int main(int argc,char *argv[]) {

	Connexion = mysql_init(NULL);
	mysql_real_connect(Connexion,"localhost","student","PassStudent","PourStudent",0,0,0);
	if (mysql_query(Connexion,"drop table Dossier2020;")) { 
		printf("Err de drop : %s\n",mysql_error(Connexion));
		exit(1);
	}

	if (mysql_query(Connexion,"create table Dossier2020 (id INT(4) auto_increment primary key,\
		NomCommande varchar(12),C1 varchar(12),T1 INT(4),C2 varchar(12),T2 INT(4),C3 varchar(12),T3 INT(4),\
	 C4 varchar(12),T4 INT(4),AEffectuer INT(4),ALivre INT(4));")) { 
		printf("Err de create : %s\n",mysql_error(Connexion));
		exit(1);
	}
	
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande1','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande2','Et 1',1,\
		'Et 2',1,'',-1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}

	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande3','Et 1',1,\
		'Et 2',1,'Et3',1,'Et4',1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande4','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande5','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande6','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande7','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	if (mysql_query(Connexion,"insert into Dossier2020 values (NULL,'Commande8','Etape 1',1,\
		'Etape 2',1,'Etape 3',1,'',-1,1,1);")) { 
		printf("Err de insert : %s\n",mysql_error(Connexion));
		exit(1);
	}
	exit(0);
}
