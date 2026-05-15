//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_PROCESS_H
#define SEGMENTATION_MEMORY_MANAGER_PROCESS_H

#include "Segment.h"
#include <memory>
#include <vector>

class Process {
private:
    std::string name;
    std::vector<std::unique_ptr<Segment>> segments;

public:
    Process(std::string name);
    void addSegment(std::unique_ptr<Segment> segment);
    std::string getName() const;
    int getSegmentCount() const;
    Segment* getSegment(int index) const;
    int getTotalSize() const;
    ~Process();
};


#endif //SEGMENTATION_MEMORY_MANAGER_PROCESS_H