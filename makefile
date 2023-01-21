.SILENT:
all: Administrateur FacturationTransfert FenetrePersonnel Serveur SuiviCommande CreationSessionMysql

CreationSessionMysql:	CreationSessionMysql.cpp Ecran.o
						g++ -m64 -Wall -I Ecran -o CreationSessionMysql CreationSessionMysql.cpp \
						-I /usr/mysql/5.6/include -L/usr/mysql/5.6/lib/amd64 -R/usr/mysql/5.6/lib/amd64 -lmysqlclient -lthread -lsocket -lnsl -lm  -I Ecran Ecran.o
						echo [+] CreationSessionMysql 

Administrateur:			mainAdministrateur.o windowadministrateur.o Ecran.o moc_windowadministrateur.o
						g++ -m64 -Wall -I Ecran -Wl,-R,/opt/Qt4.8/lib -o Administrateur mainAdministrateur.o  windowadministrateur.o Ecran.o  moc_windowadministrateur.o \
						-L/usr/lib/64 -L/usr/X11/lib/64 -L/opt/Qt4.8/lib -lQtGui -L/opt/Qt4.8/lib -L/usr/lib/64 -L/usr/X11/lib/64 -lQtCore -lpthread -lrt
						echo [+] Administrateur

FacturationTransfert:	FacturationTransfert.cpp Ecran.o	
						g++ -m64 -Wall -I Ecran -o FacturationTransfert FacturationTransfert.cpp Ecran.o
						echo [+] FacturationTransfert

FenetrePersonnel:		mainFenetrePersonnel.o windowFenetrePersonnel.o Ecran.o moc_windowFenetrePersonnel.o
						g++ -m64 -Wall -I Ecran -Wl,-R,/opt/Qt4.8/lib -o FenetrePersonnel mainFenetrePersonnel.o  windowFenetrePersonnel.o Ecran.o  moc_windowFenetrePersonnel.o \
						-L/usr/lib/64 -L/usr/X11/lib/64 -L/opt/Qt4.8/lib -lQtGui -L/opt/Qt4.8/lib -L/usr/lib/64 -L/usr/X11/lib/64 -lQtCore -lpthread -lrt
						echo [+] FenetrePersonnel
						
Serveur:				Serveur.cpp FctSem.o
						g++ -m64 -Wall -I Ecran -o Serveur Serveur.cpp FctSem.o \
						-I /usr/mysql/5.6/include -L/usr/mysql/5.6/lib/amd64 -R/usr/mysql/5.6/lib/amd64 -lmysqlclient -lthread -lsocket -lnsl -lm  -I Ecran Ecran.o
						echo [+] Serveur

SuiviCommande:			mainsuivicommande.o  windowsuivicommande.o Ecran.o  moc_windowsuivicommande.o
						g++ -m64 -Wall -I Ecran -Wl,-R,/opt/Qt4.8/lib -o SuiviCommande mainsuivicommande.o  windowsuivicommande.o Ecran.o  moc_windowsuivicommande.o \
						-L/usr/lib/64 -L/usr/X11/lib/64 -L/opt/Qt4.8/lib -lQtGui -L/opt/Qt4.8/lib -L/usr/lib/64 -L/usr/X11/lib/64 -lQtCore -lpthread -lrt
						echo [+] SuiviCommande
			
Ecran.o:				Ecran.cpp
						g++ -m64 -Wall -I Ecran -c Ecran.cpp -D SUN
						echo [+] Ecran.o

mainAdministrateur.o:			mainAdministrateur.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -IEcran -I. -I. \
								-I/usr/include -I/usr/X11/include -o mainAdministrateur.o mainAdministrateur.cpp
								echo [+] mainAdministrateur.o

windowadministrateur.o:			windowadministrateur.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I Ecran -I. \
								-I. -I/usr/include -I/usr/X11/include -o windowadministrateur.o windowadministrateur.cpp
								echo [+] windowadministrateur.o


mainFenetrePersonnel.o:			mainFenetrePersonnel.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -IEcran -I. -I. \
								-I/usr/include -I/usr/X11/include -o mainFenetrePersonnel.o mainFenetrePersonnel.cpp
								echo [+] mainFenetrePersonnel.o

windowFenetrePersonnel.o:		windowFenetrePersonnel.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I Ecran -I. -I. \
								-I/usr/include -I/usr/X11/include -o windowFenetrePersonnel.o windowFenetrePersonnel.cpp
								echo [+] windowFenetrePersonnel.o

mainsuivicommande.o:			mainsuivicommande.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -IEcran -I. -I. \
								-I/usr/include -I/usr/X11/include -o mainsuivicommande.o mainsuivicommande.cpp
								echo [+] mainsuivicommande.o

windowsuivicommande.o:			windowsuivicommande.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I Ecran -I. -I. \
								-I/usr/include -I/usr/X11/include -o windowsuivicommande.o windowsuivicommande.cpp
								echo [+] windowsuivicommande.o

moc_windowadministrateur.o:		moc_windowadministrateur.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I. -I. -I. \
								-I/usr/include -I/usr/X11/include -o moc_windowadministrateur.o moc_windowadministrateur.cpp
								echo [+] moc_windowadministrateur.o

moc_windowFenetrePersonnel.o:	moc_windowFenetrePersonnel.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I. -I. -I. -I/usr/include \
								-I/usr/X11/include -o moc_windowFenetrePersonnel.o moc_windowFenetrePersonnel.cpp
								echo [+] moc_windowFenetrePersonnel.o

moc_windowsuivicommande.o:		moc_windowsuivicommande.cpp
								g++ -m64 -Wall -I Ecran -c -D_XOPEN_SOURCE=600 -D__EXTENSIONS__ -O2 -Wall -W \
								-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED -I/opt/Qt4.8/mkspecs/solaris-g++-64 -I. \
								-I/opt/Qt4.8/include/QtCore -I/opt/Qt4.8/include/QtGui -I/opt/Qt4.8/include -I. -I. -I. -I/usr/include \
								-I/usr/X11/include -o moc_windowsuivicommande.o moc_windowsuivicommande.cpp
								echo [+] moc_windowsuivicommande.o
								
FctSem.o:				FctSem.cpp FctSem.h
						g++ -m64 -Wall -I FctSem -c FctSem.cpp FctSem.h -D SUN
						echo [+] FctSem.o

clean:
	rm *.o Administrateur FacturationTransfert FenetrePersonnel Serveur SuiviCommande CreationSessionMysql





