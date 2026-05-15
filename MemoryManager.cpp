//
// Created by hp on 5/10/2026.
//

#include "MemoryManager.h"
#include <algorithm>
#include <iomanip>

MemoryManager::MemoryManager(int totalMemory) : totalMemory(totalMemory), useFirstFit(true) {
    // Initialize with one large free partition
    addDefaultHole();
}

void MemoryManager::setAllocationAlgorithm(Allocating_Algorithm* algorithm) {
    // Check if it's First-Fit or Best-Fit algorithm
    if (dynamic_cast<First_Fit_Algorithm*>(algorithm) != nullptr) {
        useFirstFit = true;
    } else if (dynamic_cast<Best_Fit_Algorithm*>(algorithm) != nullptr) {
        useFirstFit = false;
    }
    allocater_manager = std::make_unique<Allocater_Manager>(algorithm);
}

void MemoryManager::addProcess(std::unique_ptr<Process> process) {
    processes.push_back(std::move(process));
}

bool MemoryManager::allocateProcess(const std::string& processName) {
    Process* proc = getProcess(processName);
    if (!proc) {
        std::cout << "[ERROR] Process " << processName << " not found!\n";
        return false;
    }
    
    // Pre-flight check: verify each segment can fit
    for (int i = 0; i < proc->getSegmentCount(); i++) {
        Segment* seg = proc->getSegment(i);
        if (findFreeHole(seg->getSize(), useFirstFit) == -1) {
            std::cout << "[ERROR] Process " << processName << " cannot be allocated - not enough contiguous memory!\n";
            return false;
        }
    }
    
    // Create a copy of partitions to simulate allocation
    auto originalPartitions = partitions;
    
    // Allocate each segment
    for (int i = 0; i < proc->getSegmentCount(); i++) {
        Segment* seg = proc->getSegment(i);
        int holeIndex = findFreeHole(seg->getSize(), useFirstFit);
        
        if (holeIndex != -1) {
            // Save hole info before modifying partitions
            int holeStartAddr = partitions[holeIndex].startAddress;
            int holeSize = partitions[holeIndex].size;
            
            // Create new allocated partition
            Partition newPartition(processName, seg->getName(), holeStartAddr, seg->getSize(), true);
            seg->setStartAddress(holeStartAddr);
            
            // Remove old hole
            partitions.erase(partitions.begin() + holeIndex);
            
            // Insert allocated partition
            partitions.insert(partitions.begin() + holeIndex, newPartition);
            
            // Insert remaining hole if any space left
            if (holeSize > seg->getSize()) {
                int remainingStartAddr = holeStartAddr + seg->getSize();
                int remainingSize = holeSize - seg->getSize();
                Partition remainingHole("FREE", "FREE", remainingStartAddr, remainingSize, false);
                partitions.insert(partitions.begin() + holeIndex + 1, remainingHole);
            }
        } else {
            // If any segment fails to allocate after pre-flight check, rollback and fail
            partitions = originalPartitions;
            std::cout << "[ERROR] Process " << processName << " cannot be allocated - insufficient memory available!\n";
            return false;
        }
    }
    
    std::cout << ">> Process " << processName << " allocated successfully!\n";
    return true;
}

bool MemoryManager::deallocateProcess(const std::string& processName) {
    auto it = partitions.begin();
    bool found = false;
    
    while (it != partitions.end()) {
        if (it->isAllocated && it->processName == processName) {
            it->isAllocated = false;
            it->processName = "FREE";
            it->segmentName = "FREE";
            found = true;
            ++it;
        } else {
            ++it;
        }
    }
    
    if (found) {
        // Merge adjacent free partitions
        for (int i = 0; i < partitions.size() - 1; i++) {
            if (!partitions[i].isAllocated && !partitions[i + 1].isAllocated) {
                partitions[i].size += partitions[i + 1].size;
                partitions.erase(partitions.begin() + i + 1);
                i--;
            }
        }
        std::cout << ">> Process " << processName << " deallocated successfully!\n";
        return true;
    }
    
    std::cout << "[ERROR] Process " << processName << " not found!\n";
    return false;
}

