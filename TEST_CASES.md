# Memory Segmentation Manager - Test Cases

## Overview
This document provides comprehensive test cases for the Memory Segmentation Management System using both First-Fit and Best-Fit allocation algorithms.

---

## Test Case 1: Basic First-Fit Allocation
**Objective**: Test basic allocation using First-Fit algorithm

**Setup**:
- Total Memory: 1000 bytes
- Holes: Address 0, Size 1000 (single contiguous block)

**Steps**:
1. Initialize system with 1000 bytes
2. Initialize holes: Start=0, Size=1000
3. Select First-Fit algorithm
4. Create Process P1 with 3 segments:
   - Code: 100 bytes
   - Data: 150 bytes
   - Stack: 75 bytes
5. Allocate P1
6. Display all information

**Expected Output**:
- P1 segments allocated at addresses 0, 100, 250 respectively
- Remaining free hole: Address 325, Size 675
- Allocation table shows 3 allocated partitions
- Memory layout shows occupied and free spaces

**Pass Criteria**: All segments allocated sequentially, correct addresses, free space calculation correct

---

## Test Case 2: Basic Best-Fit Allocation
**Objective**: Test basic allocation using Best-Fit algorithm

**Setup**:
- Total Memory: 1000 bytes
- Holes: 
  - Address 0, Size 100
  - Address 100, Size 600
  - Address 700, Size 300

**Steps**:
1. Initialize system with 1000 bytes
2. Initialize holes as listed above
3. Select Best-Fit algorithm
4. Create Process P1 with 2 segments:
   - Code: 150 bytes
   - Data: 80 bytes
5. Allocate P1
6. Display allocation table

**Expected Output**:
- Code segment (150 bytes) allocated in hole at 100 (best fit for 150 from 600)
- Data segment (80 bytes) allocated in hole at 300 (best fit for 80 from 300)
- First hole (100 bytes) remains unused
- Correct partition tracking

**Pass Criteria**: Best-fit algorithm chooses smallest suitable holes

---

## Test Case 3: Process Cannot Fit
**Objective**: Test error handling when process cannot fit

**Setup**:
- Total Memory: 500 bytes
- Holes:
  - Address 0, Size 150
  - Address 150, Size 200
  - Address 350, Size 150

**Steps**:
1. Initialize system
2. Set up holes as listed
3. Select any algorithm
4. Create Process P1 with 2 segments:
   - Code: 200 bytes
   - Data: 200 bytes (total 400 bytes needed)
5. Try to allocate P1

**Expected Output**:
- Error message: "[ERROR] Process P1 cannot be allocated - not enough contiguous memory!"
- No allocation occurs
- Free partitions remain unchanged

**Pass Criteria**: Process not allocated, error message displayed, memory unchanged

---

## Test Case 4: Multiple Process Allocation
**Objective**: Test allocating multiple processes sequentially

**Setup**:
- Total Memory: 2000 bytes
- Holes: Address 0, Size 2000

**Steps**:
1. Initialize system
2. Select First-Fit algorithm
3. Create P1: 3 segments (Code: 200, Data: 150, Stack: 100)
4. Create P2: 2 segments (Code: 300, Data: 200)
5. Create P3: 2 segments (Code: 150, Data: 100)
6. Allocate P1
7. Allocate P2
8. Allocate P3
9. Display allocation table

**Expected Output**:
- P1 allocated at addresses 0, 200, 350
- P2 allocated at addresses 450, 750
- P3 allocated at addresses 950, 1100
- Free hole remaining: Address 1200, Size 800
- All 7 segments shown in allocation table

**Pass Criteria**: All processes allocated correctly, no overlaps, addresses sequential

---

## Test Case 5: Deallocation and Hole Merging
**Objective**: Test deallocation and adjacent hole merging

**Setup**:
- Total Memory: 1000 bytes
- Initial Holes: Address 0, Size 1000

