cmake_minimum_required(VERSION 3.28)

project(Cereberus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cereberus
Backend/AVLTreeImp/Tree.cpp
Backend/HashTableImp/Hash.cpp
Backend/SortedSet/SoSet.cpp
Networking/CommandImp/CommAbs.cpp
Networking/server.cpp
linux_main.cpp
)
