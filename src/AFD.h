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
            v (std::vector<std::pair<int, int>> (qStates)),
            _states_ (qStates),
            _init_ (init),
            _finals_ (finals)
        {};

    ~AFD() = default;

    void Transition(int from, int transition, int to) {
        if ( transition )
            v[from].second = to;
        else
            v[from].first = to;
    }

    void PrintAFD() {
        for (int i=0; i<_states_; i++) {
            int first = v[i].first;
            int second = v[i].second;
            if ( i == _init_ && _finals_[i] ) {
                std::cout << "->\t(" << i << ") -0-> " << first << std::endl;
                std::cout << "->\t(" << i << ") -1-> " << second << std::endl;
                std::cout << std::endl;
            } else if ( i == _init_ ) {
                std::cout << "->\t " << i << "  -0-> " << first << std::endl;
                std::cout << "->\t " << i << "  -1-> " << second << std::endl;
                std::cout << std::endl;
            } else if ( _finals_[i] ) {
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
            v (new std::pair<std::list<int>, std::list<int>>[qStates]),
            _initials_ (initials),
            _states_ (qStates),
            _final_ (final)
        {};

    ~AFN() {
        delete [] v;
    }

    void Transition(int from, int transition, int to) const {
        if ( transition )
            v[from].second.push_back(to);
        else
            v[from].first.push_back(to);
    }

    void PrintAFN() {
        for (int i=0; i<_states_; i++) {
            if ( i == _final_ && _initials_[i] ) {
                for (auto x : v[i].first)
                    std::cout << "->\t(" << i << ") -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "->\t(" << i << ") -1-> " << y << std::endl;
            } else if ( _initials_[i] ) {
                for (auto x : v[i].first)
                    std::cout << "->\t " << i << "  -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "->\t " << i << "  -1-> " << y << std::endl;
            } else if ( i == _final_ ) {
                for (auto x : v[i].first)
                    std::cout << "\t" << "(" << i << ") -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "\t" << "(" << i << ") -1-> " << y << std::endl;
            } else {
                for (auto x : v[i].first)
                    std::cout << "\t " << i << "  -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "\t " << i << "  -0-> " << y << std::endl;
            }
            std::cout << std::endl;
        }
    }

};

AFN RevertAFD(AFD& afd) {
    AFN afn(afd._states_, afd._init_, afd._finals_);
    for (int i=0; i<afd._states_; i++) {
        afn.Transition(afd.v[i].first, 0, i);
        afn.Transition(afd.v[i].second, 1, i);
    }

    afn.PrintAFN();
    return afn;
}

std::vector<std::vector<int>> SubSets(std::vector<int>& set) {
    std::vector<std::vector<int>> subsets;
    subsets.emplace_back();

    for (int & i : set) {
        auto subSetTemp = subsets;
        for (auto& j : subSetTemp)
            j.push_back(i);

        for (auto& j : subSetTemp)
            subsets.push_back(j);
    }

    return subsets;
}

AFD Det(AFN& afn) {
    int states = (int)pow(2, afn._states_);
    std::map<std::vector<int>, int> m;          // Se usa para los nuevos estados
    std::vector<int> s;                         // Para hallar los subconjuntos
    std::vector<int> finals(states, 0);     // Para enviar de parámetro al nuevo afd
    std::vector<int> initials;                  // Para encontrar el estado inicial formado de un conjunto de estados iniciales en el afn

    int ind=0;
    for (; ind<afn._states_; ind++)
        s.push_back(ind);

    ind=0;
    for (const auto &x : SubSets(s)) {
        m[x] = ind++;
        for (auto y : x)
            if (y == afn._final_)
                finals[m[x]] = 1;
    }

    for (int i=0; i<afn._states_; i++)
        if ( afn._initials_[i] )
            initials.push_back(i);

    AFD afd(states, m[initials], finals);

    for (const auto& it : m) {
        if ( it.first.empty() ) {
            afd.Transition(it.second, 0, it.second);
            afd.Transition(it.second, 1, it.second);
        } else {
            std::set<int> SetOfStates0, SetOfStates1;                       // Para hacer una union de los estados a los que transiciona un conjunto de estados
            for (const auto& x : it.first) {                                // En el loop se recorren los estados del conjunto de estados
                for (const auto& y : afn.v[x].first)
                    SetOfStates0.insert(y);                                 // Se agregan las transiciones con 0
                for (const auto& y : afn.v[x].second)
                    SetOfStates1.insert(y);                                 // Se agregan las transiciones con 1
            }

            // std::vector<int> v0(SetOfStates0.begin(), SetOfStates0.end()),   // Conjunto de estados a los que transiciona con 0
            //                 v1(SetOfStates1.begin(), SetOfStates1.end());   // Conjunto de estados a los que transiciona con 1

            afd.Transition(it.second, 0, m[ std::vector<int>(SetOfStates0.begin(), SetOfStates0.end()) ]);
            afd.Transition(it.second, 1, m[ std::vector<int>(SetOfStates1.begin(), SetOfStates1.end()) ]);
        }
    }

    afd.PrintAFD();

    return afd;
}

AFD Reacheable(AFD& afd) {
    std::vector<bool> reachables(afd._states_, false);
    std::queue<int> q;

    q.push(afd._init_);
    reachables[afd._init_] = true;

    while ( !q.empty() ) {
        int u = q.front();
        q.pop();
        if ( !reachables[afd.v[u].first] ) {
            q.push(afd.v[u].first);
            reachables[afd.v[u].first] = true;
        }

        if ( !reachables[afd.v[u].second] ) {
            q.push(afd.v[u].second);
            reachables[afd.v[u].second] = true;
        }
    }

    std::unordered_map<int, int> m;
    for (int i=0, indice = 0; i<afd._states_; i++)
        if ( reachables[i] )
            m[i] = indice++;

    std::vector<int> finals(m.size(), 0);

    for (auto x : m)
        if ( afd._finals_[x.first] )
            finals[x.second] = 1;

    AFD Min(m.size(), m[afd._init_], finals);

    for (auto x : m) {
        Min.Transition(x.second, 0, m[afd.v[x.first].first]);
        Min.Transition(x.second, 1, m[afd.v[x.first].second]);
    }

    Min.PrintAFD();

    return Min;
}

#endif //PROYECTOTEO_AFD_H
