# RVSTD

RVSTD is a C++17 header-only library which provides time efficient data structures for interval sets and interval maps.
It also provides union, intersection, difference, and negation operations for interval sets and minimum, maximum, and
minus operations for interval maps.

###  Downloading 

RVSTD can be downloanded by git commond. It is only depended with C++ 17 STL.

```
git clone https://github.com/boundrivesim/rvstd
```

###  Running Unit Tests

Unit tests are developed by using Catch2 library. It can be downloaded from [Catch2](https://github.com/catchorg/Catch2.git).
Also to run tests gcc and cmake should be in the system. 

```
sudo apt-get install cmake
```

The unit tests can be run in tests file with cmake

```
cmake
make
ctest
```


###  Running Benchmark Tests

Benchmark tests are also developed by using Catch2 library. It can be downloaded from [Catch2](https://github.com/catchorg/Catch2.git). 
For comparison, the boost library is used in benchmark tests. It can be downloaded from [Boost](https://github.com/boostorg/boost.git).
Also to run tests gcc and cmake should be in the system. 

```
sudo apt-get install cmake
```

The unit tests can be run in benchmark/catch2 file with cmake

```
cmake
make
ctest
```

###  Docker

The test environment is automized by dockerfile. It can be used only with running [DockerFile](https://github.com/boundrivesim/rvstd/blob/main/.devcontainer/Dockerfile)

###  Liscence

Mozilla Public License Version 2.0
