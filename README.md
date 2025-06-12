# Cereberus (Custom In-Memory DB) - Synopsis

This project is a simplified Redis-like in-memory database implemented in C++, supporting both **HashMap** and **ZSet (Sorted Set using AVL Trees)** data types. The server listens to commands from a socket, processes them, and sends appropriate responses.

---
## How to RUN IT
```
# 1. Create build directory
mkdir build && cd build

# 2. Generate build files
cmake ..

# 3. Compile
cmake --build .

#4,Run program for linux.Windows coming soon.
./Cereberus
```


## Architecture Overview

* **HashMap DBs**: Stored in a `maps` structure (`std::unordered_map<std::string, HMap>`).
* **ZSet DBs**: Stored in `mapZset` using an AVL tree for score-ordered insertion.
* **Communication**: Socket-based client-server system. Input commands are parsed, responses are written back using `write_to_socket`.

---

## Data Types

### 1. `HMap` (Hashtable)

Supports:

* `new`: create DB
* `put`: insert key-value
* `get`: retrieve value
* `del`: delete key

### 2. `ZSet` (Sorted Set)

Supports:

* `zsetCreate`: create DB
* `zsetPut`: insert element with score
* `zsetQuery`: get elements in a score range
* `zsetDel`: delete element by key

---

## Commands & Usage

Each command follows a structure:

```
<command> <DB_NAME> [key] [value/score] ...
```

### `new`

**Description**: Creates a new empty key-value DB.

```
Usage:
new myDB
```

**Response**:

```
built successfully
```

---

### `put`

**Description**: Insert a key-value pair into a HashMap DB.

```
Usage:
put myDB user1 JohnDoe
```

---

### `get`

**Description**: Retrieve a value by key.

```
Usage:
get myDB user1
```
---

### `del`

**Description**: Delete a key-value pair.

```
Usage:
del myDB user1
```

### `zsetCreate`

**Description**: Creates a new ZSet database (AVL tree).

```
Usage:
zsetCreate myZDB
```

### `zsetPut`

**Description**: Inserts a value with score into ZSet. Auto-creates DB if missing.

```
Usage:
zsetPut myZDB alice 3.5
```
### `zsetQuery`

**Description**: Queries ZSet elements by score range.

```
Usage:
zsetQuery myZDB dummyKey 2.0 5.0
```

### `zsetDel`

**Description**: Deletes a key from the ZSet.

```
Usage:
zsetDel myZDB alice
```

## Logging

All commands are logged with timestamps using `fprintf()`:

```
[command] at time: YYYY-MM-DD HH:MM:SS
```

Example:

```
put command at time: 2025-06-12 14:32:00
```

---

## Further Features coming

* Instead of using AVL use B+ trees.
* More Secure socket communications
* Bringing it to Windows
