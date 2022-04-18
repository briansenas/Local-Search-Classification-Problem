/**
 * @file mytools.h
 * @version 2.3
 * @date 05/04/2022
 * @author Brian Sena Simons 3ºA-A2
 * @brief Herramientas definidas para la práctica.
 * @code
 * int main(){
 *  MatrixXd mat(3,2);
 *  mat << 1, 1,
 *         2, 2,
 *         4, 4;
 *
 *  RowVectorXd fil = removeRow(mat,2);
 *  MatriXd::Index pos;
 *  double min = minEuclideanDistance(fil,mat,pos);
 *  cout << "Mínimo vecino por fila en: " << pos << " con valor: " << min << endl;
 * }
 * @endcode
 **/
#ifndef MYTOOLS_H
#define MYTOOLS_H

#include "./eigen-3.4.0/Eigen/Dense"
#include "./random.hpp"
#include <string.h>
#include <vector>

using namespace std;
using namespace Eigen;
using namespace std::chrono;
using Random = effolkronium::random_static;

/**
 * @brief fillRange rellena un vector de valores desde 0 hasta upperlimit;
 * @param toFill es el vector a rellenar
 * @param upperlimit es el valor tope del vector
 **/
void fillRange(vector<int>& toFill,unsigned int upperlimit);

/**
 * @brief minEuclideanDistance utiliza la librería Eigen para calcular el
 * menor vecino por fila desde una fila "fixed" NO contenida en data.
 * @param fixed Fila a buscar NO contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 **/
double minEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,Eigen::MatrixXd::Index &maxRow);

/**
 * @brief minEuclideanDistance utiliza la librería Eigen para calcular el
 * menor vecino por fila desde una fila "fixed" NO contenida en data multiplicando
 * las distancias por el vector de características que las pondera.
 * @param weights es el vector de pesos característicos a multiplicar
 * @param fixed Fila a buscar NO contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 * @see minEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,Eigen::MatrixXd::Index &maxRow);
 **/
double minEuclideanDistance(Eigen::RowVectorXd weigths,Eigen::RowVectorXd fixed, Eigen::MatrixXd data,Eigen::MatrixXd::Index &maxRow);

/**
 * @brief ManualEuclideanDistance no-utiliza completamente la librería Eigen
 * para calcular el menor vecino por fila, ya que ahora permitimos que fixed,
 * esté contenido en data, y lo que hacemos es elegir el segundo mínimo manualmente.
 * @param weights es el vector de pesos característicos a multiplicar
 * @param fixed Fila a buscar contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param pos Fila de la que provee fixed.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 * @see minEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,Eigen::MatrixXd::Index &maxRow);
 **/
double ManualEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,unsigned int pos,unsigned int &maxRow);

/**
 * @brief ManualEuclideanDistance no-utiliza completamente la librería Eigen
 * para calcular el menor vecino por fila, ya que ahora permitimos que fixed,
 * esté contenido en data, y lo que hacemos es elegir el segundo mínimo ponderado
 * por el vector de características manualmente.
 * @param weights es el vector de pesos característicos a multiplicar
 * @param fixed Fila a buscar contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param pos Fila de la que provee fixed.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 * @see double ManualEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,unsigned int pos,unsigned int &maxRow);
 **/
double ManualEuclideanDistance(Eigen::RowVectorXd weigths,Eigen::RowVectorXd fixed, Eigen::MatrixXd data,unsigned int pos, unsigned int &maxRow);

/**
 * @brief ManualEuclideanDistanceType no-utiliza completamente la librería Eigen
 * para calcular el menor vecino por fila, ya que ahora permitimos que fixed,
 * esté contenido en data, y lo que hacemos es elegir el segundo mínimo
 * perteneciente a una clase específica manualmente.
 * @param weights es el vector de pesos característicos a multiplicar
 * @param fixed Fila a buscar contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param label Vector de etiquetas.
 * @param type Clase a buscar.
 * @param pos Fila de la que provee fixed.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 * @see double ManualEuclideanDistance(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,unsigned int pos,unsigned int &maxRow);
 **/
double ManualEuclideanDistanceType(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,vector<char>label, char type, unsigned int pos, unsigned int &maxRow);

/**
 * @brief ManualEuclideanDistanceType no-utiliza completamente la librería Eigen
 * para calcular el menor vecino por fila, ya que ahora permitimos que fixed,
 * esté contenido en data, y lo que hacemos es elegir el segundo mínimo ponderado
 * por el vector de características y perteneciente a una clase específica manualmente.
 * @param weights es el vector de pesos característicos a multiplicar
 * @param fixed Fila a buscar contenida en data.
 * @param data Matrix de datos a verificar vecinos.
 * @param label Vector de etiquetas.
 * @param type Clase a buscar.
 * @param pos Fila de la que provee fixed.
 * @param maxRow es el ínidice de la fila más cercana encontrada.
 * @return el valor mínimo.
 * @see double ManualEuclideanDistanceType(Eigen::RowVectorXd fixed, Eigen::MatrixXd data,vector<char>label, char type, unsigned int pos, unsigned int &maxRow);
 **/
double ManualEuclideanDistanceType(Eigen::RowVectorXd weigths,Eigen::RowVectorXd fixed, Eigen::MatrixXd data,vector<char>label,char type,unsigned int pos, unsigned int &maxRow);

