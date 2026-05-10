//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_FIRST_FIT_ALGORITHM_H
#define SEGMENTATION_MEMORY_MANAGER_FIRST_FIT_ALGORITHM_H
#include "Allocating_Algorithm.h"


class First_Fit_Algorithm : public Allocating_Algorithm{
public:
    void allocate() override;
};


#endif //SEGMENTATION_MEMORY_MANAGER_FIRST_FIT_ALGORITHM_H