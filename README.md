# MPI-Distributed-Integral
Program to calc integrals in a distributed way using MPI lib for a Computer Science project

## Como rodar o programa ?

### Preparação
Necessário incluir a biblioteca MPI ao sistema. Rode o comando ```sudo apt install libopenmpi-dev``` no terminal e espere finalizar.

### Executando
Para compilar o arquivo digite em um terminal ```make makefile compile```

Para executar utilize o comando ```make makefile slave{x}``` onde {X} é referente ao número de escravos desejados, por exemplo:

- 1 escravo  :  ```make makefile slave1```
- 2 escravo  :  ```make makefile slave2```
- 4 escravo  :  ```make makefile slave4```
- 7 escravo  :  ```make makefile slave7```
- 10 escravo :  ```make makefile slave10```

Caso deseje rodar com valores de escravos diferente de 1, 2, 4, 7 e 10, basta utilizar o comando ```mpirun --use-hwthread-cpus -np {x} -quiet ./main```, onde {x} é o numero de escravos + 1. Ou seja, se desejar 15 escravos é necessario colocar o valor 16