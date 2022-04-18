# ** _Brian Sena Simons 3ºA - A2_ **
# Práctica 1 - MetaHeurística

### - Utilize el "cmake CMakeLists.txt && make" para compilar;
### - Los algoritmos pueden ser ejecutados utilizando los scripts en ./scripts {Ej. runAll.sh}
### - Los resultados se guardan en ./results;

## Ejecución individual:
./bin/LSalg ./datos/ionosphere.arff {seed} [0-1] {>../results/LSalg\_results.txt}

./bin/FastGreedy ./datos/ionosphere.arff {clase 1} {clase 2} {seed} [0-2]{>../results/Greedy\_results.txt}

./bin/Fast1NN ./datos/ionosphere.arff {clase 1} {clase 2}{seed} [0-2]{>../results/1NN\_results.txt}

Pd: {0=Sin Barajar}; {1=Barajando}; {2=Equlibrado de Clases};

## Descripción breve del Problema
La idea es comparar distintos tipos de algoritmos para clasificar datos pertenecientes
a una base de datos públicas que nos provee el profesorado. Partiremos primero
de la implementación del típico algoritmo de clasficación K-NN dónde K representa
el número de vecinos a mirar y la idea conceptual es buscar los K vecinos más
cercanos para realizar una predicción sobre que clase pertenece el objeto a predecir.

Una vez implementado el algoritmo 1NN intentaremos mejorar el porcentaje de aciertos
utilizando técnicas de ponderación de características mediante un vector de pesos.
El grueso de la práctica está en el cálculo de esos pesos. Utilizaremos un algoritmo
"Greedy" denominado "RELIEF" que utiliza la distancia hacia los amigos (vecinos de la
misma clase más cercanos) y enemigos (vecinos de distinta clase más cercano) para
incrementar los valores de los pesos para todas las filas pertenecientes al conjunto
de entreno. Luego normaliza.

Otra técnica será la búsqueda local utilizando una distribución de varianza 0.3
para modificar pesos  distintos sin repetición hasta que haya alguna mejora con
un total de máximo 15000 evaluaciones o 20\*n siendo n el número de características.

Para comprobar los algoritmos realizaremos un _cross-validation_ de 5-folds
dónde desplazaremos el conjunto de test (correspondiente al 20%) des final hasta
el principio de 20% en 20%.

Para la práctica he tenido que definir las funciones en mytools.h y mytools.cpp;
