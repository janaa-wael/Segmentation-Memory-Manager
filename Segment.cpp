//
// Created by hp on 5/10/2026.
//

#include "Segment.h"

Segment::Segment()
    : name("un_named segment"), size(0), startAddress(-1)
{

}

Segment::Segment(const std::string& name, int size)
    : name(name), size(size), startAddress(-1)
{

}

std::string Segment::getName() const {
    return name;
}

int Segment::getSize() const {
    return size;
}

int Segment::getStartAddress() const {
    return startAddress;
}

void Segment::setStartAddress(int address) {
    startAddress = address;
}
