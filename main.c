#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define MASTER 0

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

// Função principal
int main(int argc, char *argv[]) {

    // Variavel que eu n sei oq faz
    int processRank, sizeOfCluster, process;
    double messageItem;

    // Variaveis da Integral
    double discretization = 0.0001;
    double gap = 0.0;
    double total = 0.0;
    double calculated = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeOfCluster); // Quantidade de processos rodando
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank); // Indice do processo rodando

    printf("Iniciando processo %d de %d\n", processRank, sizeOfCluster); 

    while(gap + discretization <= 100){
        if(processRank == MASTER){

            if(gap + discretization <= 100){
                for(process = 1; process < sizeOfCluster; process++) {
                    MPI_Send(&discretization, 1, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
                    MPI_Send(&gap, 1, MPI_DOUBLE, process, 1, MPI_COMM_WORLD);
                    //printf("[MASTER] Descritizacao = %f\n", discretization);
                    //printf("[MASTER] GAP = %f\n", gap);

                    gap = gap + discretization;
                }
            } else MPI_Finalize();

            for(process = 1; process < sizeOfCluster; process++) {
                MPI_Recv(&calculated, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total = calculated + total;
                //printf("[MASTER] Atualizando valor de integral: %f\n", total);
            }
            
        } else {
            // Antes de esperar receber alguma coisa, verifica se ainda tem trabalho a ser feito
            // if (gap + discretization <= 100) {
            MPI_Recv(&discretization, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&gap, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("[%d] Discretizacao: < %f\n",processRank, discretization);
            //printf("[%d] GAP: < %f\n",processRank, gap);
            
            calculated = integral(gap, discretization);
            //printf("[%d] Integral: %f\n", processRank, calculated);

            
            MPI_Send(&calculated, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD); 
            // }
        }

        // SIncroniza processos
        MPI_Barrier(MPI_COMM_WORLD);    
        //puts("=== loop  ===");
    }
    
    printf("\nVALOR FINAL DA INTEGRAL: %f \n", total);
    return 0;
    
}
