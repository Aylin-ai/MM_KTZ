#include <stdio.h>
#include <malloc.h>

//Черновая версия

int* min(int** c, int ACount, int BCount) {
    int min;
    int minI, minJ;
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            if (c[i][j] != -1) {
                min = c[i][j];
                minI = i;
                minJ = j;
                break;
            }
        }
    }
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            if (c[i][j] == -1) continue;
            if (min > c[i][j]) {
                min = c[i][j];
                minI = i;
                minJ = j;
            }
        }
    }
    int* result = (int*)malloc(2 * sizeof(int));
    result[0] = minI;
    result[1] = minJ;
    c[minI][minJ] = -1;
    printf("\nМинимальный с - %i на координатах [%i, %i]\n", min, minI, minJ);
    return result;
}

int* xSearch(int ACount, int BCount, int** x, int dI, int dJ, int isHorizontal, int* resultX, int index, int* isComplete) {
    if (index > 5) {
        *isComplete = 1;
        return 0;
    }
    if (isHorizontal == -1) {
        int leftX, rightX;
        int downX, upX;
        for (int j = 0; j < BCount; j++) {
            if (*isComplete == 1) return 0;
            if (x[dI][j] != 0 && j < dJ) {
                leftX = x[dI][j];
                resultX[index] = dI;
                resultX[index + 1] = j;
                xSearch(ACount, BCount, x, dI, j, 1, resultX, index + 2, isComplete);
            }
            else if (x[dI][j] != 0 && j > dJ) {
                rightX = x[dI][j];
                resultX[index] = dI;
                resultX[index + 1] = j;
                xSearch(ACount, BCount, x, dI, j, 1, resultX, index + 2, isComplete);
            }
        }
        for (int i = 0; i < ACount; i++) {
            if (*isComplete == 1) return 0;
            if (x[i][dJ] != 0 && i < dI) {
                upX = x[i][dJ];
                resultX[index] = i;
                resultX[index + 1] = dJ;
                xSearch(ACount, BCount, x, i, dJ, 0, resultX, index + 2, isComplete);
            }
            else if (x[i][dJ] != 0 && i > dI) {
                downX = x[i][dJ];
                resultX[index] = i;
                resultX[index + 1] = dJ;
                xSearch(ACount, BCount, x, i, dJ, 0, resultX, index + 2, isComplete);
            }
        }
    }
    else if (isHorizontal == 0) {
        int leftX, rightX;
        for (int j = 0; j < BCount; j++) {
            if (*isComplete == 1) return 0;
            if (x[dI][j] != 0 && j < dJ) {
                leftX = x[dI][j];
                resultX[index] = dI;
                resultX[index + 1] = j;
                xSearch(ACount, BCount, x, dI, j, 1, resultX, index + 2, isComplete);
            }
            else if (x[dI][j] != 0 && j > dJ) {
                rightX = x[dI][j];
                resultX[index] = dI;
                resultX[index + 1] = j;
                xSearch(ACount, BCount, x, dI, j, 1, resultX, index + 2, isComplete);
            }
        }
    }
    else if (isHorizontal == 1) {
        int downX, upX;
        for (int i = 0; i < ACount; i++) {
            if (*isComplete == 1) return 0;
            if (x[i][dJ] != 0 && i < dI) {
                upX = x[i][dJ];
                resultX[index] = i;
                resultX[index + 1] = dJ;
                xSearch(ACount, BCount, x, i, dJ, 0, resultX, index + 2, isComplete);
            }
            else if (x[i][dJ] != 0 && i > dI) {
                downX = x[i][dJ];
                resultX[index] = i;
                resultX[index + 1] = dJ;
                xSearch(ACount, BCount, x, i, dJ, 0, resultX, index + 2, isComplete);
            }
        }
    }
}