/**
 * @brief getClass lo que nos devuelve es una matrix con los datos pertenecientes
 * a una clase específica.
 * @param data Matrix de datos completos.
 * @param label Vector de etiquetas.
 * @param type Clase específica a buscar.
 * @return Matrix con los datos de esa clase
 **/
Eigen::MatrixXd getClass(Eigen::MatrixXd data, vector<char> label, char type);

Eigen::MatrixXd getClassLabelled(Eigen::MatrixXd data, vector<char> Label, vector<char>& newLabel, char type);

/**
 * @brief getFold divide la matrix de datos "data" en dos matrices "tranning" y
 * "test" con los datos pertenecientes a uno de los 5-folds que tenemos de
 * desplazar un 20% al grupo de test sobre el conjunto total
 * @param data Matrix de datos completos.
 * @param label Vector de etiquetas.
 * @param traning Matrix con los datos para entrenar.
 * @param Tlabel Vector con las etiquetas de entreno.
 * @param test Matrix con los datos para el test.
 * @param Ttlabel Vector con las etiquetas de test.
 * @param num Fold a utilizar (0:(0-80%;80%-100%), 1:(0:60%+80%-100%;60%-80%)...)
 **/
void getFold(Eigen::MatrixXd data, vector<char> Label, Eigen::MatrixXd &training, vector<char> &TLabel,
        Eigen::MatrixXd &test, vector<char>& TtLabel, unsigned int num=1);

/**
 * @brief getFoldbyLoop divide la matrix de datos "data" en dos matrices "tranning" y
 * "test" con los datos pertenecientes a uno de los 5-folds que tenemos de
 * desplazar un 20% al grupo de test sobre el conjunto total. A diferencia de
 * getFold() intenta utilizar el "for-loop" de las etiquetas para asignar las filas.
 * Sin embargo posee peor rendimiento.
 * @param data Matrix de datos completos.
 * @param label Vector de etiquetas.
 * @param traning Matrix con los datos para entrenar.
 * @param Tlabel Vector con las etiquetas de entreno.
 * @param test Matrix con los datos para el test.
 * @param Ttlabel Vector con las etiquetas de test.
 * @param num Fold a utilizar (0:(0-80%;80%-100%), 1:(0:60%+80%-100%;60%-80%)...)
 * @see getFold()
 **/
void getFoldbyLoop(Eigen::MatrixXd data, vector<char> Label, Eigen::MatrixXd &training, vector<char> &TLabel,
        Eigen::MatrixXd &test, vector<char>& TtLabel, unsigned int num);

/*
 * La idea es obtener una distribución balanceada de datos para el
 * conjunto de entreno / test.
 * @param group1 Grupo con características de clase 1;
 * @param label1 Vector con las etiquetas de clase 1;
 * @param group2 Grupo con características de clase 2;
 * @param label2 Vector con las etiquetas de clase 2;
 * @param training Matrix resultante para el conjunto de entreno.
 * @param Tlabel Vector con las etiquetas resultante para el entreno
 * @param test Matrix resultante con el conjunto de test,
 * @param Ttlabel Vector con las etiquetas resultantes para el test.
 * @param num Número de correspondencia de fold.
 * @param seed Semilla para el barajado final
 *@see getFold();
 **/
void getBalancedFold(Eigen::MatrixXd group1, vector<char> label1,Eigen::MatrixXd group2,
        vector<char> label2, Eigen::MatrixXd &training, vector<char> &TLabel,
        Eigen::MatrixXd &test, vector<char>& TtLabel, unsigned int num, long int seed);

/**
 * @brief shuffleData utiliza el archivo "random.hpp" y la librería Eigen
 * para crear un vector de índices, al cual barajamos y luego utilizamos ese
 * vector para intercambiar posiciones. Dónde el número que aparezca en la posición
 * 0 es el intercambio de 0 con ese número y así.
 * @param mat Matrix a permutar filas.
 * @parm label Vector de etiquetas a intercambiar valores.
 * @param seed Semilla para la función Random
 **/
void shuffleData(MatrixXd& mat,vector<char>& label,long int seed);

/**
 * @brief readValues es la función utilizada para leer los archivos ".arrf" de
 * la práctia en una Matrix de la libreria "Eigen" y un vector de etiquetas "Label".
 * @param filename Nombre del archivo que vamos a leer
 * @param label Vector de etiquetas a rellenar.
 * @return Matrix con todos los valores.
 **/
Eigen::MatrixXd readValues(string filename, vector<char>& label);

/**
 * @brief removeRow nos permite quitar una fila de una matrix de entrada (sin
 * modificar la original) y devuelve la matrix resultante modificada.
 * @param matrix Matrix con los datos completos.
 * @param rowToRemove Fila a quitar.
 * @return Matrix modificada resultante
 **/
Eigen::MatrixXd removeRow(Eigen::MatrixXd matrix, unsigned int rowToRemove);

/**
 * @brief removeRow nos permite quitar una columna de una matrix de entrada (sin
 * modificar la original) y devuelve la matrix resultante modificada.
 * @param matrix Matrix con los datos completos.
 * @param colToRemove Columna a quitar.
 * @return Matrix modificada resultante
 **/
Eigen::MatrixXd removeCol(Eigen::MatrixXd matrix, unsigned int colToRemove);

#endif
