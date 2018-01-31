#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Matrix{
    int **matrix;
    int row;
    int col;
};
 struct Args{
    int *arr;
    int size;
};

int tracker = 0;
struct Matrix *matrixChain;

void init(struct Matrix *m){
    int i;
    m->matrix = (int **) malloc(m->row * sizeof(int *));
    for(i=0; i<m->row; i++){
        m->matrix[i] = (int *) malloc(m->col * sizeof(int));
    }
}

void assignValue(struct Matrix *m){
    int count = 0;
    int i,j;
    for(i=0; i<m->row; i++)
        for(j=0; j<m->col; j++)
            m->matrix[i][j] = ++count;
}

void initMatrixChain(int arr[],int size){
    int i;
    tracker = size-1;
    matrixChain = malloc((size-1)*sizeof(struct Matrix));
    for(i=0; i<size-1; i++){
        matrixChain[i].row = arr[i];
        matrixChain[i].col = arr[i+1];
        init(&matrixChain[i]);
        assignValue(&matrixChain[i]);
    }
}

void print(struct Matrix *m){
    int i,j;
    for(i=0; i<m->row; i++){
        for(j=0; j<m->col; j++){
            printf("%d  ",m->matrix[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");
}

void multiplyMatrices(int x,int y){
    if(matrixChain[x].col == matrixChain[y].row){
        int i,j,k;
        struct Matrix resultMatrix;
        resultMatrix.row = matrixChain[x].row;
        resultMatrix.col = matrixChain[y].col;
        init(&resultMatrix);

        for(i=0; i<matrixChain[x].row; i++){
            for(j=0; j<matrixChain[y].col; j++){
                resultMatrix.matrix[i][j] = 0;
                for(k=0; k<matrixChain[x].col; k++){

                    resultMatrix.matrix[i][j] = resultMatrix.matrix[i][j] + matrixChain[x].matrix[i][k] * matrixChain[y].matrix[k][j];

                }
            }
        }

        matrixChain[y] = resultMatrix;
        //print(&matrixChain[y]);
    }
    else{
        exit(EXIT_FAILURE);
    }
}

void* startCalculation(void* a){

    struct Args *args = a;
    initMatrixChain(args->arr,args->size);

    int i;
    for(i=0; i<tracker-1; i++){
        multiplyMatrices(i,i+1);
    }
    print(&matrixChain[tracker-1]);
}

int main()
{
    int threadId;
    int arr[5] = {1,2,3,4,5};
    struct Args arg;
    arg.arr = arr;
    arg.size = 5;
    pthread_t thread_1;
    //iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
    threadId = pthread_create(&thread_1, NULL, startCalculation, &arg);

    pthread_join(thread_1,NULL);

    return 0;
}
