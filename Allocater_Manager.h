//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_ALLOCATER_MANAGER_H
#define SEGMENTATION_MEMORY_MANAGER_ALLOCATER_MANAGER_H

#include "Allocating_Algorithm.h"

class Allocater_Manager {
private:
    Allocating_Algorithm* algorithm;
public:
    Allocater_Manager(Allocating_Algorithm* algorithm);
    void allocateSegment();
    ~Allocater_Manager();
};


#endif //SEGMENTATION_MEMORY_MANAGER_ALLOCATER_MANAGER_H