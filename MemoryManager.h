//
// Created by hp on 5/10/2026.
//

#ifndef SEGMENTATION_MEMORY_MANAGER_MEMORYMANAGER_H
#define SEGMENTATION_MEMORY_MANAGER_MEMORYMANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include "Segment.h"
#include "Process.h"
#include "Allocater_Manager.h"
#include "First_Fit_Algorithm.h"
#include "Best_Fit_Algorithm.h"

struct Partition {
    std::string processName;
    std::string segmentName;
    int startAddress;
    int size;
    bool isAllocated;
    
    Partition(std::string pName, std::string sName, int start, int sz, bool allocated)
        : processName(pName), segmentName(sName), startAddress(start), size(sz), isAllocated(allocated) {}
};

class MemoryManager {
private:
    int totalMemory;
    bool useFirstFit;
    std::vector<Partition> partitions;
    std::vector<std::unique_ptr<Process>> processes;
    std::unique_ptr<Allocater_Manager> allocater_manager;
    
public:
    MemoryManager(int totalMemory);
    
    void setAllocationAlgorithm(Allocating_Algorithm* algorithm);
    void addProcess(std::unique_ptr<Process> process);
    
    bool allocateProcess(const std::string& processName);
    bool deallocateProcess(const std::string& processName);
    
    Process* getProcess(const std::string& processName);
    int findFreeHole(int size, bool isFirstFit) const;
    
    void addHole(int startAddress, int size);
    void clearPartitions();
    void addDefaultHole();
    
    void displayMemoryLayout() const;
    void displayAllocationTable() const;
    void displayFreePartitions() const;
    void displayProcessSegments() const;
    void displayMemoryLayoutASCII() const;
    void defragmentMemory();
    
    ~MemoryManager();
};


#endif //SEGMENTATION_MEMORY_MANAGER_MEMORYMANAGER_H