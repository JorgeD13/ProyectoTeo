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
            _finals_ (finals) {};

    ~AFD() = default;

    void Transition(int from, int transition, int to) {
        if (transition)
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
            v (new std::pair<std::list<int>, std::list<int>>[qStates]),
            _initials_ (initials),
            _states_ (qStates),
            _final_ (final)
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
            if ( _initials_[i] ) {
                for (auto x : v[i].first)
                    std::cout << "->\t " << i << "  -0-> " << x << std::endl;
                for (auto y : v[i].second)
                    std::cout << "->\t " << i << "  -1-> " << y << std::endl;
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
            }
            std::cout << std::endl;
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
    std::vector<int> s;
    std::vector<int> finals(states, 0);

    int ind=0;
    for (; ind<afn._states_; ind++)
        s.push_back(ind);

    ind=0;
    for (const auto &x : SubSets(s))
        m[x] = ind++;

    ind=0;
    for (const auto& x : m) {
        for (auto y : x.first) {
            if (y == afn._final_)
                finals[ind] = 1;
        }
        ind++;
    }

    std::cout << "bien" << std::endl;

    AFD afd(states, m[afn._initials_], finals);
    std::cout << "bien2" << std::endl;

    for (int i=0; i<states; i++) {
        int s1 = afn.v[i].first.size();
        int s2 = afn.v[i].second.size();
        if (afn.v[i].first.empty())
            afd.Transition(i, 0, m[{}]);
        else if (s1 == 1)
            afd.Transition(i, 0, m[{*afn.v[i].first.begin()}]);
        else if (s1 > 1) {
            // TODO
            std::vector<int> temp;
            for (auto& x : afn.v[i].first)
                temp.push_back(x);
            sort(temp.begin(), temp.end());
            afd.Transition(i, 0, m[temp]);
        }

        if (afn.v[i].second.empty())
            afd.Transition(i, 1, m[{}]);
        else if (s2 == 1)
            afd.Transition(i, 1, m[{*afn.v[i].second.begin()}]);
        else if (s2 > 1) {
            // TODO
            std::vector<int> temp;
            for (auto& x : afn.v[i].second)
                temp.push_back(x);
            sort(temp.begin(), temp.end());
            afd.Transition(i, 1, m[temp]);
        }
    }

    afd.PrintAFD();

    return afd;
}

#endif //PROYECTOTEO_AFD_H
