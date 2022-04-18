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
    if(argc<=5){
        cerr << "[ERROR] Couldn't resolve file name;" << endl;
        cerr << "[SOLVE] Exec: ./main {filename} {class a} {class b} {seed} {No-Shuffle-Balance}[0-2]" << endl;
        exit(-1);
    }

    /// Leemos los datos de entrada como es el archivo, clase1, clase2 y la semilla.
    string filename = argv[1];
    char type1 = *argv[2];
    char type2 = *argv[3];
    long int seed = atoi(argv[4]);
    int shuffle = atoi(argv[5]);
    Random::seed(seed);
    vector<char> label;
    MatrixXd allData = readValues(filename,label);

    /// Inicializamos todas las variables que vamos a necesitar para almacenar información
    MatrixXd data, test, group1, group2;
    vector<char> Tlabel, Ttlabel, label_group1, label_group2;
    vector<float> Success, Reduction, Time;
    float right = 0, max_w = -1;
    milliseconds tiempo;
    char mytype;unsigned int i, size,friendly,enemy;
    MatrixXd::Index Neighbour;

    RowVectorXd toTest, Weights, dist1, dist2;
    high_resolution_clock::time_point momentoInicio, momentoFin;

    if(shuffle==1){
        cout << "[WARNING]: Data has been shuffled; " << endl;
        shuffleData(allData,label,seed);
    }
    if(shuffle==2){
        cout << "[WARNING]: Data has been balanced and shuffled (inevitably); " << endl;
        group1 = getClassLabelled(allData,label, label_group1, type1);
        group2 = getClassLabelled(allData,label, label_group2, type2);
    }

    /// Realizamos un total de 5 iteraciones obteniendo un fold distinto cada vez.
    for(int x=0,max=5;x<max;x++){
        momentoInicio = high_resolution_clock::now();
        if(shuffle!=2)
            getFold(allData,label,data,Tlabel,test,Ttlabel,x);
        else
            getBalancedFold(group1,label_group1,group2,label_group2,data,Tlabel, test, Ttlabel,x,seed);
        /// Inicializamos los pesos a 0.
        Weights = RowVectorXd::Constant(data.cols(),0);
        /// Para cada fila del conjunto de entrenamiento hacemos:
        for(i=0,size=data.rows();i<size;i++){
            /// Verificamos a que clase corresponde la fila.
            mytype = Tlabel.at(i);
            if(mytype == type1){
                /// Calculamos los índices a las filas amigas/enemigas más cercanas
                ManualEuclideanDistanceType(data.row(i),data,Tlabel,type1,i,friendly);
                ManualEuclideanDistanceType(data.row(i),data,Tlabel,type2,i,enemy);
            }else{
                ManualEuclideanDistanceType(data.row(i),data,Tlabel,type2,i,friendly);
                ManualEuclideanDistanceType(data.row(i),data,Tlabel,type1,i,enemy);
            }
            dist1 = data.row(i).array() - data.row(friendly).array();
            dist2 = data.row(i).array() - data.row(enemy).array();
            /// Alteramos los pesos acorde con las distancias obtenidas.
            Weights.array() += dist2.array().abs() - dist1.array().abs();
        }
        max_w = Weights.maxCoeff();
        right = 0;
        /// Procedemos a normalizar los pesos.
        for(i=0,size=Weights.cols();i<size;i++){
            if(*(Weights.data() + i) < 0.1){
                *(Weights.data() + i) = 0;
                right++;
            }
            else *(Weights.data() + i) = *(Weights.data() + i) / max_w;
        }
        Reduction.push_back(right);

        right = 0;
        /// Procedemos a verificar el porcentaje de aciertos con los valores obtenidos.
        for(i=0,size=test.rows();i<size;i++){
            minEuclideanDistance(Weights,test.row(i),data,Neighbour);
            if(Ttlabel.at(i) == Tlabel.at(Neighbour))
                right++;
        }
        Success.push_back(right/test.rows());

        momentoFin = high_resolution_clock::now();

        /// Calculamos los tiempos obtenidos para una iteración
        tiempo = duration_cast<milliseconds>(momentoFin - momentoInicio);
        Time.push_back(tiempo.count());
    }

    /// Imprimimos los valores por pantalla.
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

    unsigned int z;
    for(z = 0, size=Weights.cols(); z < size; z++)
        cout << *(Weights.data()+z) << ",";
    cout << endl;

    return 0;
}
