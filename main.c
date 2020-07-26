#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

#define MASTER 0

// Variaveis da Integral
double discretization = 0.0001;
double gap = 0.0;
double lastTotal = 0.0;
double total = 0.0;
double calculated = 0.0;

// Função para calculo da integral
double integral(double gap, double discretization){
  // Trapeze Rule:
  // h = (b - a) / N

  double total = 0;
  double xa = 0;
  double xb = 0;

  xa = sqrt((100 * 100) - (gap * gap));
  xb = sqrt((100 * 100) - ((gap + discretization) * (gap + discretization)));

  total = ((xa + xb) * discretization) / 2;

  return total;
}

// Rotina executada pelo Mestre
void Master(int sizeOfCluster){
    
    for(int process = 1; process < sizeOfCluster; process++) {

        MPI_Send(&discretization, 1, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
        MPI_Send(&gap, 1, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
        //printf("[MASTER] Descritizacao = %f\n", discretization);
        //printf("[MASTER] GAP = %f\n", gap);

        gap = gap + discretization;
    }
    
    for(int process = 1; process < sizeOfCluster; process++) {

        MPI_Recv(&calculated, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        lastTotal = total;
        total = calculated + total;
        //printf("[MASTER] Atualizando valor de integral: %f\n", total);
    }

}

// Rotina executada pelo EScravo
void Slave(int slaveRank){

    MPI_Recv(&discretization, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&gap, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //printf("[%d] Discretizacao: < %f\n",slaveRank, discretization);
    //printf("[%d] GAP: < %f\n",slaveRank, gap);
    
    calculated = integral(gap, discretization);
    //printf("[%d] Integral: %f\n", slaveRank, calculated);
    
    MPI_Send(&calculated, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD); 

}

// Função principal
int main(int argc, char *argv[]) {

    // Variavel que eu n sei oq faz
    int processRank, sizeOfCluster, process, done = 0;

    // Variavel para medir tempo
    float t1, t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeOfCluster); // Quantidade de processos rodando
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank); // Indice do processo rodando

    if(processRank == MASTER){
        printf("Calculando integral com discretizacao = %f\n", discretization); 
        printf("AGUARDE...\n"); 
    }

    t1 = clock(); 
    while(gap + discretization <= 100){

        if(processRank == MASTER) Master(sizeOfCluster);
        
        else Slave(processRank);

        // Sincroniza processos
        MPI_Barrier(MPI_COMM_WORLD);    

    }
    t2 = clock();
    // Printar o resultado final
    float time = (t2 - t1) / CLOCKS_PER_SEC;

    if(processRank == MASTER){

        // Log de Resultados
        printf("\n============== RESULTADO ==============");
        printf("\n> NUMERO DE ESCRAVOS      : %d", sizeOfCluster-1);
        printf("\n> VALOR FINAL DA INTEGRAL : %f", lastTotal);
        printf("\n> TEMPO TOTAL DE EXECUCAO : ");
        printf("%fs", time);
        printf("\n=======================================\n\n ");

    }
    
    return 0;
    
}
