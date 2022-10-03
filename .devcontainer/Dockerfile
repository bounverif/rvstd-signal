# Follow commands below to build and run the container
#
# > docker build . -t bouncmpe/jsonmerge:latest
# > docker run -it --rm -v $(PWD):/workspaces/project bouncmpe/jsonmerge

FROM ubuntu:22.04

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        sudo \
        git \
        build-essential \
        cmake \
        ninja-build \
        python3-all-dev \
        python3-pip \
    && ln -s /usr/bin/python3 /usr/bin/python \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 
   
RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-1.80.0 https://github.com/boostorg/boost.git /tmp/boost \
    && cd /tmp/boost \
    && ./bootstrap.sh --prefix=/usr/local --without-libraries=python \
    && ./b2 -j$(nproc) install \
    && cd / && rm -rf /tmp/boost

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        clang \
        clangd \
        clang-format \
        clang-tidy \
        lcov \
        valgrind \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN git clone --depth 1 --branch v3.1.0 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2
    
RUN git clone --depth 1 --branch v1.7.0 https://github.com/google/benchmark.git /tmp/benchmark \
    && cmake -S/tmp/benchmark -B/tmp/benchmark/build \
    -DBENCHMARK_ENABLE_TESTING=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /tmp/benchmark/build/ --target install -j$(nproc) \
    && rm -rf /tmp/benchmark
    
WORKDIR /workspaces/project

COPY . /workspaces/project
