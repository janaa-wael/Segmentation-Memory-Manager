//
// Created by hp on 5/10/2026.
//

#include "Allocater_Manager.h"

Allocater_Manager::Allocater_Manager(Allocating_Algorithm* algorithm)
    :algorithm(algorithm)
{

}

void Allocater_Manager::allocateSegment() {
    if (algorithm) {
        algorithm->allocate();
    }
}

Allocater_Manager::~Allocater_Manager() {
    delete algorithm;
}
