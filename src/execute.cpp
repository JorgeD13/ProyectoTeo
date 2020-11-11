//
// Created by Jorge on 13/10/2020.
//

#include "execute.h"
#include "FilesManagement.h"

execute::execute(int key) : _key_ (key) {}

execute::~execute() {
    _key_ = 0;
}

void execute::exe() {
    // Declaring AFDs
    AFD afd, Final1;

    // Read archives
    ReciveData(_key_, afd);

    // Show original AFD
    afd.PrintAFD();

    std::cout << "MINIMO:\n";

    // Running the algorithm
    Brzozowski(afd, Final1);

    // Showing the AFD with the minimum number of states
    Final1.PrintAFD();

    // Expresing the AFD in the same format of the input
    ShowData(_key_, Final1);

    // TODO
    /*
     * El algoritmo de estados equivalentes
     * La idea es crear una tabla para que
     * los estados que no sean distinguibles
     * se unan en un solo estado...
     */
    std::cout << "Algoritmo de estados equivalentes" << std::endl;
    EqualStatesAlgorithm(afd);
}

    /*
    std::cout << "------------------------ AFD dado: ------------------------\n";
    afd.PrintAFD();
    std::cout << "\n";
    std::cout << "-------------------- Invertir Estados: --------------------\n";
    AFN afn = RevertAFD(afd);
    std::cout << "---------------------- Deterministar: ----------------------\n";
    AFD afd1 = Det(afn);
    std::cout << "------------------- Estados Alcanzables: -------------------\n";
    AFD afd2 = Reacheable(afd1);

    std::cout << "-------------------- Invertir Estados: --------------------\n";
    AFN afn1 = RevertAFD(afd2);
    std::cout << "---------------------- Deterministar: ----------------------\n";
    AFD afd3 = Det(afn1);
    std::cout << "------------------- Estados Alcanzables: -------------------\n";
    AFD afd4 = Reacheable(afd3);
    */