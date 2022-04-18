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
    if(argc<3){
        cerr << "[ERROR] Couldn't resolve file name;" << endl;
        cerr << "[SOLVE] Exec: ./main {filename} {seed} {shuffle}[0-1]" << endl;
        exit(-1);
    }

    /// Leemos los datos de entrada como es el nombre del archivo y la semilla.
    string filename = argv[1];
    long int seed = atoi(argv[2]);
    int shuffle = atoi(argv[3]);

    Random::seed(seed);
    vector<char> label;
    MatrixXd allData = readValues(filename,label);
    //shuffleData(allData,label,seed);

    /// Inicializamos todas las variables que vamos a necesitar para almacenar información
    vector<float> Success, Failure, Time;
    float right = 0;
    milliseconds tiempo;
    unsigned int i, size,friendly,max=5;
    high_resolution_clock::time_point momentoInicio, momentoFin;
    if(shuffle==1){
        cout << "[WARNING]: Data will be shuffled;" << endl;
    }else{
        max = 1;
    }
    /// Realizamos un total de 5 iteraciones barajando los valores de la matrix inicial
    for(unsigned int x=0;x<max;x++){
        if(shuffle==1)
            shuffleData(allData,label,seed);
        momentoInicio = high_resolution_clock::now();

        right = 0;
        /// Para toda fila, calculamos su vecino más cercano como predición de su clase.
        for(i=0,size=allData.rows();i<size;i++){
            ManualEuclideanDistance(allData.row(i),allData,i,friendly);
            if(label[i] == label[friendly])
                right++;
        }
        /// Guardamos la cantidad de aciertos.
        Success.push_back(right/allData.rows());

        momentoFin = high_resolution_clock::now();

        /// Calculamos los tiempos con dos librerías distintas.
        tiempo = duration_cast<milliseconds>(momentoFin - momentoInicio);
        Time.push_back(tiempo.count());
    }


    /// Imprimimos los valores por pantalla.
    cout << "ITERATION\t" << "SUCCESS RATE\t"  << "FAIL RATE\t" << "TIME\t" << endl;
    float avg_success = 0,avg_fail =0, avg_time = 0;
    for(i=0,size=Success.size();i<size;i++){
        cout << "NUM - " << i << "\t" << Success[i] << "\t" << 1-Success[i] << "\t" << Time[i]  << endl;
        avg_success += Success[i];
        avg_fail += 1-Success[i];
        avg_time += Time[i];
    }

    avg_success /= size;
    avg_fail /= size;
    cout << "TOTAL ->\t" << avg_success << "\t" << avg_fail << "\t" << avg_time;
    avg_time /= size;
    cout << "(" << avg_time << ")" << endl;

    return 0;
}
