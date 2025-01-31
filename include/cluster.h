#ifndef DABTREE_CLUSTER_H_
#define DABTREE_CLUSTER_H_

#include <iostream>
#include "mpi.h"
#include "tree.h"
#include "message_tree.h"
#include <cstring>


#define DLLEXPORT extern "C"
using namespace std;



EnsembleMessageTreePtr Master(int round, int comm_sz, MPI_Datatype &MPI_TREE);

void Worker(int round, int myrank, MPI_Datatype &MPI_TREE);

void BunddleMaster(int round, int comm_sz, MPI_Datatype &MPI_TREE);
void BunddleWorker(int round, int myrank, MPI_Datatype &MPI_TREE);
DLLEXPORT int Dabtree(int round,bool bunddle=true);


#endif
