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

    AFD() :
    _init_ (0),
    _states_ (0)
    {}

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

    // afn.PrintAFN();
    return afn;
}

// Sacado de:
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
    // std::cout << "aqui" << std::endl;
    auto subs = SubSets(s);
    // std::cout << "aqui" << std::endl;
    for (const auto &x : subs) {
        m[x] = ind++;
        for (auto y : x)
            if (y == afn._final_)
                finals[m[x]] = 1;
    }
    // std::cout << "aqui" << std::endl;
    for (int i=0; i<afn._states_; i++)
        if ( afn._initials_[i] )
            initials.push_back(i);

    AFD afd(states, m[initials], finals);

    for (const auto& it : m) { // key, value
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
            //                  v1(SetOfStates1.begin(), SetOfStates1.end());   // Conjunto de estados a los que transiciona con 1

            afd.Transition(it.second, 0, m[ std::vector<int>(SetOfStates0.begin(), SetOfStates0.end()) ]);
            afd.Transition(it.second, 1, m[ std::vector<int>(SetOfStates1.begin(), SetOfStates1.end()) ]);
        }
    }

    // afd.PrintAFD();

    return afd;
}

AFD Reacheable(AFD& afd) {
    std::vector<bool> reachables(afd._states_, false);
    std::queue<int> q;

    q.push(afd._init_);
    reachables[ afd._init_ ] = true;

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

    /*
     * M[0] = 0
     * M[5] = 1
     * M[6] = 2
     */

    std::vector<int> finals(m.size(), 0);

    for (auto x : m)
        if ( afd._finals_[x.first] )
            finals[x.second] = 1;

    AFD Min(m.size(), m[ afd._init_ ], finals);

    for (auto x : m) {
        Min.Transition(x.second, 0, m[afd.v[x.first].first]);
        Min.Transition(x.second, 1, m[afd.v[x.first].second]);
    }

    // Min.PrintAFD();

    return Min;
}

void Brzozowski(AFD& afdinicial, AFD& afdfinal) {
    AFN afn1 = RevertAFD(afdinicial);
    AFD afd1 = Det(afn1);
    AFD afd2 = Reacheable(afd1);

    AFN afn2 = RevertAFD(afd2);
    AFD afd3 = Det(afn2);
    afdfinal = Reacheable(afd3);
}

std::vector<std::map<int, bool>> EqualStatesAlgorithm(AFD& afdinicial) {
    /// CAMBIAR A UNORDERED MAP
    // AFD afdinicial = Reacheable(afdi);
    // afdi.PrintAFD();
    // afdinicial.PrintAFD();
    std::vector<std::map<int, bool>> v(afdinicial._states_);
    std::deque<std::pair<int, int>> dq(afdinicial.v.begin(), afdinicial.v.end());

    /// TABLA
    for (int i=0; i<v.size(); i++)
        for (int j=i+1; j<v.size(); j++)
            v[i][j] = false;

    /// Distinguir estados finales
    for (auto _x_ = 0; _x_ < afdinicial._finals_.size(); _x_++)
        if ( afdinicial._finals_[ _x_ ] )
            for (auto _y_ = 0; _y_ < afdinicial._finals_.size(); _y_++)
                if ( !afdinicial._finals_[ _y_ ] ) {
                    if (_x_ < _y_) {
                        v[ _x_ ][ _y_ ] = true;
                        std::cout << _x_ << "-" << _y_ << "\n";
                    } else {
                        v[ _y_ ][ _x_ ] = true;
                        std::cout << _y_ << "-" << _x_ << "\n";
                    }
                }


    /// Imprimir tabla
    for (int i=0; i<v.size(); i++) {
        std::cout << i << ":\n";
        for (auto y : v[i])
			std::cout << "\t" << y.first << " ";
		std::cout<<std::endl;
    }

    bool count = true;  // Si hay un cambio en la tabla, contamos el cambio y volvemos a repetir
    
    while (count) {
        count  = false;
        int index = 0;
        while ( !dq.empty() ) {
            auto u = dq.front();
            dq.pop_front();
            auto it = dq.begin()++;
            int curr = index+1;
            // u=A y it B, C, D
            while ( it != dq.end() ) {
                int u0 = u.first,
                    u1 = u.second,
                    it0 = (*it).first,
                    it1 = (*it).second;
                if ( u0 < it0 && v[u0][it0] ) {
                    count = true;
                    v[index][curr] = true;
                    std::cout << index << "-" << curr << "\n";
                } else if ( it0 < u0 && v[it0][u0] ) {
                    count = true;
                    v[index][curr] = true;
                    std::cout << index << "-" << curr << "\n";
                }

                if ( u1 < it1 && v[u1][it1] ) {
                    count = true;
                    v[index][curr] = true;
                    std::cout << index << "-" << curr << "\n";
                } else if ( it1 < u1 && v[it1][u1] ) {
                    count = true;
                    v[index][curr] = true;
                    std::cout << index << "-" << curr << "\n";
                }
                it++;
                curr++;
            }
            index++;
        }
    }

    int in = 0;
    for (const auto& x : v) {
        std::cout << in++ << std::boolalpha;
        for (auto y : x)
            std::cout << "\t" << y.second;
        std::cout << "\n";
    }

    return v;
}


