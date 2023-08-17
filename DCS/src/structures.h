#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <vector>
using namespace std;

typedef struct s_dep {
	int id;
	int seq;
	simtime_t recvtime;
	int incrComponent;

	s_dep(unsigned int id, unsigned int seq, simtime_t recvtime,
			unsigned int incrComponent) {
		this->id = id;
		this->seq = seq;
		this->recvtime = recvtime;
		this->incrComponent = incrComponent;
	}
} dep;

#define LIMIT_HASHS 20 // ATTENTION DOIT ÊTRE LE MÊME POUR LES MSG DANS COMPLETEHASHTEST ET RECOVERYTEST SINON DOIT
// AJOUTER ITERATIVEDELIVERY APRÈS RECOVERYMSG.PUSH_BACK DANS RECOVERYTEST SINON PEUT TROUVER
// UN ENSEMBLE DE DEPENDANCES QUE N'A PAS TROUVÉ DANS COMLPETEHASHTEST MAIS NE VA JAMAIS LE DÉLIVRER

#endif /* STRUCTURES_H_ */
