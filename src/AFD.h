//
// Created by Jorge on 18/10/2020.
//

#ifndef PROYECTOTEO_AFD_H
#define PROYECTOTEO_AFD_H

#include <utility>

#include "libs.h"

// #define INF numeric_limits<int>::max()

struct AFD{
    std::vector<std::pair<int, int>> v; // El elemento de la posición i transiciona en 0 con first y en 1 con second
    std::vector<int> _finals_;
    int _init_;
    int _states_;

    AFD(int qStates, int init, std::vector<int>& finals) :
            v(std::vector<std::pair<int, int>> (qStates)),
            _states_(qStates),
            _init_(init),
            _finals_(finals) {};

    ~AFD() = default;

    void Transition(int from, int transition, int to) {
        if(transition)
            v[from].second = to;
        else
            v[from].first = to;
    }

    void PrintAFD() {
        for (int i=0; i<_states_; i++) {
            int first = v[i].first;
            int second = v[i].second;
            if (i == _init_) {
                std::cout << "->\t " << i << "  -0-> " << first << std::endl;
                std::cout << "->\t " << i << "  -1-> " << second << std::endl;
                std::cout << std::endl;
            } else if (_finals_[i]) {
                std::cout << "\t" << "(" << i << ") -0-> " << first << std::endl;
                std::cout << "\t" << "(" << i << ") -1-> " << second << std::endl;
                std::cout << std::endl;
            } else {
                std::cout << "\t " << i << "  -0-> " << first << std::endl;
                std::cout << "\t " << i << "  -1-> " << second << std::endl;
                std::cout << std::endl;
            }
        }
    }

};

struct AFN{
    std::pair<std::list<int>, std::list<int>> *v;
    std::vector<int> _initials_;           // Las claves serán los estados y se verificarán si exi
    int _final_;
    int _states_;

    AFN(int qStates, int final, std::vector<int>& initials) :
            v(new std::pair<std::list<int>, std::list<int>>[qStates]),
            _initials_(initials),
            _states_(qStates),
            _final_(final)
    {}

    ~AFN() {
        delete [] v;
    }

    void transition(int from, int transition, int to) const {
        if (transition)
            v[from].first.push_back(to);
        else
            v[from].second.push_back(to);
    }

    void PrintAFN() {
        for (int i=0; i<_states_; i++) {
            if (_initials_[i]) {
                for (auto x : v[i].first)
                    std::cout << "->\t " << i << "  -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "->\t " << i << "  -1-> " << y << std::endl;
                std::cout << std::endl;
            } else if (i == _final_) {
                for (auto x : v[i].first)
                    std::cout << "\t" << "(" << i << ") -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "\t" << "(" << i << ") -1-> " << y << std::endl;
            } else {
                for (auto x : v[i].first)
                    std::cout << "\t " << i << "  -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "\t " << i << "  -0-> " << y << std::endl;
                std::cout << std::endl;
            }
        }
    }

};

AFN RevertAFD(AFD& afd) {
    AFN afn(afd._states_, afd._init_, afd._finals_);
    for (int i=0; i<afd._states_; i++) {
        afn.v[afd.v[i].first].first.push_back(i);
        afn.v[afd.v[i].second].second.push_back(i);
    }

    afn.PrintAFN();
    return afn;
}

AFD Det(AFN& afn) {


    return afd;
}

#endif //PROYECTOTEO_AFD_H
