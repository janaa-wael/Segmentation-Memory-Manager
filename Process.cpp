//
// Created by hp on 5/10/2026.
//

#include "Process.h"

Process::Process(std::string name) : name(name) {}

void Process::addSegment(std::unique_ptr<Segment> segment) {
    segments.push_back(std::move(segment));
}

std::string Process::getName() const {
    return name;
}

int Process::getSegmentCount() const {
    return segments.size();
}

Segment* Process::getSegment(int index) const {
    if (index >= 0 && index < segments.size()) {
        return segments[index].get();
    }
    return nullptr;
}

int Process::getTotalSize() const {
    int total = 0;
    for (const auto& seg : segments) {
        total += seg->getSize();
    }
    return total;
}

Process::~Process() {}