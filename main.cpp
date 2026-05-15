#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "MemoryManager.h"
#include "Process.h"
#include "Segment.h"
#include "First_Fit_Algorithm.h"
#include "Best_Fit_Algorithm.h"

using namespace std;

void displayMainMenu();
void displayAllocationMenu();

int main() {
    cout << "\n============================================================\n";
    cout << "     Memory Segmentation Management System\n";
    cout << "           (First-Fit & Best-Fit Allocation)\n";
    cout << "============================================================\n";
    
    int totalMemory = 0;
    bool memoryInitialized = false;
    
    while (true) {
        if (!memoryInitialized) {
            cout << "\nFirst, let's initialize the system memory.\n";
            cout << "Enter total memory size (in bytes): ";
            cin >> totalMemory;
            
            if (totalMemory <= 0) {
                cout << "[ERROR] Invalid memory size! Please enter a positive number.\n";
                continue;
            }
            
            memoryInitialized = true;
        }
        
        unique_ptr<MemoryManager> memoryManager = make_unique<MemoryManager>(totalMemory);
        Allocating_Algorithm* currentAlgorithm = nullptr;
        bool algorithmSelected = false;
        
        bool systemRunning = true;
        bool holesInitialized = false;
        
        while (systemRunning) {
            displayMainMenu();
            
            int choice;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            
            switch (choice) {
                case 1: {
                    // Initialize Holes
                    cout << "\n==========================================\n";
                    cout << "Initialize Memory Holes\n";
                    cout << "==========================================\n";
                    
                    if (holesInitialized) {
                        cout << "Clear existing holes? (y/n): ";
                        char clearChoice;
                        cin >> clearChoice;
                        cin.ignore();
                        
                        if (clearChoice != 'y' && clearChoice != 'Y') {
                            break;
                        }
                    }
                    
                    // Clear default partition before adding holes
                    memoryManager->clearPartitions();
                    if (holesInitialized) {
                        cout << "[OK] Partitions cleared.\n";
                    }
                    
                    bool addingHoles = true;
                    while (addingHoles) {
                        cout << "\nAdd a hole:\n";
                        cout << "Enter starting address: ";
                        int startAddr;
                        cin >> startAddr;
                        
                        cout << "Enter hole size (in bytes): ";
                        int holeSize;
                        cin >> holeSize;
                        cin.ignore();
                        
                        memoryManager->addHole(startAddr, holeSize);
                        
                        cout << "Add another hole? (y/n): ";
                        char moreHoles;
                        cin >> moreHoles;
                        cin.ignore();
                        
                        if (moreHoles != 'y' && moreHoles != 'Y') {
                            addingHoles = false;
                            holesInitialized = true;
                        }
                    }
                    break;
                }
                
                case 2: {
                    // Create Process
                    cout << "\n==========================================\n";
                    cout << "Create New Process\n";
                    cout << "==========================================\n";
                    
                    cout << "Enter process name (e.g., P1): ";
                    string processName;
                    getline(cin, processName);
                    
                    if (processName.empty()) {
                        cout << "[ERROR] Process name cannot be empty!\n";
                        break;
                    }
                    
                    cout << "Enter number of segments: ";
                    int numSegments;
                    cin >> numSegments;
                    cin.ignore();
                    
                    if (numSegments <= 0) {
                        cout << "[ERROR] Number of segments must be positive!\n";
                        break;
                    }
                    
                    auto process = make_unique<Process>(processName);
                    
                    for (int i = 0; i < numSegments; i++) {
                        cout << "\nSegment " << (i + 1) << ":\n";
                        cout << "  Enter segment name (e.g., Code, Data, Stack): ";
                        string segmentName;
                        getline(cin, segmentName);
                        
                        cout << "  Enter segment size (in bytes): ";
                        int segmentSize;
                        cin >> segmentSize;
                        cin.ignore();
                        
                        if (segmentSize <= 0) {
                            cout << "  [ERROR] Segment size must be positive!\n";
                            i--;
                            continue;
                        }
                        
                        auto segment = make_unique<Segment>(segmentName, segmentSize);
                        process->addSegment(move(segment));
                    }
                    
                    memoryManager->addProcess(move(process));
                    cout << "\n[OK] Process " << processName << " created successfully with " 
                         << numSegments << " segments!\n";
                    break;
                }
                
                case 3: {
                    // Select Allocation Algorithm
                    cout << "\n==========================================\n";
                    cout << "Select Allocation Algorithm\n";
                    cout << "==========================================\n";
                    displayAllocationMenu();
                    
                    int algoChoice;
                    cout << "Select algorithm (1-2): ";
                    cin >> algoChoice;
                    cin.ignore();
                    
                    if (currentAlgorithm) delete currentAlgorithm;
                    
                    if (algoChoice == 1) {
                        currentAlgorithm = new First_Fit_Algorithm();
                        algorithmSelected = true;
                        cout << "[OK] First-Fit algorithm selected!\n";
                    } else if (algoChoice == 2) {
                        currentAlgorithm = new Best_Fit_Algorithm();
                        algorithmSelected = true;
                        cout << "[OK] Best-Fit algorithm selected!\n";
                    } else {
                        cout << "[ERROR] Invalid choice!\n";
                        algorithmSelected = false;
                    }
                    
                    if (algorithmSelected) {
                        memoryManager->setAllocationAlgorithm(currentAlgorithm);
                    }
                    break;
                }
                
                case 4: {
                    // Allocate Process
                    if (!algorithmSelected) {
                        cout << "[ERROR] Please select an allocation algorithm first!\n";
                        break;
                    }
                    
                    cout << "\n==========================================\n";
                    cout << "Allocate Process\n";
                    cout << "==========================================\n";
                    cout << "Enter process name to allocate: ";
                    string processToAllocate;
                    getline(cin, processToAllocate);
                    
                    memoryManager->allocateProcess(processToAllocate);
                    break;
                }
                
                case 5: {
                    // Deallocate Process
                    cout << "\n==========================================\n";
                    cout << "Deallocate Process\n";
                    cout << "==========================================\n";
                    cout << "Enter process name to deallocate: ";
                    string processToDeallocate;
                    getline(cin, processToDeallocate);
                    
                    memoryManager->deallocateProcess(processToDeallocate);
                    break;
                }
                
                case 6: {
                    // Display Memory Layout
                    memoryManager->displayMemoryLayout();
                    break;
                }
                
                case 7: {
                    // Display Memory Layout ASCII
                    memoryManager->displayMemoryLayoutASCII();
                    break;
                }
                
                case 8: {
                    // Display Allocation Table
                    memoryManager->displayAllocationTable();
                    break;
                }
                
                case 9: {
                    // Display Free Partitions
                    memoryManager->displayFreePartitions();
                    break;
                }
                
                case 10: {
                    // Display Process Segments
                    memoryManager->displayProcessSegments();
                    break;
                }
                
                case 11: {
                    // Display All Information
                    memoryManager->displayProcessSegments();
                    memoryManager->displayMemoryLayoutASCII();
                    memoryManager->displayAllocationTable();
                    memoryManager->displayFreePartitions();
                    break;
                }
                
                case 12: {
                    // Reinitialize System
                    cout << "\nReinitializing system...\n";
                    systemRunning = false;
                    break;
                }
                
                case 13: {
                    // Exit
                    cout << "\nThank you for using Memory Segmentation Manager!\n";
                    cout << "============================================================\n";
                    cout << "                      Program Terminated\n";
                    cout << "============================================================\n";
                    return 0;
                }
                
                default:
                    cout << "[ERROR] Invalid choice! Please try again.\n";
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
        
        if (currentAlgorithm) delete currentAlgorithm;
    }
    
    return 0;
}

void displayMainMenu() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "             MAIN MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Initialize Memory Holes\n";
    cout << "2. Create New Process\n";
    cout << "3. Select Allocation Algorithm\n";
    cout << "4. Allocate Process\n";
    cout << "5. Deallocate Process\n";
    cout << "6. Display Memory Layout (Bar View)\n";
    cout << "7. Display Memory Layout (ASCII View)\n";
    cout << "8. Display Allocation Table\n";
    cout << "9. Display Free Partitions\n";
    cout << "10. Display Process Segments Details\n";
    cout << "11. Display All Information\n";
    cout << "12. Reinitialize System\n";
    cout << "13. Exit Program\n";
    cout << string(50, '=') << "\n";
}

void displayAllocationMenu() {
    cout << "1. First-Fit Algorithm\n";
    cout << "   (Allocates to first hole that fits)\n";
    cout << "\n2. Best-Fit Algorithm\n";
    cout << "   (Allocates to smallest hole that fits)\n";
}