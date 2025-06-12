# ZSet: Sorted Set Implementation in C++

## 📌 Overview

**ZSet** is a custom C++ implementation of a sorted set data structure that allows:
- Fast insertions and deletions.
- Efficient score-based queries with lexicographical tie-breakers.
- Offset-based pagination.

It combines:
- An **AVL Tree** (augmented with node count and height for rank-based traversal),
- A **Hash Map** (for fast lookup and deduplication),
to achieve both fast access and ordered traversal.

---

## 🔧 Features

- `zset_insert(name, len, score)`  
  Inserts or updates a key with the given score.

-  `zset_lookup(name, len)`  
  Retrieves an element by name.

-  `zset_delete(node)`  
  Removes a node from both tree and hash map.

-  `zset_seekge(score, name)`  
  Finds the first node with score ≥ given score and name ≥ given name.

-  `do_zquery(name, score, offset, limit)`  
  Returns a string containing `limit` elements starting from the element ≥ `score` and `name`, offset by `offset`.

---

## ⚙️ Data Structures

### ZNode

Each element in the ZSet:

```cpp
struct ZNode {
    HNode* hmap;         // Hash map node
    AVLNode* root;       // AVL tree node
    double score;        // Sort key
    size_t len;
    char name[];         // Flexible array member
};
