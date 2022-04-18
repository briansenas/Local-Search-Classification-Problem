#include "../tools/eigen-3.4.0/Eigen/Dense"
#include <iostream>
#include "../tools/mytools.h"
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>

using namespace std;
using namespace Eigen;
using namespace std::chrono;

int main(int argc, char** argv){
    if(argc<=4){
        cerr << "[ERROR] Couldn't resolve file name;" << endl;
        cerr << "[SOLVE] Exec: ./main {filename} {class a} {class b} {seed}" << endl;
        exit(-1);
    }

    /// Leemos los datos de entrada como es el archivo, clase1, clase2 y la semilla.
    string filename = argv[1];
    char type1 = *argv[2];
    char type2 = *argv[3];
    long int seed = atoi(argv[4]);
    Random::seed(seed);
    vector<char> label;
    MatrixXd allData = readValues(filename,label);
    //shuffleData(allData,label,seed);

    /// Inicializamos todas las variables que vamos a necesitar para almacenar información
    MatrixXd data, test, group1, group2, temp;
    vector<char> Tlabel, Ttlabel;
    vector<float> Success, Reduction, Time;
    float right = 0, max_w = -1, tiempo2;
    milliseconds tiempo;
    char mytype;unsigned int i, size;
    MatrixXd::Index friendly, enemy;
    RowVectorXd toTest, Weights, dist1, dist2;
    high_resolution_clock::time_point momentoInicio, momentoFin;
    bool WhatFriend;

    /// Realizamos un total de 5 iteraciones obteniendo un fold distinto cada vez.
    for(int x=0,max=5;x<max;x++){
        momentoInicio = high_resolution_clock::now();
        getFold(allData,label,data,Tlabel,test,Ttlabel,x);
        /// Inicializamos los pesos a 0.
        Weights = RowVectorXd::Constant(data.cols(),0);
        /// Para cada fila del conjunto de entrenamiento, hacemos:
        for(i=0,size=data.rows();i<size;i++){
            mytype = Tlabel.at(i);
            /// Quitamos esa fila de la matrix inicial
            temp = removeRow(data,i);
            Tlabel.erase(Tlabel.begin()+i);
            /// Obtenemos las matrices con apenas un tipo de clase sin la fila en concreto.
            group1 = getClass(temp, Tlabel, type1);
            group2 = getClass(temp, Tlabel, type2);
            /// Calculamos las distancias de su enemigo y amigo más cercano.
            if(mytype == type1){
                minEuclideanDistance(data.row(i),group1,friendly);
                minEuclideanDistance(data.row(i),group2,enemy);
                dist1 = data.row(i).array() - group1.row(friendly).array();
                dist2 = data.row(i).array() - group2.row(enemy).array();
            }else{
                minEuclideanDistance(data.row(i),group2,friendly);
                minEuclideanDistance(data.row(i),group1,enemy);
                dist1 = data.row(i).array() - group2.row(friendly).array();
                dist2 = data.row(i).array() - group1.row(enemy).array();
            }
            /// Actualizamos el peso con los valores calculados anteriormente.
            Weights.array() += dist2.array().abs() - dist1.array().abs();
            Tlabel.insert(Tlabel.begin()+i,mytype);
        }
        max_w = Weights.maxCoeff();
        right = 0;
        /// Procedemos a realizar la normalización de los pesos.
        for(i=0,size=Weights.size();i<size;i++){
            if(*(Weights.data() + i) < 0.1){
                *(Weights.data() + i) = 0;
                right++;
            }
            else
                *(Weights.data() + i) = *(Weights.data() + i) / max_w;
        }
        Reduction.push_back(right);

        right = 0;
        /// Verificamos en el conjunto de test la cantidad de aciertos encontrados.
        for(i=0,size=test.rows();i<size;i++){
            minEuclideanDistance(Weights,test.row(i),data,friendly);
            if(Ttlabel.at(i) == Tlabel.at(friendly))
                right++;
        }
        Success.push_back(right/test.rows());

        momentoFin = high_resolution_clock::now();

        /// Calculamos los tiempos obtenidos para 1 iteración completa.
        tiempo = duration_cast<milliseconds>(momentoFin - momentoInicio);
        Time.push_back(tiempo.count());
    }

    /// Imprimimos los valores obtenidos en pantalla.
    cout << "FOLDING NUMBER\t" << "SUCCESS RATE\t"  << "FAIL RATE\t" << "REDUCTION RATE\t\t" << "FUNCTION\t" << "TIME\t" << endl;
    float avg_success = 0,avg_fail =0, avg_red = 0, avg_fun = 0, avg_time = 0,alpha=0.5,function=0;
    for(i=0,size=Success.size();i<size;i++){
        Reduction[i] /= data.cols();
        function = (Success[i]*alpha + Reduction[i]*(1-alpha));
        cout << "FOLDING - " << i << "\t" << Success[i] << "\t" << 1-Success[i] << "\t" << Reduction[i] << "\t\t" << function << "\t" << Time[i]  << endl;
        avg_success += Success[i];
        avg_fail += 1-Success[i];
        avg_red += Reduction[i];
        avg_fun += function;
        avg_time += Time[i];
    }

    avg_success /= size;
    avg_fail /= size;
    avg_red /= size;
    avg_fun /= size;
    cout << "TOTAL ->\t" << avg_success << "\t" << avg_fail << "\t" << avg_red << "\t\t" << avg_fun << "\t" << avg_time;
    avg_time /= size;
    cout << "(" << avg_time << ")" << endl;

    return 0;
}
