//
// Created by Jana Wael on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_BEST_FIT_ALGORITHM_H
#define SEGMENTATION_MEMORY_MANAGER_BEST_FIT_ALGORITHM_H
#include "Allocating_Algorithm.h"


class Best_Fit_Algorithm : public Allocating_Algorithm{
public:
    Best_Fit_Algorithm();
    void allocate() override;
};


#endif //SEGMENTATION_MEMORY_MANAGER_BEST_FIT_ALGORITHM_H