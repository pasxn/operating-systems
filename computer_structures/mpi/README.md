#### MPI Functions

* int MPI_Init(int *argc, char** argv);
---
* int MPI_Comm_size(MPI_Comm comm, int* size);
* int MPI_Comm_rank(MPI_Comm comm, int* rank);

* int MPI_Send(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
* int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status);
* int MPI_Sendrecv(const void* sendbuf, int sendcount, 
                 MPI_Datatype sendtype,
                 int dest, int sendtag,
                 void* recvbuf, int recvcount,
                 MPI_Datatype recvtype,
                 int source, int recvtag,
                 MPI_Comm comm, MPI_Status* status);

* int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
                 MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);

