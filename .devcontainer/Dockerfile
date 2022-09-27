# Follow commands below to build and run the container
#
# > docker build . -t bouncmpe/jsonmerge:latest
# > docker run -it --rm -v $(PWD):/workspaces/project bouncmpe/jsonmerge

FROM ubuntu:22.04

RUN apt-get update && apt-get -y install \
        sudo \
        git \
        build-essential \
        clang \
        clangd \
        cmake \
    && apt-get autoremove -y && apt-get clean -y

