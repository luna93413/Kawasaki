#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define Lx 64 // Tamaño de la red en la dirección x
#define Ly 64// Tamaño de la red en la dirección y
#define N_STEPS 100000

double T = 1.0;

// Función para inicializar la red con magnetización inicial nula
void inicializarRedMagnetizacionNula(int lattice[Lx][Ly]) {
    int total_spins = (Lx-2) * Ly;
    int half_spins = total_spins / 2;
    int count_positive = 0, count_negative = 0;

    for (int x = 0; x < Lx; x++) {
        for (int y = 0; y < Ly; y++) {
            if (x == 0) {
                lattice[x][y] = -1; // Borde superior con espines negativos
            } else if (x == Lx - 1) {
                lattice[x][y] = 1; // Borde inferior con espines positivos
            } else {
                if (count_positive < half_spins) {
                    lattice[x][y] = 1;
                    count_positive++;
                } else {
                    lattice[x][y] = -1;
                    count_negative++;
                }
            }
        }
    }

     //Mezclar los espines aleatoriamente
     for (int x = 1; x < Lx - 1; x++) {
        for (int y = 0; y < Ly; y++) {
           int x_rand = 1 + rand() % (Lx - 2);
           int y_rand = rand() % Ly;

            int temp = lattice[x][y];
            lattice[x][y] = lattice[x_rand][y_rand];
            lattice[x_rand][y_rand] = temp;
        }
    }
}

void inicializarRedMitadPositivaMitadNegativa(int lattice[Lx][Ly]) {
    // Asignar los bordes
    for (int y = 0; y < Ly; y++) {
        lattice[0][y] = -1;         // Borde superior
        lattice[Lx - 1][y] = 1;     // Borde inferior
    }

    // Llenar la mitad superior (excluyendo el borde) con 1
    for (int x = 1; x < (Lx - 1) / 2 + 1; x++) {
        for (int y = 0; y < Ly; y++) {
            lattice[x][y] = -1;
        }
    }

    // Llenar la mitad inferior (excluyendo el borde) con -1
    for (int x = (Lx - 1) / 2 + 1; x < Lx - 1; x++) {
        for (int y = 0; y < Ly; y++) {
            lattice[x][y] = 1;
        }
    }


    // Asignar los bordes y el resto de la red
    for (int y = 0; y < Ly; y++) {
                lattice[0][y] = -1; // Borde superior
                lattice[Lx-1][y] = 1; // Borde inferior
    }
}


// Función para inicializar la red con magnetización inicial nula y ordenada
void inicializarRedMagnetizacionNulaOrdenada(int lattice[Lx][Ly]) {
    for (int x = 0; x < Lx; x++) {
        for (int y = 0; y < Ly; y++) {
            if (x == 0) {
                lattice[x][y] = -1; // Borde superior con espines negativos
            } else if (x == Lx - 1) {
                lattice[x][y] = 1; // Borde inferior con espines positivos
            } else {
                // Alternar espines positivos y negativos en un patrón fijo
                if ((x + y) % 2 == 0) {
                    lattice[x][y] = 1; // Espín positivo
                } else {
                    lattice[x][y] = -1; // Espín negativo
                }
            }
        }
    }
}

