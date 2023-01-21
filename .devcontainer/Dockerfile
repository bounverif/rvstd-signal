# Follow commands below to build and run the container
#
# > docker build . -t bouncmpe/rvstd:latest
# > docker run -it --rm -v $(PWD):/workspaces/rvstd bouncmpe/rvstd:latest

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
        python-is-python3 \
    && ln -s /usr/bin/python3 /usr/bin/python \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 
   
RUN git clone -j$(nproc) --recursive --depth 1 --branch boost-1.81.0 https://github.com/boostorg/boost.git /tmp/boost \
    && cd /tmp/boost \
    && ./bootstrap.sh --prefix=/usr/local --without-libraries=python \
    && ./b2 -j$(nproc) install \
    && cd / && rm -rf /tmp/boost

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        clang \
        clangd \
        lcov \
        valgrind \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN git clone --depth 1 --branch v3.2.1 https://github.com/catchorg/Catch2.git /tmp/catch2 \
    && cmake -S/tmp/catch2 -B/tmp/catch2/build \
    -DBUILD_TESTING=OFF \
    -DCATCH_INSTALL_DOCS=OFF \
    && cmake --build /tmp/catch2/build/ --target install -j$(nproc) \
    && rm -rf /tmp/catch2
    
RUN git clone --depth 1 --branch v1.7.1 https://github.com/google/benchmark.git /tmp/benchmark \
    && cmake -S/tmp/benchmark -B/tmp/benchmark/build \
    -DBENCHMARK_ENABLE_TESTING=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /tmp/benchmark/build/ --target install -j$(nproc) \
    && rm -rf /tmp/benchmark

# Add a container user (good practice)
RUN groupadd work -g 1000 \
   && useradd -ms /bin/bash bouncmpe -g 1000 -u 1000 \
   && printf "bouncmpe:bouncmpe" | chpasswd \
   && printf "bouncmpe ALL= NOPASSWD: ALL\\n" >> /etc/sudoers

USER bouncmpe
