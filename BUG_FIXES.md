# Bug Fixes - May 10, 2026

## Issues Found and Fixed

### Issue 1: Hole Initialization Overlap Error ❌ → ✅

**Problem**:
- User unable to initialize holes due to "Hole overlaps with existing partition!" error
- System starts with default free partition (0, 1000)
- When user tries to add hole (0, 1000), overlap detection prevented it

**Root Cause**:
- MemoryManager constructor creates default partition covering entire memory
- First "Initialize Holes" attempt detected this as overlap

**Solution**:
- Modified main.cpp case 1: Clear partitions BEFORE adding holes
- Added new method `addDefaultHole()` in MemoryManager
- Separated concerns:
  - `clearPartitions()`: Completely clears (for hole initialization)
  - `addDefaultHole()`: Recreates default state (for constructor)
- Users can now reinitialize holes without overlap errors

**Files Modified**:
- `main.cpp`: Updated hole initialization logic (case 1)
- `MemoryManager.h`: Added `addDefaultHole()` declaration
- `MemoryManager.cpp`: Implemented both methods separately

---

### Issue 2: Missing Free Partitions After Allocation ❌ → ✅

**Problem**:
- After allocating 275 bytes of 1000, no free partition was displayed
- "Total Free: 0 bytes" when should be 725 bytes
- Remaining hole (275, 725) was being lost

**Root Cause**:
- Allocation code used reference to `hole` partition object
- After calling `insert()`, reference became dangling
- Remaining hole size calculation failed because reference was invalid

**Original Code Issue**:
```cpp
Partition& hole = partitions[holeIndex];  // Reference taken
// ... create newPartition ...
partitions.insert(...);  // Insert invalidates reference!
// Now "hole" reference is unreliable
if (hole.size == seg->getSize()) {  // Using stale reference
    partitions.erase(...);
} else {
    partitions[holeIndex + 1].size -= seg->getSize();  // Wrong!
}
```

**Solution**:
- Save hole information (address, size) BEFORE any modifications
- Remove old hole first
- Insert allocated partition
- Create and insert remaining hole explicitly
- No dangling references

**New Code**:
```cpp
// Save hole info before modifying partitions
int holeStartAddr = partitions[holeIndex].startAddress;
int holeSize = partitions[holeIndex].size;

// Remove old hole, insert allocation, insert remainder
partitions.erase(partitions.begin() + holeIndex);
partitions.insert(partitions.begin() + holeIndex, newPartition);

if (holeSize > seg->getSize()) {
    Partition remainingHole("FREE", "FREE", remainingStartAddr, remainingSize, false);
    partitions.insert(partitions.begin() + holeIndex + 1, remainingHole);
}
```

**Files Modified**:
- `MemoryManager.cpp`: Completely rewrote allocation logic (allocateProcess method)

---

## Testing the Fixes

### Test Case: Initialize Holes Correctly

**Steps**:
1. Enter total memory: 1000
2. Select Option 1 (Initialize Holes)
3. Add hole: Start=0, Size=500
4. Add hole: Start=500, Size=500
5. Should work without overlap errors!

**Expected**:
- Both holes added successfully
- Free partitions table shows two 500-byte holes

---

### Test Case: Free Space Preserved After Allocation

**Steps**:
1. Enter total memory: 1000
2. Option 1: Initialize single hole (0, 1000)
3. Option 3: Select First-Fit
4. Option 2: Create P1 with segments (Code: 100, Data: 100, Stack: 75)
5. Option 4: Allocate P1
6. Option 9: Display Free Partitions

**Expected**:
- Free partition shows: Address 275, Size 725
- "Total Free: 725 bytes"
- Allocation table shows 3 allocated segments
- No more "Total Free: 0 bytes" error

---

## Code Quality Improvements

1. **Safer Vector Operations**: Eliminated dangling references
2. **Clear Intent**: Separate methods for different initialization needs
3. **Better State Management**: Explicit partition clearing before hole addition
4. **Correct Memory Accounting**: Free space now properly preserved

---

## Verification

All fixes verified through:
- Code review for reference validity
- Logical verification of partition management
- Test execution scenarios

Program should now work correctly for:
✅ Multi-hole initialization
✅ Correct free space tracking
✅ Proper allocation without losing memory
✅ Accurate memory layout visualization