**Steps**:
1. Initialize system
2. Select First-Fit algorithm
3. Create P1: 2 segments (Code: 100, Data: 100)
4. Create P2: 2 segments (Code: 150, Data: 150)
5. Allocate P1 (addresses 0-200)
6. Allocate P2 (addresses 200-500)
7. Display free partitions (should show 1 hole: 500-1000)
8. Deallocate P1
9. Display free partitions

**Expected Output**:
- After P1 deallocation, hole at address 0 (size 200) should merge with hole at 200 (size 800)
- Result: Single free hole at address 0, Size 1000
- P2 remains allocated

**Pass Criteria**: Adjacent holes merged correctly, fragmentation handled properly

---

## Test Case 6: First-Fit vs Best-Fit Difference
**Objective**: Demonstrate difference between First-Fit and Best-Fit algorithms

**Setup**:
- Total Memory: 500 bytes
- Holes:
  - Address 0, Size 80
  - Address 80, Size 200
  - Address 280, Size 220

**Steps - First Run (First-Fit)**:
1. Initialize system
2. Select First-Fit
3. Create P1: 1 segment (Process: 150 bytes)
4. Allocate P1
5. Note allocation address

**Expected for First-Fit**: P1 allocated at address 80

**Steps - Second Run (Best-Fit)**:
1. Reinitialize system with same holes
2. Select Best-Fit
3. Create same P1
4. Allocate P1
5. Note allocation address

**Expected for Best-Fit**: P1 allocated at address 280 (smallest hole that fits is 220)

**Pass Criteria**: First-Fit uses first available, Best-Fit minimizes fragmentation

---

## Test Case 7: ASCII Memory Layout Visualization
**Objective**: Test ASCII memory visualization feature

**Setup**:
- Total Memory: 400 bytes
- Holes: Address 0, Size 400

**Steps**:
1. Initialize system
2. Create P1: 2 segments (Code: 100, Data: 100)
3. Allocate P1
4. Display Memory Layout (ASCII View)

**Expected Output**:
- ASCII visual representation showing allocated and free blocks
- Detailed partition table with addresses and sizes
- Clear indication of P1:Code and P1:Data locations
- Remaining free space clearly marked

**Pass Criteria**: Visual output is readable and accurate

---

## Test Case 8: Edge Case - Exact Fit
**Objective**: Test when segment size exactly matches hole size

**Setup**:
- Total Memory: 500 bytes
- Holes:
  - Address 0, Size 200
  - Address 200, Size 300

**Steps**:
1. Initialize system
2. Select any algorithm
3. Create P1: 2 segments (Code: 200, Data: 300)
4. Allocate P1
5. Display free partitions

**Expected Output**:
- Code segment allocated at address 0, Size 200
- Data segment allocated at address 200, Size 300
- No free holes remaining
- Free partition display shows "Total Free: 0 bytes"

**Pass Criteria**: Holes completely consumed, no fragmentation

---

## Test Case 9: Complex Fragmentation Scenario
**Objective**: Test handling of highly fragmented memory

**Setup**:
- Total Memory: 1000 bytes
- Holes:
  - Address 0, Size 100
  - Address 100, Size 150
  - Address 250, Size 100
  - Address 350, Size 200
  - Address 550, Size 100
  - Address 650, Size 350

**Steps**:
1. Initialize system with all holes
2. Select Best-Fit algorithm
3. Create P1: Code 120 bytes
4. Create P2: Code 180 bytes
5. Create P3: Code 90 bytes
6. Allocate all three
7. Display allocation table and free partitions

**Expected Output**:
- Each process allocated to best-fitting hole
- P1 allocated in 150-byte hole (at 100)
- P2 allocated in 200-byte hole (at 350)
- P3 allocated in 100-byte hole (at 550 or 0)
- Remaining holes correctly identified

**Pass Criteria**: Best-fit algorithm works correctly with fragmented memory

---

## Test Case 10: Deallocation of Middle Process
**Objective**: Test deallocation and hole creation within allocated memory

**Setup**:
- Total Memory: 900 bytes
- Holes: Address 0, Size 900