Process* MemoryManager::getProcess(const std::string& processName) {
    for (auto& proc : processes) {
        if (proc->getName() == processName) {
            return proc.get();
        }
    }
    return nullptr;
}

int MemoryManager::findFreeHole(int size, bool isFirstFit) const {
    if (isFirstFit) {
        // First-Fit: find first hole that fits
        for (int i = 0; i < partitions.size(); i++) {
            if (!partitions[i].isAllocated && partitions[i].size >= size) {
                return i;
            }
        }
    } else {
        // Best-Fit: find smallest hole that fits
        int bestIndex = -1;
        int smallestSize = totalMemory + 1;
        
        for (int i = 0; i < partitions.size(); i++) {
            if (!partitions[i].isAllocated && partitions[i].size >= size && partitions[i].size < smallestSize) {
                bestIndex = i;
                smallestSize = partitions[i].size;
            }
        }
        return bestIndex;
    }
    return -1;
}

void MemoryManager::displayMemoryLayout() const {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "MEMORY LAYOUT (Total: " << totalMemory << " bytes)\n";
    std::cout << std::string(80, '=') << "\n";
    
    int memoryIndex = 0;
    for (const auto& partition : partitions) {
        int percentage = (partition.size * 100) / totalMemory;
        std::string displayName = partition.isAllocated ? 
            (partition.processName + ":" + partition.segmentName) : "FREE";
        
        std::cout << "[";
        for (int i = 0; i < percentage; i++) {
            std::cout << "#";
        }
        for (int i = percentage; i < 100; i++) {
            std::cout << " ";
        }
        std::cout << "] " << std::setw(20) << displayName << " (" 
                  << partition.startAddress << "-" << (partition.startAddress + partition.size - 1) 
                  << ") " << partition.size << " bytes\n";
    }
}

void MemoryManager::displayAllocationTable() const {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "ALLOCATION TABLE\n";
    std::cout << std::string(100, '=') << "\n";
    std::cout << std::left << std::setw(15) << "Process Name" << std::setw(15) << "Segment Name" 
              << std::setw(15) << "Start Address" << std::setw(15) << "Size" << "Status\n";
    std::cout << std::string(100, '-') << "\n";
    
    for (const auto& partition : partitions) {
        if (partition.isAllocated) {
            std::cout << std::left << std::setw(15) << partition.processName 
                      << std::setw(15) << partition.segmentName
                      << std::setw(15) << partition.startAddress
                      << std::setw(15) << partition.size << "ALLOCATED\n";
        }
    }
}

void MemoryManager::displayFreePartitions() const {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "FREE PARTITIONS (HOLES)\n";
    std::cout << std::string(80, '=') << "\n";
    std::cout << std::left << std::setw(15) << "Start Address" 
              << std::setw(15) << "Size" << "End Address\n";
    std::cout << std::string(80, '-') << "\n";
    
    int totalFree = 0;
    for (const auto& partition : partitions) {
        if (!partition.isAllocated) {
            std::cout << std::left << std::setw(15) << partition.startAddress 
                      << std::setw(15) << partition.size
                      << (partition.startAddress + partition.size - 1) << "\n";
            totalFree += partition.size;
        }
    }
    std::cout << std::string(80, '-') << "\n";
    std::cout << "Total Free: " << totalFree << " bytes\n";
}

void MemoryManager::displayProcessSegments() const {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "PROCESS SEGMENTS DETAILS\n";
    std::cout << std::string(100, '=') << "\n";
    
    for (const auto& proc : processes) {
        std::cout << "\nProcess: " << proc->getName() << " (Total Size: " << proc->getTotalSize() << " bytes)\n";
        std::cout << std::string(100, '-') << "\n";
        std::cout << std::left << std::setw(20) << "Segment Name" 
                  << std::setw(15) << "Size" << std::setw(15) << "Start Address" << "Allocated\n";
        
        for (int i = 0; i < proc->getSegmentCount(); i++) {
            Segment* seg = proc->getSegment(i);
            bool allocated = false;
            int startAddr = -1;
            
            for (const auto& part : partitions) {
                if (part.isAllocated && part.processName == proc->getName() && part.segmentName == seg->getName()) {
                    allocated = true;
                    startAddr = part.startAddress;
                    break;
                }
            }
            
            std::cout << std::left << std::setw(20) << seg->getName() 
                      << std::setw(15) << seg->getSize()
                      << std::setw(15) << (startAddr >= 0 ? std::to_string(startAddr) : "NOT ALLOCATED")
                      << (allocated ? "YES" : "NO") << "\n";
        }
    }
}