int* potentialSearch(int ACount, int BCount, int** c, int** x) {
    int* V = (int*)malloc(BCount * sizeof(int));
    int* U = (int*)malloc(ACount * sizeof(int));

    for (int i = 0; i < ACount; i++) {
        U[i] = -999;
    }
    for (int i = 0; i < BCount; i++) {
        V[i] = -999;
    }

    U[0] = 0;
    for (int k = 0; k < 2; k++){
        for (int i = 0; i < ACount; i++) {
            for (int j = 0; j < BCount; j++) {
                if (V[j] == -999 && U[i] != -999 && x[i][j] != 0) {
                    V[j] = c[i][j] + U[i];
                    printf("V[%i] = %i + %i = %i\n", j, c[i][j], U[i], V[j]);
                    continue;
                }
                else if (U[i] == -999 && V[j] != -999 && x[i][j] != 0) {
                    U[i] = V[j] - c[i][j];
                    printf("U[%i] = %i - %i = %i\n", i, V[j], c[i][j], U[i]);
                    continue;
                }
            }
        }
    }
    int* result = (int*)malloc((ACount + BCount) * sizeof(int));
    printf("\nU - ");
    for (int i = 0; i < ACount; i++) {
        printf("%i\t", U[i]);
        result[i] = U[i];
    }
    printf("\nV - ");
    for (int i = ACount, j = 0; i < BCount + ACount, j < BCount; i++, j++) {
        printf("%i\t", V[j]);
        result[i] = V[j];
    }
    printf("\n");
    return result;
}

int** deltaSearch(int ACount, int BCount, int* U, int* V, int** c, int* isOptimal) {
    *isOptimal = 1;

    int** d = (int**)malloc(ACount * sizeof(int*));
    for (int i = 0; i < ACount; i++) {
        d[i] = (int*)malloc(BCount * sizeof(int));
    }
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            d[i][j] = V[j] - U[i] - c[i][j];
            printf("d[%i][%i] = %i - %i - %i = %i\n", i, j, V[j], U[i], c[i][j], d[i][j]);
            if (d[i][j] > 0) *isOptimal = 0;
        }
    }
    printf("\nДельты\n");
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            printf("%i\t", d[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return d;
}

int** optimalSearch(int ACount, int BCount, int* U, int* V, int** c, int** x, int** d, int* isOptimal) {
    int dI, dJ;
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            if (d[i][j] > 0) {
                dI = i;
                dJ = j;
                break;
            }
        }
    }
    int* resultX = (int*)malloc(6 * sizeof(int));
    int* isComplete = (int*)malloc(sizeof(int));
    *isComplete = 0;
    xSearch(ACount, BCount, x, dI, dJ, -1, resultX, 0, isComplete);
    int x1 = x[resultX[0]][resultX[1]];
    int x2 = x[resultX[2]][resultX[3]];
    int x3 = x[resultX[4]][resultX[5]];

    int minX;
    int mI, mJ;
    if (x1 <= x3) {
        minX = x1;
        mI = resultX[0];
        mJ = resultX[1];
        x[resultX[2]][resultX[3]] = x2 + minX;
        x[resultX[4]][resultX[5]] = x3 - minX;
    } else if (x3 <= x1) {
        minX = x3;
        mI = resultX[4];
        mJ = resultX[5];
        x[resultX[0]][resultX[1]] = x1 - minX;
        x[resultX[2]][resultX[3]] = x3 + minX;
    }

    x[dI][dJ] = minX;
    x[mI][mJ] = 0;

    printf("\nОбновленный X\n");
    for (int i = 0; i < ACount; i++){
        for (int j = 0; j < BCount; j++){
            printf("%i\t", x[i][j]);
        }
        printf("\n");
    }

    int* UV = (int*)malloc((ACount + BCount) * sizeof(int));
    UV = potentialSearch(ACount, BCount, c, x);

    for (int i = 0; i < ACount; i++) {
        U[i] = UV[i];
    }
    for (int i = ACount, j = 0; i < BCount + ACount && j < BCount; i++, j++) {
        V[j] = UV[i];
    }

    return deltaSearch(ACount, BCount, U, V, c, isOptimal);
}