std::vector< std::vector<int> > EqStates(AFD& afd){
    int top = afd._states_;
    std::vector< std::vector<int> > DTable(top, std::vector<int>(top, 0));
    std::map< std::pair<int, int>, std::pair< std::pair<int, int>, std::pair<int, int> > > L;

    /// Marcamos con 1 los pares de estados que son de aceptación y de no aceptación
    for (int i = 0; i < top-1; i++) {
        for (int j = i+1; j < top; j++) {
            if ( !afd._finals_[i] && afd._finals_[j] || !afd._finals_[j] && afd._finals_[i] ) {
                DTable[i][j] = 1;
                DTable[j][i] = 1;
            }
        }
    }

    /// Determinando qué pares dependen de qué pares
    for (int i = 0; i < top-1; i++) {
        for (int j = i+1; j < top; j++) {
            // std::cout << i << " - " << j << "\n";
            /// Se aplican los pares que depende de otros pares
            L[std::make_pair(i, j)] = std::make_pair(std::make_pair(afd.v[i].first, afd.v[j].first), std::make_pair(afd.v[i].second, afd.v[j].second));
        }
    }

    /// Llenado de tabla
    bool key = true;
    while ( key ) {
        key = false;
        for (auto x : L) {
            // M[pair(p, q)] = pair( pair(r, s), pair(r, s) )
            if ( (DTable[x.second.first.first][x.second.first.second] == 1 || DTable[x.second.second.first][x.second.second.second] == 1) && DTable[x.first.first][x.first.second] != 1 ) {
                DTable[x.first.first][x.first.second] = 1;
                DTable[x.first.second][x.first.first] = 1;
                key = true;
                // std::cout << x.first.first << " -1- " << x.first.second << "\n";
            }
        }
    }

    for (int i = 0; i < top; i++)
        std::cout << "\t" << i;
    std::cout << std::endl;
    for (int i = 0; i < top; i++)
        std::cout << "\t" << "_";
    std::cout << std::endl;
    int n = 0;
    for (const auto& x : DTable) {
        std::cout << n++ << "     |\t";
        for (const auto& y : x)
            std::cout << y << "\t";
        std::cout << "\n";
    }

    return DTable;
}

void Hopcroft(AFD temp) {
    AFD afd = Reacheable(temp);

    /// Obtenemos la tabla de estados equivalentes
    std::vector< std::vector<int> > Dtable = EqStates(afd);

    /// Agrupamos los estados de la tabla en conjuntos de estados
    std::map< std::vector<int>, int > SetOfStates;
    std::vector<bool> used(Dtable.size(), false);

    int n = 0;
    int init;
    std::vector<int> finals;
    std::vector<int> Eqs(afd._states_);

    for (int i = 0; i < Dtable.size(); i++) {
        if ( !used[i] ) {
            bool key = true;
            std::vector<int> v;
            v.push_back(i);
            used[i] = true;
            if ( afd._init_ == i)
                init = n;
            if ( afd._finals_[i] ) {
                finals.push_back(1);
                key = false;
            }
            Eqs[i] = n;
            for (int j = i+1; j < Dtable.size(); j++) {
                if ( !Dtable[i][j] && !used[j]) {
                    v.push_back(j);
                    used[j] = true;
                    if ( afd._init_ == j)
                        init = n;
                    if ( afd._finals_[j] && key ) {
                        finals.push_back(1);
                        key = false;
                    }
                    Eqs[j] = n;
                }
            }
            if ( key )
                finals.push_back(0);
            SetOfStates[v] = n++;
        }
    }

    /*
    std::cout << "\nFinals:\n";
    for (const auto& x : finals)
        std::cout << x << " ";
    std::cout << "\n";

    std::cout << "\nEqs:\n";
    for (const auto& x : Eqs)
        std::cout << x << " ";
    std::cout << "\n";
     */

    /*
    for (const auto& x : SetOfStates) {
        std::cout << "( ";
        for (auto y : x.first) {
            std::cout << y << " ";
        }
        std::cout << ") -> " << x.second << "\n";
    }
    */

    /// Construir el AFD usando los conjuntos de estados como nuevos estados.
    AFD MINIMUM(SetOfStates.size(), init, finals);

    /*
    /// Antiguo:
    /// El doble for internamente tiene complejidad: O(E) porque
    /// recorre conjuntos estados que por propiedad explicada en
    /// el libro Hopcroft ningún estado se encuentra en dos conjuntos.
    */

    /// Mejorado: Cualquier elemento del conjunto de elementos tiene
    /// un equivalente en el arreglo Eqs, accedemos al primero puesto
    /// que todos los conjuntos de estados son estrictamente no vacíos
    /// y accedemos su equivalente en Eqs.

    for (const auto& x : SetOfStates) {
        int i = afd.v[ x.first[0] ].first;
        int j = afd.v[ x.first[0] ].second;
        MINIMUM.Transition(x.second, 0, i);
        MINIMUM.Transition(x.second, 1, j);
        /*
        std::set<int> v0;
        std::set<int> v1;
        for (auto y : x.first) {
            v0.insert(afd.v[y].first);
            v1.insert(afd.v[y].second);
        }

        std::vector<int> t0(v0.begin(), v0.end());
        std::vector<int> t1(v1.begin(), v1.end());
        std::sort(t0.begin(), t0.end());
        std::sort(t1.begin(), t1.end());

        /// Los primeros 2 fors se ejecutan en O(E) y el tercero se ejecuta en O(t0) y O(t1)
        for (const auto& item : SetOfStates) {
            for (auto item2 : item.first) {
                for (auto select : t0) {
                    if ( select == item2 )
                        f = item.second;
                }
                for (auto select : t1) {
                    if ( select == item2 )
                        s = item.second;
                }
            }
        }
         */

        /*
        std::cout << "Transition 0: ";
        for (auto it1 : t0)
            std::cout << it1 << " ";

        std::cout << "\nTransition 1: ";
        for (auto it1 : t1)
            std::cout << it1 << " ";
        std::cout << "\n\n";
        */
    }

    std::cout << "\nMINIMUM:\n";
    MINIMUM.PrintAFD();
}

#endif //PROYECTOTEO_AFD_H
