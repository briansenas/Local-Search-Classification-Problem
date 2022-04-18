#include "../tools/eigen-3.4.0/Eigen/Dense"
// From https://github.com/effolkronium/random
#include "../tools/random.hpp"

#include "../tools/mytools.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>

using namespace std;
using namespace Eigen;
using namespace std::chrono;
using Random = effolkronium::random_static;

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
    vector<char> label;
    MatrixXd allData = readValues(filename,label);

    /// Inicializamos todas las variables que vamos a necesitar para almacenar información
    MatrixXd data, test, group1, group2;
    vector<char> Tlabel, Ttlabel, label_group1, label_group2;
    vector<double> Success, Reduction, Time;
    vector<int> indexGrid, Evaluations;
    fillRange(indexGrid,allData.cols());
    double right = 0, reduct = 0, function_before, function_after,alpha =0.5;
    std::normal_distribution<double> distribution(0.0, sqrt(0.3));
    milliseconds tiempo;
    unsigned int i, size, ran_num, tilBetter, maxTilBetter = 20*allData.cols(), eval_num=0,max_eval=15000;
    MatrixXd::Index Neighbour;
    unsigned int ManualNeighbour;
    RowVectorXd  Weights_before, Weights, dist1, dist2;
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

    for(int x=0,max=5;x<max;x++){
        momentoInicio = high_resolution_clock::now();
        if(shuffle!=2)
            getFold(allData,label,data,Tlabel,test,Ttlabel,x);
        else
            getBalancedFold(group1,label_group1,group2,label_group2,data,Tlabel, test, Ttlabel,x,seed);
        /// Inicializamos los pesos a un valor aleatorio entre 0 y 1.
        Weights = (RowVectorXd::Random(data.cols())+RowVectorXd::Constant(data.cols(),1))/2.0;
        Weights_before = Weights;
        tilBetter = 0;
        function_after = function_before = 0;
        /// Barajamos nuestro vector de índices a los pesos a modificar.
        Random::shuffle(indexGrid);
        ran_num = eval_num = 0;
        while(eval_num < max_eval and tilBetter<maxTilBetter ){
            /// Modificamos el indice barajado correspondiente para modificar los pesos.
            Weights[indexGrid[ran_num++]] += Random::get(distribution);
            /// Si hemos modificados volvemos a barajar.
            if(ran_num>=indexGrid.size()){
                Random::shuffle(indexGrid);
                ran_num = 0;
            }

            reduct = 0;
            /// Truncamos los pesos a 0 y 1; Contamos las reducciones a 0.
            for(i=0,size=Weights.cols();i<size;i++){
                if(*(Weights.data() + i) < 0.1){
                    *(Weights.data() + i) = 0;
                    reduct++;
                }
                else if(*(Weights.data()+i)>1)
                    *(Weights.data()+i) = 1;
            }

            right = 0;
            /// Verificamos el porcentaje de acierto obtenido con la modificación.
            for(i=0,size=data.rows();i<size;i++){
                ManualEuclideanDistance(Weights,data.row(i),data,i, ManualNeighbour);
                if(Tlabel[i] == Tlabel[ManualNeighbour])
                    right++;
            }

            /// Evaluamos la función.
            function_after = alpha*(right/data.rows()) + (1-alpha)*(reduct/Weights.cols());
            eval_num++;

            /// Verificamos si hemos mejorado, actuamos acorde.
            if(function_after <= function_before){
                tilBetter++;
                Weights = Weights_before;
            }else{
                tilBetter = 0;
                Weights_before = Weights;
                function_before = function_after;
                Random::shuffle(indexGrid);
                ran_num = 0;
            }
        } // END WHILE
        right = 0;
        /// Calculamos el porcentaje de aciertos final tras obtener nuestro conjunto óptimo de pesos.
        for(i=0,size=test.rows();i<size;i++){
            minEuclideanDistance(Weights,test.row(i),data,Neighbour);
            if(Ttlabel[i] == Tlabel[Neighbour])
                right++;
        }

        momentoFin = high_resolution_clock::now();
        /// Almacenamos los valores obtenidos.
        Success.push_back(right/test.rows());
        Reduction.push_back(reduct/Weights.cols());
        Evaluations.push_back(eval_num);

        /// Calculamos los tiempos obtenidos la iteración completa.
        tiempo = duration_cast<milliseconds>(momentoFin - momentoInicio);
        Time.push_back(tiempo.count());
    }

    /// Imprimimos los valores por pantalla.
    cout << "FOLDING NUMBER\t" << "SUCCESS RATE\t"  << "FAIL RATE\t" << "REDUCTION RATE\t\t"
        << "FUNCTION\t" << "EVALUATION\t" << "TIME\t" << endl;
    double avg_success = 0,avg_fail =0, avg_red = 0, avg_fun = 0, avg_time = 0, avg_iter = 0, function=0;
    for(i=0,size=Success.size();i<size;i++){
        function = (Success[i]*alpha + Reduction[i]*(1-alpha));
        avg_iter += Evaluations[i];
        cout << "FOLDING - " << i << "\t" << Success[i] << "\t" << 1-Success[i] << "\t" << Reduction[i] << "\t\t"
            << function << "\t\t" << Evaluations[i] << "\t\t" << Time[i]  << endl;
        avg_success += Success[i];
        avg_fail += (1-Success[i]);
        avg_red += Reduction[i];
        avg_fun += function;
        avg_time += Time[i];
    }

    avg_success /= size;
    avg_fail /= size;
    avg_red /= size;
    avg_fun /= size;
    avg_iter /= size;
    cout << "TOTAL ->\t" << avg_success << "\t" << avg_fail << "\t" << avg_red << "\t\t" << avg_fun << "\t" << avg_iter <<  "\t" << avg_time;
    avg_time /= size;
    cout << "(" << avg_time << ")" << endl;

    unsigned int z;
    for(z = 0, size=Weights.cols(); z < size; z++)
        cout << *(Weights.data()+z) << ",";
    cout << endl;
}

