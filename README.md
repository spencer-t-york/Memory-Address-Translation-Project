In this part, you will design the page table and deal with allocations of physical machines on the same machine as discussed above. You will create two new source files: `phyframe.c` and `pagetable.c`, as well as a new main program, named `mempart2.c`, plus any necessary header files. 

- `phyframe.c` is used to manage the physical frames
- `pagetable.c` will hold all functions that manage the page table for the process.

For this part, we will also assume the first physical frame is reserved for the OS, while other frames are initially available for the process. 
To manage physical frames, you will use a simple physical page allocation scheme: You will allocate each physical page in the order of frame number initially, starting from 1, 2, 3, .... If physical frames are available, you will always allocate from these available frames at first. Once there are no free physical frames, you will need to use LRU page replacement policy to choose a physical frame to be replaced, which means the least recently used page will be the first to be evicted.

First, you should update the old page table entry such that no two virtual pages are pointing to the same physical frame. In reality, it is better to quickly locate which page table entry is actually pointing to this physical frame, typically called as "reverse mapping". However, you may search the whole page table of all active processes (one process in the assignment) to find out this, with significant performance overhead. You must design and implement a reverse mapping scheme. Otherwise, you will automatically lose 15 points.
Second, you should change the page table entry of the target page to point to the frame. Note that you should also set up the reverse mapping correctly.

Note: you can use any mechanism discussed in the class to implement the strict LRU policy, such as counter mechanism, but not second-chance algorithm. Importantly, you should update the corresponding information upon each access.

The input sequence of the program is the same as that used in Part 1. Thus, you should be able to utilize the same parser to obtain the corresponding virtual address, then translate it to a physical address with your implemented page table.

In the end, you can use the same function as in Part1 to output the translated physical address sequence into an output file. You are provided a part2 input file,    part2sequence    Click for more options . Similar to Part1, you only need to report the md5sum of your output file. and write it to p2result.txt. In addition, your program should report the number of page faults encountered for the given access sequence, and this number should be reported in p2result.txt file too.
