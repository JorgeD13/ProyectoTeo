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

    std::string t;
    //getline(ifs, t);
    ifs >> t;
    std::cout << stoi(t)+1 << std::endl;
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