//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_ALLOCATING_METHOD_H
#define SEGMENTATION_MEMORY_MANAGER_ALLOCATING_METHOD_H

#include <iostream>

class Allocating_Algorithm {
private:

public:
    virtual void allocate() = 0;
    virtual ~Allocating_Algorithm() = default;
};


#endif //SEGMENTATION_MEMORY_MANAGER_ALLOCATING_METHOD_H