#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100


void afficherTableau(double tableau[][MAX], int m, int n, int baseVariables[]) {
    printf("\n \t \t\t Tableau actuel  \n");
    printf("|  Base  |");
    for (int j = 0; j < n - 1; j++) {
        printf("   x%d   |", j + 1);
    }
    printf("   b    |\n");

    for (int i = 0; i <= m; i++) {
        printf("+--------+");
        for (int j = 0; j < n; j++) {
            printf("--------+");
        }
        printf("\n|");

        if (i < m) {
            printf("   x%d   ", baseVariables[i] + 1);
        } else {
            printf("   -Z   ");
        }
        printf("|");

        for (int j = 0; j < n; j++) {
            printf(" %6.2f |", tableau[i][j]);
        }
        printf("\n");
    }
    printf("+--------+");
    for (int j = 0; j < n; j++) {
        printf("--------+");
    }
    printf("\n");
}


void simplex(double c[], double A[][MAX], double b[], int n, int m, char problemType) {
    int tableau_m = m + 1;
    int tableau_n = n + m + 1;
    double tableau[MAX][MAX] = {0};
    int baseVariables[MAX] = {0};

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            tableau[i][j] = A[i][j];
        }
        tableau[i][n + i] = 1.0;
        tableau[i][tableau_n - 1] = b[i];
        baseVariables[i] = n + i;
    }

    for (int j = 0; j < n; j++) {
        tableau[m][j] = (problemType == 'M') ? c[j] : -c[j];
    }

    afficherTableau(tableau, m, tableau_n, baseVariables);

    while (1) {
        int colPivot = -1;
        double extremeValue = 0;
        for (int j = 0; j < tableau_n - 1; j++) {
            if ((problemType == 'M' && tableau[m][j] > extremeValue) ||
                (problemType == 'm' && tableau[m][j] < extremeValue)) {
                extremeValue = tableau[m][j];
                colPivot = j;
            }
        }
        if (colPivot == -1) break;

        int rowPivot = -1;
        double minRatio = 1e9;
        for (int i = 0; i < m; i++) {
            if (tableau[i][colPivot] > 0) {
                double ratio = tableau[i][tableau_n - 1] / tableau[i][colPivot];
                if (ratio < minRatio && ratio >= 0) {
                    minRatio = ratio;
                    rowPivot = i;
                }
            }
        }
        if (rowPivot == -1) {
            printf("\nLe probleme est non borne.\n");
            return;
        }

        baseVariables[rowPivot] = colPivot;
        double pivot = tableau[rowPivot][colPivot];

        for (int j = 0; j < tableau_n; j++) {
            tableau[rowPivot][j] /= pivot;
        }

        for (int i = 0; i <= m; i++) {
            if (i != rowPivot) {
                double factor = tableau[i][colPivot];
                for (int j = 0; j < tableau_n; j++) {
                    tableau[i][j] -= factor * tableau[rowPivot][j];
                }
            }
        }

        afficherTableau(tableau, m, tableau_n, baseVariables);
    }

    printf("\n### Solution optimale ###\n");
    for (int i = 0; i < n; i++) {
        int isBasic = 0;
        for (int j = 0; j < m; j++) {
            if (baseVariables[j] == i) {
                printf("x%d = %.2lf\n", i + 1, tableau[j][tableau_n - 1]);
                isBasic = 1;
                break;
            }
        }
        if (!isBasic) {
            printf("x%d = 0.00\n", i + 1);
        }
    }

    double zValue = -tableau[m][tableau_n - 1];
    if (problemType == 'm') zValue = -zValue;
    printf("Valeur optimale de Z = %.2lf\n", zValue);
}

int main() {
    int n, m;
    char problemType;
    double c[MAX], A[MAX][MAX], b[MAX];

    printf("****************************** BEN DRIF ABDELHAMID SMI-S5 ******************************\n\n");
    printf("********** Methode du Simplexe **********\n");

    printf("Entrez 'M' pour maximisation ou 'm' pour minimisation : ");
    scanf(" %c", &problemType);

    printf("Entrez le nombre de variables de decision : ");
    scanf("%d", &n);

    printf("Entrez le nombre de contraintes : ");
    scanf("%d", &m);

    printf("\n*** Fonction Objectif ***\n");
    for (int i = 0; i < n; i++) {
        printf("Coefficient de x%d : ", i + 1);
        scanf("%lf", &c[i]);
    }

    printf("\n*** Contraintes ***\n");
    for (int i = 0; i < m; i++) {
        printf("\nContrainte %d :\n", i + 1);
        for (int j = 0; j < n; j++) {
            printf("Coefficient de x%d : ", j + 1);
            scanf("%lf", &A[i][j]);
        }
        printf("Membre constant b[%d] : ", i + 1);
        scanf("%lf", &b[i]);
    }

    simplex(c, A, b, n, m, problemType);
    return 0;
}
