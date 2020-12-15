//
// Created by Jorge on 9/11/2020.
//

#ifndef PROYECTOTEO_FILESMANAGEMENT_H
#define PROYECTOTEO_FILESMANAGEMENT_H

#include "AFD.h"

void ReciveData(int _key_, AFD& afd) {
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

    AFD temp(qStates, init, Efinals);

    int from, transition, to;
    for(int i=0; i<qStates; i++) {
        ifs >> from; ifs >> transition; ifs >> to;
        temp.Transition(from, transition, to);
        ifs >> from; ifs >> transition; ifs >> to;
        temp.Transition(from, transition, to);
    }
    afd = temp;
}

void ShowData(int _key_, AFD& Final) {
    int NewQStates = Final._states_;
    int NewInit = Final._init_;
    std::string of = "../output/output" + std::to_string(_key_);
    std::ofstream ofs(of);
    ofs << NewQStates << " " << NewInit;
    std::string NewFinals;
    int NewQFinals=0;
    for (int i=0; i<Final._finals_.size(); i++) {
        if (Final._finals_[i] != 0) {
            NewFinals += " ";
            NewFinals += std::to_string(i);
            NewQFinals++;
        }
    }
    ofs << " " << NewQFinals << NewFinals;
    for (int i=0; i<Final.v.size(); i++) {
        ofs << "\n" << i << " " << 0 << " " << Final.v[i].first;
        ofs << "\n" << i << " " << 1 << " " << Final.v[i].second;
    }

    ofs.close();
}

void ShowData5(int _key_, AFD& Final) {
    int NewQStates = Final._states_;
    int NewInit = Final._init_;
    std::string of = "../output5/output" + std::to_string(_key_);
    std::ofstream ofs(of);
    ofs << NewQStates << " " << NewInit;
    std::string NewFinals;
    int NewQFinals=0;
    for (int i=0; i<Final._finals_.size(); i++) {
        if (Final._finals_[i] != 0) {
            NewFinals += " ";
            NewFinals += std::to_string(i);
            NewQFinals++;
        }
    }
    ofs << " " << NewQFinals << NewFinals;
    for (int i=0; i<Final.v.size(); i++) {
        ofs << "\n" << i << " " << 0 << " " << Final.v[i].first;
        ofs << "\n" << i << " " << 1 << " " << Final.v[i].second;
    }

    ofs.close();
}

#endif //PROYECTOTEO_FILESMANAGEMENT_H