int ktz(int ACount, int BCount,
    int* A, int* B, int** c) {
    
    int aCount = 0, bCount = 0;
    for (int i = 0, j = 0; i < ACount, j < BCount; i++, j++){
        aCount += A[i];
        bCount += B[j];
    }
    if (aCount != bCount){
        printf("Данные для решения задачи неккоректны. Выход из программы...\n");
        return 0;
    }

    int** cCopy = (int**)malloc(ACount * sizeof(int*));
    for (int i = 0; i < ACount; i++) {
        cCopy[i] = (int*)malloc(BCount * sizeof(int));
    }
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            cCopy[i][j] = c[i][j];
        }
    }
    int** x = (int**)malloc(ACount * sizeof(int*));
    for (int i = 0; i < ACount; i++) {
        x[i] = (int*)malloc(BCount * sizeof(int));
    }
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            x[i][j] = 0;
        }
    }
    while (1) {
        int* minCoordinates = malloc(2 * sizeof(int));
        minCoordinates = min(c, ACount, BCount);
        int a = A[minCoordinates[0]];
        int b = B[minCoordinates[1]];
        if (a > b) {
            x[minCoordinates[0]][minCoordinates[1]] = b;
            B[minCoordinates[1]] = 0;
            A[minCoordinates[0]] = a - b;
            for (int i = 0; i < ACount; i++) {
                c[i][minCoordinates[1]] = -1;
            }
        }
        else if (a < b) {
            x[minCoordinates[0]][minCoordinates[1]] = a;
            A[minCoordinates[0]] = 0;
            B[minCoordinates[1]] = b - a;
            for (int j = 0; j < BCount; j++) {
                c[minCoordinates[0]][j] = -1;
            }
        }
        else {
            x[minCoordinates[0]][minCoordinates[1]] = b;
            B[minCoordinates[1]] = 0;
            A[minCoordinates[0]] = a - b;
            for (int i = 0; i < ACount; i++) {
                c[i][minCoordinates[1]] = -1;
            }
        }
        int endACount = 0;
        int endBCount = 0;
        for (int i = 0; i < ACount; i++) {
            if (A[i] == 0) endACount++;
        }
        for (int i = 0; i < BCount; i++) {
            if (B[i] == 0) endBCount++;
        }
        printf("\nОбновленный с\n");
        for (int i = 0; i < ACount; i++){
            for (int j = 0; j < BCount; j++){
                printf("%i\t", c[i][j]);
            }
            printf("\n");
        }
        printf("\nОбновленный X\n");
        for (int i = 0; i < ACount; i++){
            for (int j = 0; j < BCount; j++){
                printf("%i\t", x[i][j]);
            }
            printf("\n");
        }
        if (endACount == ACount || endBCount == BCount) {
            break;
        }
    }
    printf("\nИтоговый с\n");
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            printf("%i\t", c[i][j]);
        }
        printf("\n");
    }
    printf("\nX\n");
    for (int i = 0; i < ACount; i++) {
        for (int j = 0; j < BCount; j++) {
            printf("%i\t", x[i][j]);
        }
        printf("\n");
    }

    int* UV = (int*)malloc((ACount + BCount) * sizeof(int));
    UV = potentialSearch(ACount, BCount, cCopy, x);

    int* V = (int*)malloc(BCount * sizeof(int));
    int* U = (int*)malloc(ACount * sizeof(int));

    for (int i = 0; i < ACount; i++) {
        U[i] = UV[i];
    }
    for (int i = ACount, j = 0; i < BCount + ACount && j < BCount; i++, j++) {
        V[j] = UV[i];
    }

    int* isOptimal = (int*)malloc(sizeof(int));

    int** d = deltaSearch(ACount, BCount, U, V, cCopy, isOptimal);

    while (*isOptimal == 0){
        printf("\nПлан неоптимален. Перестройка...\n");
        d = optimalSearch(ACount, BCount, U, V, cCopy, x, d, isOptimal);
    }

}

int main() {
    
    int aCount, bCount;
    
    printf("Введите размер А: ");
    scanf("%i", &aCount);
    
    printf("\nВведите размер B: ");
    scanf("%i", &bCount);
    
    int* A = malloc(aCount * sizeof(int));
    int* B = malloc(bCount * sizeof(int));
    
    for (int i = 0; i < aCount; i++){
        printf("\nВведите А[%i] - ", i);
        scanf("%i", &A[i]);
    }
    
    for (int i = 0; i < bCount; i++){
        printf("\nВведите B[%i] - ", i);
        scanf("%i", &B[i]);
    }
    
    int** c = (int**)malloc(aCount * sizeof(int*));
    for (int i = 0; i < aCount; i++){
        c[i] = (int*)malloc(bCount * sizeof(int));
        for (int j = 0; j < bCount; j++){
            printf("\nВведите с[%i][%i] - ", i, j);
            scanf("%i", &c[i][j]);
        }
    }

    printf("Изначальные А\n");
    for (int i = 0; i < aCount; i++){
        printf("%i\t", A[i]);
    }

    printf("\nИзначальные B\n");
    for (int i = 0; i < bCount; i++){
        printf("%i\t", B[i]);
    }

    printf("\nИзначальные C\n");
    for (int i = 0; i < aCount; i++) {
        for (int j = 0; j < bCount; j++) {
            printf("%i\t", c[i][j]);
        }
        printf("\n");
    }

    ktz(aCount, bCount, A, B, c);
    return 0;
}
