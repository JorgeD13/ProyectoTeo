//
// Created by Jorge on 18/10/2020.
//

#ifndef PROYECTOTEO_AFD_H
#define PROYECTOTEO_AFD_H

#include "libs.h"

// #define INF numeric_limits<int>::max()

class AFD{
private:
    std::vector<std::pair<int, int>> v; // El elemento de la posición i transiciona en 0 con first y en 1 con second
    std::vector<int> Efinales;
    int _init_;
    int _states_;
public:
    AFD(int qStates, int init, int qFinales, std::vector<int>& finals) :
            v(std::vector<std::pair<int, int>> (qStates)),
            _states_(qStates),
            _init_(init),
            Efinales(finals) {};

    void Transition(int from, int transition, int to) {
        if(transition)
            v[from].second = to;
        else
            v[from].first = to;
    }

    void PrintAFD() {
        for(int i=0; i<_states_; i++) {
            if(i == _init_) {
                std::cout << "->\t" << i << "-0->" << v[i].first << std::endl;
                std::cout << "->\t" << i << "-1->" << v[i].second << std::endl;
                std::cout << std::endl;
            } else if(Efinales[i]) {
                std::cout << "\t" << "(" << i << ") -0->" << v[i].first << std::endl;
                std::cout << "\t" << "(" << i << ") -1->" << v[i].second << std::endl;
                std::cout << std::endl;
            } else {
                std::cout << "\t" << i << "-0->" << v[i].first << std::endl;
                std::cout << "\t" << i << "-1->" << v[i].second << std::endl;
                std::cout << std::endl;
            }
        }
    }
};

#endif //PROYECTOTEO_AFD_H
