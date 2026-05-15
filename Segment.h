//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_SEGMENT_H
#define SEGMENTATION_MEMORY_MANAGER_SEGMENT_H

#include <string>

class Segment {
private:
    std::string name;
    int size;
    int startAddress;
public:
    Segment();
    Segment(const std::string& name, int size);
    std::string getName() const;
    int getSize() const;
    int getStartAddress() const;
    void setStartAddress(int address);
};


#endif //SEGMENTATION_MEMORY_MANAGER_SEGMENT_H