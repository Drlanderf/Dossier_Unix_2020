#include "FctSem.h"

int SemWait(int Sema) {
	struct sembuf SemOp;
	
	SemOp.sem_num = Sema;
	SemOp.sem_op = -1;
	SemOp.sem_flg = SEM_UNDO;
	
	return semop(idSem, &SemOp, 1);
}


int SemSignal(int Sema) {
	struct sembuf SemOp;
	
	SemOp.sem_num = Sema;
	SemOp.sem_op = +1;
	SemOp.sem_flg = SEM_UNDO;
	
	return semop(idSem, &SemOp, 1);
}