void MemoryManager::displayMemoryLayoutASCII() const {
    std::cout << "\n" << std::string(100, '=') << "\n";
    std::cout << "MEMORY LAYOUT - ASCII VISUALIZATION (Total: " << totalMemory << " bytes)\n";
    std::cout << std::string(100, '=') << "\n";
    
    // Create a visual representation of memory
    const int BLOCK_WIDTH = 80;
    double bytesPerChar = (double)totalMemory / BLOCK_WIDTH;
    
    std::cout << "Address | ";
    for (const auto& partition : partitions) {
        int blockWidth = (partition.size + bytesPerChar - 1) / bytesPerChar;
        blockWidth = std::max(1, blockWidth);
        
        char fillChar = partition.isAllocated ? '#' : '-';
        std::string displayName = partition.isAllocated ? 
            (partition.processName[0] + std::string(1, partition.segmentName[0])) : "F";
        
        for (int i = 0; i < blockWidth; i++) {
            std::cout << fillChar;
        }
    }
    std::cout << "\n";
    
    std::cout << "\nDetailed Partitions:\n";
    std::cout << std::string(100, '-') << "\n";
    std::cout << std::left << std::setw(12) << "Address" << std::setw(12) << "Size" 
              << std::setw(30) << "Partition" << "Status\n";
    std::cout << std::string(100, '-') << "\n";
    
    for (const auto& partition : partitions) {
        std::string displayName = partition.isAllocated ? 
            (partition.processName + ":" + partition.segmentName) : "FREE";
        std::string status = partition.isAllocated ? "ALLOCATED" : "FREE";
        
        std::cout << std::left << std::setw(12) << partition.startAddress 
                  << std::setw(12) << partition.size
                  << std::setw(30) << displayName << status << "\n";
    }
    std::cout << std::string(100, '=') << "\n";
}

void MemoryManager::addHole(int startAddress, int size) {
    if (size <= 0 || startAddress < 0 || startAddress + size > totalMemory) {
        std::cout << "[ERROR] Invalid hole: address must be >= 0 and address + size <= " << totalMemory << "\n";
        return;
    }
    
    // Check for overlaps
    for (const auto& partition : partitions) {
        if ((startAddress >= partition.startAddress && startAddress < partition.startAddress + partition.size) ||
            (startAddress + size > partition.startAddress && startAddress + size <= partition.startAddress + partition.size) ||
            (startAddress <= partition.startAddress && startAddress + size >= partition.startAddress + partition.size)) {
            std::cout << "[ERROR] Hole overlaps with existing partition!\n";
            return;
        }
    }
    
    // Add hole and sort
    partitions.emplace_back("FREE", "FREE", startAddress, size, false);
    std::sort(partitions.begin(), partitions.end(), 
        [](const Partition& a, const Partition& b) {
            return a.startAddress < b.startAddress;
        });
    
    std::cout << "[OK] Hole added: Address " << startAddress << ", Size " << size << " bytes\n";
}

void MemoryManager::clearPartitions() {
    partitions.clear();
    // Recreate empty state - user will add holes
}

void MemoryManager::addDefaultHole() {
    partitions.clear();
    partitions.emplace_back("FREE", "FREE", 0, totalMemory, false);
}

void MemoryManager::defragmentMemory() {
    std::cout << "[INFO] Defragmenting memory...\n";
    // Implementation of defragmentation (optional enhancement)
}

MemoryManager::~MemoryManager() {}