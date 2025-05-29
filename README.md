# Deque README


## 项目概述

<img src="https://www.oreilly.com/api/v2/epubs/9781787120952/files/assets/fd7f0c6e-e5cb-400d-ad2f-c38e91772682.png" width="500">

使用分块链表（Unrolled Linked List）数据结构实现高效的双端队列（Deque），支持快速随机访问与动态容量调整。
## 核心要求

分块链表就是对存储元素进行分块，以加速随机访问等操作。理想情况下，每一块的大小在 $O(\sqrt n)$ 量级。一种保证方法是在块过大时分成两块，相邻两块均很小时合成一块。在具体实现中，每一
块内部的储存方式和所有块的储存方式均可使用链表或数组。 你需要保证头尾插入和删除的均摊
复杂度是 $O(1)$，随机插入、删除和查询的复杂度是最坏 $O(\sqrt n)$。

实现方式：链表套循环数组，效率会更高一些。**注：实现包括动态调整块长的策略，未固定块长**

## 项目结构

```
.
├── tests/
│   ├── one/             # 功能测试用例
│   ├── two/      
│   ├── three/      
│   ├── four/      
│   ├── two.memcheck/    # 内存检查专用测试
│   └── four.memcheck/    
├── (various utility hpp files...)
├── README.md            # 给deque写的文档
└── deque.hpp            # deque实现
```

## Strategy Explanation for Block Splitting and Merging

In this project, I have implemented a **deque** (double-ended queue) using an **Unrolled Linked List** (ULL) as the underlying data structure. This structure consists of blocks, each holding multiple elements, and we employ strategies for **block splitting** and **block merging** to manage the size and organization of these blocks efficiently.

### 1. **Block Splitting**

**Splitting** occurs when a block is almost full and we need to make space for additional elements. The strategy ensures that each block does not exceed its capacity, allowing for efficient memory management and fast access times.

- **Triggering Condition:**  
  When the number of elements in a block exceeds its capacity by more than a certain threshold (i.e., if the block is nearly full), we split it into two smaller blocks.  
  The block is split at the midpoint of its current size, with the first half of the elements retained in the original block and the second half moved to a newly created block.
  
- **Complexity:**  
  The time complexity of splitting a block is **O(n)**, where **n** is the number of elements in the block, as we need to transfer elements from the original block to the new block. However, this operation happens infrequently (only when a block is full), so the amortized cost is kept low.

- **Why This Works Efficiently:**  
  By keeping block sizes smaller and balanced (around half-full), we ensure that the cost of splitting remains minimal. This also helps in maintaining fast access times (random access in **O(√n)**).

### 2. **Block Merging**

**Merging** occurs when two adjacent blocks become too small and need to be combined to avoid excessive fragmentation. This ensures that we do not waste memory by leaving too many under-utilized blocks.

- **Triggering Condition:**  
  When the number of elements in a block is smaller than **√n / 2** (where **n** is the total number of elements in the deque), and it is adjacent to another small block, we merge the two blocks. This condition ensures that blocks are not too small, preventing excessive fragmentation and helping maintain efficient memory usage.


- **Complexity:**  
  The time complexity of merging two blocks is **O(n)**, where **n** is the total number of elements in the two blocks being merged. The operation involves transferring elements from both blocks into a new block and updating the pointers in the linked list to reflect the change. The amortized cost remains low due to infrequent merges.

- **Why This Works Efficiently:**  
  Merging helps to prevent memory fragmentation, ensuring that we always have blocks with a reasonable number of elements. This strategy helps maintain **O(√n)** access times and keeps memory usage efficient, even as elements are added or removed.

### 3. **Dynamic Block Size Adjustment**

The block size is dynamically adjusted to optimize the performance of both the **split** and **merge** operations. The **default block size** is set to **350** elements, but this can grow or shrink depending on the number of elements in the blocks.

- **Splitting Factor:**  
  When splitting a block, we increase the block size by a factor, which ensures that the block grows incrementally to maintain balance.
  
- **Merging Factor:**  
  When merging two blocks, the size of the new block is typically set to double the size of the original blocks. This prevents frequent splits and merges, maintaining a balance between the number of blocks and the number of elements stored.

### 4. **Time Complexity Justification**

- **Amortized Complexity of Insertions and Deletions:**  
  Both **insertions** and **deletions** at the ends (head or tail) of the deque are performed in **O(1)** amortized time due to the use of block-level management. Even though **splits** and **merges** may occur during insertions or deletions, these operations are infrequent and are thus amortized over multiple operations.

- **Random Access:**  
  Accessing an element at any arbitrary position in the deque is done in **O(√n)** time due to the **unrolled linked list** structure. Each block holds a number of elements, and since the blocks are kept at an optimal size, finding an element is a **sqrt(n)** operation. The overall complexity for accessing an element is therefore very efficient.

- **Space Efficiency:**  
  The **block size adjustment** strategies (splitting and merging) help ensure that memory is used efficiently. Blocks are not too large to cause memory waste, nor are they too small to result in excessive overhead from splitting and merging operations.

## Conclusion

The **split and merge** strategy in the **Unrolled Linked List** structure helps us achieve optimal performance for the deque operations. By dynamically adjusting block sizes and merging small blocks, we can ensure that both **random access** and **amortized insertion/deletion** remain efficient and scalable. The design also balances between space efficiency and fast access, making it suitable for scenarios where both performance and memory management are crucial.