**Steps**:
1. Initialize system
2. Select First-Fit
3. Create P1: 1 segment (Code: 150)
4. Create P2: 1 segment (Code: 200)
5. Create P3: 1 segment (Code: 100)
6. Allocate all three (sequential: 0-150, 150-350, 350-450)
7. Display allocation table
8. Deallocate P2 (middle process)
9. Display allocation table and free partitions

**Expected Output**:
- After deallocation: P1 at 0-150, P3 at 350-450
- Free hole created at 150-350 (size 200)
- Another free hole at 450-900 (size 450)
- Holes NOT merged (non-adjacent)

**Pass Criteria**: Middle hole created correctly, non-adjacent holes not merged

---

## Test Case 11: Reallocation After Deallocation
**Objective**: Test re-allocating process after previous deallocation

**Setup** (continuing from Test Case 10):
- State: P1 and P3 allocated, P2 deallocated (holes at 150-350 and 450-900)

**Steps**:
1. From previous test state
2. Create P4: 1 segment (Code: 180)
3. Allocate P4
4. Display allocation table

**Expected Output**:
- P4 allocated in first available hole at address 150 (first-fit)
- Remaining hole: 330-350 (size 20) merged with 450-900 (after fill)
- Final state shows P1, P3, P4 allocated

**Pass Criteria**: Reallocation works correctly in freed space

---

## Test Case 12: Invalid Hole Input
**Objective**: Test validation of hole initialization

**Setup**:
- Total Memory: 1000 bytes

**Steps**:
1. Initialize system
2. Try to add invalid holes:
   - Hole 1: Start=500, Size=600 (extends beyond total memory)
   - Hole 2: Start=-50, Size=100 (negative address)
   - Hole 3: Start=0, Size=0 (zero size)

**Expected Output**:
- Error message: "[ERROR] Invalid hole: address must be >= 0 and address + size <= 1000"
- None of the holes added
- System remains unchanged

**Pass Criteria**: Invalid inputs rejected with clear error messages

---

## Test Case 13: Overlapping Holes Detection
**Objective**: Test detection of overlapping holes

**Setup**:
- Total Memory: 1000 bytes

**Steps**:
1. Initialize system
2. Add Hole 1: Start=0, Size=200
3. Add Hole 2: Start=150, Size=200 (overlaps with Hole 1)
4. Display free partitions

**Expected Output**:
- Error message for Hole 2: "[ERROR] Hole overlaps with existing partition!"
- Only Hole 1 added
- Hole 2 rejected

**Pass Criteria**: Overlap detection prevents invalid configuration

---

## Test Case 14: Process Segments Details Display
**Objective**: Test process segment information display

**Setup**:
- Total Memory: 1000 bytes
- Holes: Address 0, Size 1000

**Steps**:
1. Initialize system
2. Create P1 with segments:
   - Code: 150
   - Data: 200
   - Stack: 100
   - Heap: 50
3. Create P2 with segments:
   - Code: 120
   - Data: 80
4. Allocate P1
5. Display Process Segments Details

**Expected Output**:
- P1 displayed with total size 500
- All 4 segments listed with sizes and allocation status
- Code, Data, Stack allocated with start addresses
- Heap marked as "NOT ALLOCATED" (if space unavailable)
- P2 marked with all segments as "NOT ALLOCATED"

**Pass Criteria**: Segment details accurately display allocation status

---

## Test Case 15: Full Memory Scenario
**Objective**: Test system when memory is completely filled

**Setup**:
- Total Memory: 500 bytes

**Steps**:
1. Initialize system
2. Select First-Fit
3. Create P1: Code 200, Data 150, Stack 150 (total 500)
4. Allocate P1
5. Create P2: Code 50
6. Try to allocate P2
7. Display all information

**Expected Output**:
- P1 successfully allocated at 0-500
- P2 allocation fails with error
- Memory layout shows 100% occupied
- Free partitions shows "Total Free: 0 bytes"

**Pass Criteria**: Full memory handled correctly, new allocation rejected

---