// Función para imprimir la red en un fichero
void escribirRed(FILE *file, int lattice[Lx][Ly]) {
    for (int x = 0; x < Lx; x++) {
        for (int y = 0; y < Ly; y++) {
            fprintf(file, "%d", lattice[x][y]);
            if (y < Ly - 1) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}

double calcularEnergia(int y1, int x1, int y2, int x2, int lattice[Lx][Ly]) {
    // Vecinos de (y1, x1), excluyendo (y2, x2)
    int derecha1, izquierda1, arriba1, abajo1;
    
    if ((y1 + 1) % Ly == y2 && x1 == x2) {
        derecha1 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        derecha1 = lattice[x1][(y1 + 1) % Ly];
    }

    if ((y1 - 1 + Ly) % Ly == y2 && x1 == x2) {
        izquierda1 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        izquierda1 = lattice[x1][(y1 - 1 + Ly) % Ly];
    }

    if ((x1 - 1) == x2 && y1 == y2) {
        arriba1 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        arriba1 = lattice[x1 - 1][y1];
    }

    if (x1 + 1 == x2 && y1 == y2) {
        abajo1 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        abajo1 = lattice[x1 + 1][y1];
    }

    // Vecinos de (y2, x2), excluyendo (y1, x1)
    int derecha2, izquierda2, arriba2, abajo2;
    if ((y2 + 1) % Ly == y1 && x2 == x1) {
        derecha2 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        derecha2 = lattice[x2][(y2 + 1) % Ly];
    }

    if ((y2 - 1 + Ly) % Ly == y1 && x2 == x1) {
        izquierda2 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        izquierda2 = lattice[x2][(y2 - 1 + Ly) % Ly];
    }

    if (x2 - 1 == x1 && y2 == y1) {
        arriba2 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        arriba2 = lattice[x2 - 1][y2];
    }

    if (x2 + 1 == x1 && y2 == y1) {
        abajo2 = 0; // Excluir el vecino que es el espín intercambiado
    } else {
        abajo2 = lattice[x2 + 1][y2];
    }

    // Energía local sin los términos cruzados
    double energia1 = -lattice[x1][y1] * (derecha1 + izquierda1 + arriba1 + abajo1);
    double energia2 = -lattice[x2][y2] * (derecha2 + izquierda2 + arriba2 + abajo2);

    return energia1 + energia2;
}
// Función para imprimir la red
void imprimirRed(int lattice[Lx][Ly]) {
    for (int x = 0; x < Lx; x++) {
        for (int y = 0; y < Ly; y++) {
            printf("%2d ", lattice[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

// Función para calcular la magnetización promedio en la mitad superior del sistema
double calcularMagnetizacionMitadSuperior(int lattice[Lx][Ly]) {
    int suma = 0;
    for (int x = 0; x < Lx / 2; x++) { // Solo filas de la mitad superior
        for (int y = 0; y < Ly; y++) {
            suma += lattice[x][y];
        }
    }
    return (double) fabs(suma) / ((Lx / 2) * Ly);
}


// Función para calcular la magnetización promedio en la mitad inferior del sistema
double calcularMagnetizacionMitadInferior(int lattice[Lx][Ly]) {
    int suma = 0;
    for (int x = Lx / 2; x < Lx; x++) { // Solo filas de la mitad inferior
        for (int y = 0; y < Ly; y++) {
            suma += lattice[x][y];
        }
    }
    return (double) fabs(suma) / ((Lx / 2) * Ly);
}

// Función para calcular la energía de la configuración
double calcularEnergiaConfiguracion(int lattice[Lx][Ly]) {
    double energia = 0.0;

    for (int x = 1; x < Lx-1; x++) {
        for (int y = 0; y < Ly; y++) {
            // Suma de los vecinos
            int sumaVecinos = lattice[x][y] + lattice[x][y] +
                              lattice[x][(y + 1) % Ly] + lattice[x][(y - 1 + Ly) % Ly];

            // Contribución de la celda (x, y) a la energía
            energia += lattice[x][y] * sumaVecinos;
        }
    }

    // Multiplicar por -1/2 según la fórmula
    return -0.5 * energia;
}


// Función para calcular y mostrar las densidades promedio de espines positivos y negativos en una columna
void calcularDensidadesFila(int lattice[Lx][Ly], int fila, double *densidadpositivo, double *densidadnegativo) {
    int positivos = 0;
    int negativos = 0;
    // Recorrer la columna especificada
    for (int y= 0; y < Ly; y++) {
        if (lattice[(int)fila][y] == 1) {
            positivos++;
        } else if (lattice[(int)fila][y] == -1) {
            negativos++;
        }
    }

    // Calcular las densidades
   *densidadpositivo = (double)positivos;
   *densidadnegativo = (double)negativos;
}

void calcularCalorEspecifico(double sumaEnergia, double sumaEnergiaCuadrada, int conteoEnergia, double *cv) {

      double promedio_E = sumaEnergia/ conteoEnergia;
      double promedio_E2 = sumaEnergiaCuadrada / conteoEnergia;
  
      double varianza_E = promedio_E2 - (promedio_E * promedio_E);
      // Calcular el calor específico a volumen constante
      *cv = varianza_E / (Lx*Lx*T*T);
}


int main() {
    #define MAX_MEDICIONES (N_STEPS / 100 + 1)
    double magnetizaciones[MAX_MEDICIONES];
    int indiceMag = 0;
    int lattice[Lx][Ly];
    srand(time(NULL)); // Semilla para números aleatorios

    // Preguntar al usuario qué tipo de magnetización inicial desea
    int opcion;
    printf("Seleccione la condición inicial:\n");
    printf("1. Magnetización inicial nula (desordenada aleatoriamente)\n");
    printf("2. Magnetización inicial nula (ordenada)\n");
    printf("3. Mitad positivos, mitad negativos (aleatorio)\n");
    printf("Opción: ");
    scanf("%d", &opcion);

    // Inicializar la red según la elección del usuario
    if (opcion == 1) {
    inicializarRedMagnetizacionNula(lattice);
    } else if (opcion == 2) {
    inicializarRedMagnetizacionNulaOrdenada(lattice);
    } else if (opcion == 3) {
    inicializarRedMitadPositivaMitadNegativa(lattice);
    } else {
    printf("Opción no válida. Saliendo del programa.\n");
    return 1;
    }

    // Abrir el fichero para guardar las configuraciones
    FILE *file = fopen("configuraciones.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el fichero.\n");
        return 1;
    }

    FILE *energiaFile = fopen("energia_pmontecarlo.txt", "w");
    if (energiaFile == NULL) {
        printf("Error al abrir el fichero de energía.\n");
        return 1;
    }

    // Abrir los ficheros para guardar las magnetizaciones
    FILE *magnetizacionSuperiorFile = fopen("magnetizacionsuperior.txt", "a");
    if (magnetizacionSuperiorFile == NULL) {
        printf("Error al abrir el fichero de magnetización superior.\n");
        return 1;
    }

    FILE *magnetizacionInferiorFile = fopen("magnetizacioninferior.txt", "a");
    if (magnetizacionInferiorFile == NULL) {
        printf("Error al abrir el fichero de magnetización inferior.\n");
        return 1;
    }

     FILE *filedensidadpositivo = fopen("densidadpositivo.txt", "a");
    if (magnetizacionInferiorFile == NULL) {
        return 1;
    }
    FILE *filedensidadnegativo = fopen("densidadnegativo.txt", "a");
    if (magnetizacionInferiorFile == NULL) {
        return 1;
    }
    FILE *filecv = fopen("filecv.txt", "a");
    if (magnetizacionInferiorFile == NULL) {
        return 1;
    }

    // Imprimir la configuración inicial
    printf("Configuración inicial de la red:\n");
    imprimirRed(lattice);

     // Escribir la configuración inicial en el fichero
     escribirRed(file, lattice);

      // Variables para acumular magnetización
    double sumaMagnetizacionSuperior = 0.0;
    double sumaMagnetizacionInferior = 0.0;
    int configuracionesCambiadas = 0;
    double sumaEnergia = 0.0;
    double sumaEnergiacuadrada = 0.0;
    int conteoEnergia = 0;

    double sumadensidadpositivo = 0.0;
    double sumadensidadnegativo = 0.0;
    double densidadpositivo = 0.0;
    double densidadnegativo = 0.0;
    int fila = Lx / 4;
    
    double energias[N_STEPS];
    double energia_actual;
    double cv;

    // Algoritmo de Metropolis con dinámica de Kawasaki
    bool convergenciaAlcanzada = false;

 

    //for (int step = 0; step < N_STEPS && !convergenciaAlcanzada; step++) {
    for (int step = 0; step < N_STEPS; step++) {
        for (int j = 0; j < Lx * Lx; j++) {
         // Escoger un espín al azar (excluyendo los bordes superior e inferior)
         int y1 = rand() % Ly;
         int x1 = 1 + rand() % (Lx - 2); //valores entre 1 y Lx-2


            // Modificación para restringir movimientos en los bordes internos
            int dy[] = {-1, 1, 0, 0}; // Desplazamientos en x: izquierda, derecha, sin cambio, sin cambio
            int dx[] = {0, 0, -1, 1}; // Desplazamientos en y: sin cambio, sin cambio, arriba, abajo

         int dir;
         if (x1 == 1) {
           // Si estamos en la fila x1 = 1, solo permitimos izquierda, derecha o abajo
           int opciones[] = {0, 1, 3}; // Índices de las direcciones permitidas
           dir = opciones[rand() % 3];
         } else if (x1 == Lx - 2) {
            // Si estamos en la fila x1 = Lx - 2, solo permitimos izquierda, derecha o arriba
            int opciones[] = {0, 1, 2}; // Índices de las direcciones permitidas
           dir = opciones[rand() % 3];
         } else {
            // Para el resto de las filas, permitimos todas las direcciones
            dir = rand() % 4;
         }

            // Calcular las coordenadas del vecino. Ya consideramos condiciones periódicas en y
         int y2 = (y1 + dy[dir] + Ly) % Ly;
         int x2 = x1 + dx[dir];


         // Asegurarse de que los espines sean diferentes para intercambiar
         if (lattice[x1][y1] != lattice[x2][y2]) {

            double energiaAntes = calcularEnergia(y1, x1, y2, x2, lattice);

            // Intercambiar los espines
            int temp = lattice[x1][y1];
            lattice[x1][y1] = lattice[x2][y2];
            lattice[x2][y2] = temp;

            // Calcular la energía después del intercambio
            double energiaDespues = calcularEnergia(y1, x1, y2, x2, lattice);

            // Calcular la diferencia de energía
            double dE = energiaDespues - energiaAntes;

            // Calcular la probabilidad de transición
            double probabilidad;
            if (dE > 0) {
                probabilidad = exp(-dE / T);
            } else {
                probabilidad = 1.0;
            }

            // Generar un número aleatorio entre 0 y 1
            double r = (double)rand() / RAND_MAX;

            
            // Verificar la convergencia solo si se acepta el cambio en N pasos montecarlo
            if (j == Lx * Lx - 1) {
                energia_actual = calcularEnergiaConfiguracion(lattice);

                // Guardar la energía en el array
                energias[step] = energia_actual;

                // Comparar la energía del paso montecarlo actual con la del paso i + 3000
                if (step >= 3000) {
                    double energiaComparar = energias[step - 3000];
                    if (fabs(energia_actual - energiaComparar) < 4) {
                       // printf("Convergencia alcanzada entre los pasos %d y %d.\n", step - 3000 + 1, step + 1);
                        //convergenciaAlcanzada = true;
                       // break;
                    }
                }

                // Guardar la energía y la iteración en el fichero
                fprintf(energiaFile, "%.6f %d\n", energia_actual, step + 1);
            }
        
         if(r > probabilidad) { 
                // Revertir el intercambio si no se acepta
                temp = lattice[x1][y1];
                lattice[x1][y1] = lattice[x2][y2];
                lattice[x2][y2] = temp;
            }
        }
    }
        escribirRed(file, lattice);
        
            // Calcular la magnetización y energía cada 2000 pasos montecarlo
        if ((step + 1) % (100) == 0 && step > 2000) {
            double magnetizacionSuperior = calcularMagnetizacionMitadSuperior(lattice);
            double magnetizacionInferior = calcularMagnetizacionMitadInferior(lattice);

            sumaMagnetizacionSuperior += magnetizacionSuperior;
            sumaMagnetizacionInferior += magnetizacionInferior;
            configuracionesCambiadas++;

          
            magnetizaciones[indiceMag++] = magnetizacionSuperior;
            calcularDensidadesFila(lattice, fila, &densidadpositivo, &densidadnegativo);
            sumadensidadpositivo += densidadpositivo;
            sumadensidadnegativo += densidadnegativo;
            
            double energiaConfiguracion = calcularEnergiaConfiguracion(lattice);
            sumaEnergia += energiaConfiguracion;
            sumaEnergiacuadrada += energiaConfiguracion * energiaConfiguracion;
            conteoEnergia++;
         }
        
         
    }
    fclose(energiaFile); 
 
    double promedioMagnetizacionSuperior = sumaMagnetizacionSuperior / configuracionesCambiadas;
    double promedioMagnetizacionInferior = sumaMagnetizacionInferior / configuracionesCambiadas;
    double promedioDensidadPositiva = sumadensidadpositivo / (configuracionesCambiadas*Ly);
    double promedioDensidadNegativa = sumadensidadnegativo / (configuracionesCambiadas*Ly);


    fprintf(magnetizacionSuperiorFile, "%.6f %.1f\n", promedioMagnetizacionSuperior, T);
    fprintf(magnetizacionInferiorFile, "%.6f %.1f\n", promedioMagnetizacionInferior, T);
    

    fprintf(filedensidadnegativo, "%.6f %.1f\n", promedioDensidadNegativa, T);
    fprintf(filedensidadpositivo, "%.6f %.1f\n", promedioDensidadPositiva, T);
    
     calcularCalorEspecifico(sumaEnergia, sumaEnergiacuadrada, conteoEnergia, &cv);
    fprintf(filecv, "%.6f %.1f\n", cv, T);

        
    // Calcular y mostrar la energía media por partícula
    if (conteoEnergia > 0) {
     double energiaMediaPorParticula = sumaEnergia / (conteoEnergia * 2 * Lx * Ly);
     printf("Energía media por partícula: %.6f\n", energiaMediaPorParticula);
    } 
    

   FILE *magnetizacionesFile = fopen("magnetizaciones_vs_tiempo.txt", "w");
    if (magnetizacionesFile != NULL) {
    for (int i = 0; i <  indiceMag; i++) {
        fprintf(magnetizacionesFile, "%.6f\n", magnetizaciones[i]);
    }
    fclose(magnetizacionesFile);
    } else {
    printf("Error al abrir el fichero de magnetizaciones vs tiempo.\n");
    }

     fclose(file);

    fclose(magnetizacionSuperiorFile);
    fclose(magnetizacionInferiorFile);
    fclose(filedensidadpositivo);
    fclose(filedensidadnegativo);
    fclose(filecv);

     return 0;
}

