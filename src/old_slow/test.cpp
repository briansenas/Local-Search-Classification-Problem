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

void printStuff(MatrixXd data, vector<char> label){
    for(unsigned int i=0;i<data.rows();i++){
        for(unsigned int j=0;j<data.cols(); j++)
            cout << data(i,j) << "  " ;
        cout << label[i] << endl;
    }
}

Eigen::MatrixXd getClassLabelled(Eigen::MatrixXd data, vector<char> Label, vector<char>& newLabel, char type){
    Eigen::MatrixXd res;
    newLabel.clear();
    res.resize(1,data.cols());
    for(unsigned int i=0,size=Label.size();i<size;i++){
        if(Label[i] == type){
            res.row(res.rows()-1) = data.row(i);
            res.conservativeResize(res.rows()+1,res.cols());
            newLabel.push_back(type);
        }
    }
    res.conservativeResize(res.rows()-1,res.cols());
    return res;
}

void getBalancedFold(Eigen::MatrixXd group1, vector<char> label1,Eigen::MatrixXd group2,
        vector<char> label2, Eigen::MatrixXd &training, vector<char> &TLabel,
        Eigen::MatrixXd &test, vector<char>& TtLabel, unsigned int num){
    TLabel.clear();
    TtLabel.clear();

    MatrixXd train1, test1;
    vector<char> tlab1, ttlab1;

    MatrixXd train2, test2;
    vector<char> tlab2, ttlab2;

    getFold(group1,label1,train1,tlab1,test1,ttlab1,num);
    getFold(group2,label2,train2,tlab2,test2,ttlab2,num);

    training.resize(train1.rows()+train2.rows(),group1.cols());
    training << train1, train2;
    test.resize(test1.rows()+test2.rows(), group1.cols());
    test << test1, test2;

    TLabel.insert(TLabel.begin(), tlab1.begin(), tlab1.end());
    TLabel.insert(TLabel.end(), tlab2.begin(), tlab2.end());
    TtLabel.insert(TtLabel.begin(), ttlab1.begin(), ttlab1.end());
    TtLabel.insert(TtLabel.end(), ttlab2.begin(), ttlab2.end());

    shuffleData(training, TLabel,0);
    shuffleData(test, TtLabel, 0);
}

int main(int argc, char** argv){

    MatrixXd data(9,2);
    data << 1,1,
            2,2,
            2,2,
            4,4,
            5,5,
            6,6,
            7,7,
            8,8,
            9,9;

    vector<char> label;
    label.push_back('b');
    label.push_back('g');
    label.push_back('b');
    label.push_back('g');
    label.push_back('b');
    label.push_back('g');
    label.push_back('b');
    label.push_back('g');
    label.push_back('b');

    MatrixXd group1, group2;
    vector<char> tlabel, ttlabel, label1, label2;
    group1 = getClassLabelled(data,label, label1, 'b');
    group2 = getClassLabelled(data, label, label2, 'g');

    printStuff(data,label);
    cout << " ############## " << endl;


    MatrixXd training, test;

    getFold(group1, label1, group2, label2, training, tlabel, test, ttlabel,0);

    printStuff(training,tlabel);
    cout << " ############## " << endl;
    printStuff(test,ttlabel);

}