## Test Case 16: Reinitialize System
**Objective**: Test system reinitialization

**Setup** (after multiple operations):
- Multiple processes allocated/deallocated

**Steps**:
1. Perform various operations
2. Select Reinitialize System (option 12)
3. Initialize new system with 2000 bytes
4. Add new holes
5. Create new processes

**Expected Output**:
- Previous state completely cleared
- New system starts fresh
- No remnants of previous operations

**Pass Criteria**: System reinitializes cleanly without data carryover

---

## Manual Testing Checklist

### Functionality Tests
- [ ] Initialize memory with valid size
- [ ] Add single hole
- [ ] Add multiple non-overlapping holes
- [ ] Reject invalid holes (negative, exceeding limits)
- [ ] Reject overlapping holes
- [ ] Create processes with multiple segments
- [ ] Select First-Fit algorithm
- [ ] Select Best-Fit algorithm
- [ ] Allocate process successfully
- [ ] Allocate process that doesn't fit
- [ ] Deallocate process
- [ ] Deallocate non-existent process
- [ ] Deallocate already-deallocated process
- [ ] View bar memory layout
- [ ] View ASCII memory layout
- [ ] View allocation table
- [ ] View free partitions
- [ ] View process segments
- [ ] View all information
- [ ] Reinitialize system
- [ ] Exit program

### Algorithm Tests
- [ ] First-Fit allocates to first suitable hole
- [ ] Best-Fit allocates to smallest suitable hole
- [ ] Different results between algorithms with same setup
- [ ] Both handle edge cases (exact fit, fragmented memory)

### Robustness Tests
- [ ] Handle processes larger than available memory
- [ ] Handle deallocating non-existent processes
- [ ] Handle invalid menu choices
- [ ] Handle negative/zero input sizes
- [ ] Handle empty process names
- [ ] Handle multiple sequential operations

### Output Quality Tests
- [ ] Error messages are clear
- [ ] Success messages confirm operations
- [ ] Memory visualizations are accurate
- [ ] Tables are properly formatted
- [ ] Addresses and sizes are correct
- [ ] Fragmentation is correctly handled

---

## Test Results Summary

Use this table to record test results:

| Test Case | Algorithm | Status | Notes |
|-----------|-----------|--------|-------|
| 1 | First-Fit | [ ] PASS [ ] FAIL | |
| 2 | Best-Fit | [ ] PASS [ ] FAIL | |
| 3 | N/A | [ ] PASS [ ] FAIL | |
| 4 | First-Fit | [ ] PASS [ ] FAIL | |
| 5 | First-Fit | [ ] PASS [ ] FAIL | |
| 6 | Both | [ ] PASS [ ] FAIL | |
| 7 | N/A | [ ] PASS [ ] FAIL | |
| 8 | N/A | [ ] PASS [ ] FAIL | |
| 9 | Best-Fit | [ ] PASS [ ] FAIL | |
| 10 | First-Fit | [ ] PASS [ ] FAIL | |
| 11 | First-Fit | [ ] PASS [ ] FAIL | |
| 12 | N/A | [ ] PASS [ ] FAIL | |
| 13 | N/A | [ ] PASS [ ] FAIL | |
| 14 | N/A | [ ] PASS [ ] FAIL | |
| 15 | First-Fit | [ ] PASS [ ] FAIL | |
| 16 | N/A | [ ] PASS [ ] FAIL | |

---

## Known Testing Considerations

1. **Memory Addresses**: All addresses start at 0
2. **Segment Allocation**: Each segment is allocated contiguously
3. **Hole Merging**: Only adjacent free holes merge
4. **Algorithm Differences**: Most noticeable with fragmented memory
5. **Process Uniqueness**: Process names should be unique for clarity
6. **Deallocation**: Deallocating a process marks all its segments as free

---

## Additional Verification Notes

- Ensure addresses never overlap between allocated partitions
- Verify total allocated + total free = total memory
- Check that algorithms behave as documented
- Confirm error messages appear for invalid inputs
- Validate that menu navigation works smoothly
