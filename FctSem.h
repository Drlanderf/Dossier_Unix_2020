#ifndef FCTSEM_H
#define FCTSEM_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

extern int idSem;

int SemWait(int Sema);
int SemSignal(int Sema);

#endif
