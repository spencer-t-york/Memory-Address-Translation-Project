# TODO: Need to implement Reverse Mapping

# Memory Partitioning: Part 2

In this part, you will design the page table and handle allocations of physical frames on the same machine as discussed above. You will create two new source files: `phyframe.c` and `pagetable.c`, as well as a new main program, named `mempart2.c`, plus any necessary header files. Here:

- **`phyframe.c`** is used to manage the physical frames.
- **`pagetable.c`** will hold all functions that manage the page table for the process.

## Physical Frame Management

For this part, we will assume:

- The **first physical frame** is reserved for the **OS**.
- Other frames are initially available for the process.

To manage physical frames, you will use a simple physical page allocation scheme:

- **Allocate each physical page** in the order of frame numbers, starting from 1, 2, 3, ...
- If physical frames are available, always allocate from these available frames first.
- When no free physical frames are left, use the **LRU (Least Recently Used) page replacement policy** to choose a physical frame to replace. This means the **least recently used page** will be the first to be evicted.

### Important Requirements

1. **Update the Old Page Table Entry**  
   Ensure that **no two virtual pages** are pointing to the same physical frame. In real systems, it’s best to quickly locate which page table entry points to a physical frame, a process called **reverse mapping**. However, for simplicity, you may search the entire page table of all active processes (in this assignment, only one process), even if it incurs a performance overhead.

   **Note**: You must design and implement a reverse mapping scheme. Failing to do so will result in an **automatic 15-point deduction**.

2. **Modify the Target Page Table Entry**  
   Change the page table entry of the target page to point to the frame, and set up the reverse mapping correctly.

### LRU Policy Implementation

- You may use any mechanism discussed in class to implement the strict LRU policy, such as a **counter mechanism**, but **not the second-chance algorithm**.
- Ensure that the corresponding information is updated **upon each access**.

## Program Input and Output

The input sequence for this program is the same as that used in **Part 1**. This allows you to:

1. **Utilize the same parser** to obtain the corresponding virtual address.
2. **Translate the virtual address to a physical address** using your implemented page table.

At the end, you can use the same function from **Part 1** to output the translated physical address sequence to an output file.

- You are provided with a `part2` input file named `part2sequence`.
- Similar to Part 1, report the **md5sum** of your output file by writing it to **`p2result.txt`**.
- Additionally, your program should report the **number of page faults** encountered for the given access sequence, which should also be recorded in `p2result.txt`.

# To Run the Program
- Enter the following commands:
   -  `gcc mempart2.c phyframe.c pagetable.c -o mempart2`
   -  `./mempart2 part2sequence`

- The output will show the number of page faults and generate a `part2out` file

# Notes
### Program Process
- Check if the `virtualPageNumber` from the logical address is in the `invertedPageTable`:
  - If yes:
    - The index of the `virtualPageNumber` in the `invertedPageTable` is the `physicalFrameNumber`.
    - Update the `LRUstack` to move the `virtualPageNumber` to the top (as most recent).
    - Return the index of the `virtualPageNumber` in the `invertedPageTable` as the `physicalFrameNumber`.
  - If no:
    - Increment the `pageFaultCounter`.
    - Check if there is an open spot in the `invertedPageTable`:
      - If yes:
        - Store the `virtualPageNumber` in that open index.
        - Update the `LRUstack` to push the `virtualPageNumber` to the top (as most recent).
        - Return the index of the `virtualPageNumber` in the `invertedPageTable` as the `physicalFrameNumber`.
      - If no:
        - Consult the `LRUstack` to find the `virtualPageNumber` in the bottom-most element of the stack.
        - Replace this element in the `invertedPageTable` with the new `virtualPageNumber` from the logical address.
        - Update the `LRUstack` by removing the old `virtualPageNumber` and pushing the new one to the top (as most recent).
        - Return the index of the `virtualPageNumber` in the `invertedPageTable` as the `physicalFrameNumber`.

- Perform the translation using the formula:  
  `physical_address = physicalFrameNumber * 128 + offset;`


### LRU Policy
- We are using the stack implementation of the LRU policy
    - Using an array so the last element is easier/faster to access
    - When a page number is accessed (whether it is currently in the stack or not, it is moved to the top of the stack.
    - At any point, the least recently used page number is at the bottom of the stack. This element will be replaced
