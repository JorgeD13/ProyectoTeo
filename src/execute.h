//
// Created by Jorge on 13/10/2020.
//

#ifndef PROYECTOTEO_EXECUTE_H
#define PROYECTOTEO_EXECUTE_H

#include "libs.h"

class execute {
private:
    int _key_;
public:
    execute(int key);
    ~execute();
    void ReciveData();
    void ProcessData();
    void ShowData();
    void exe();
};


#endif //PROYECTOTEO_EXECUTE_H
