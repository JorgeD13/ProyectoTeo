//
// Created by Jorge on 13/10/2020.
//

#include "execute.h"
#include "AFD.h"

execute::execute(int key) : _key_(key) {}

execute::~execute() {
    _key_ = 0;
}

void execute::ReciveData() {
    std::string s = "../input/input" + std::to_string(_key_);
    std::ifstream ifs(s);
    std::vector<int> Efinals;
    int qStates, init, qFinals, eFinal;

    ifs >> qStates;
    ifs >> init;
    ifs >> qFinals;
    Efinals = std::vector<int> (qStates, 0);
    for(int i=0; i< qFinals; i++) {
        ifs >> eFinal;
        Efinals[eFinal] = 1;
    }

    AFD afd(qStates, init, Efinals);
    //std::cout << qStates << init << qFinals << std::endl;
    int from, transition, to;
    for(int i=0; i<qStates; i++) {
        ifs >> from; ifs >> transition; ifs >> to;
        afd.Transition(from, transition, to);
        ifs >> from; ifs >> transition; ifs >> to;
        afd.Transition(from, transition, to);
    }
    std::cout << "------------------------ AFD dado: ------------------------\n";
    afd.PrintAFD();
    std::cout << "\n";
    std::cout << "-------------------- Invertir Estados: --------------------\n";
    AFN afn = RevertAFD(afd);
    std::cout << "---------------------- Deterministar: ----------------------\n";
    Det(afn);
}

void execute::ProcessData() {

}

void execute::ShowData() {

}

void execute::exe() {
    ReciveData();
    ProcessData();
    ShowData();
}