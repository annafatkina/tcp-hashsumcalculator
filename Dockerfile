FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# install dependencies
RUN apt-get update && apt-get install -y git gcc g++ libboost-all-dev libgtest-dev cmake make 

# clone sources
RUN  git clone https://github.com/annafatkina/tcp-hashsumcalculator.git

# build sources
RUN cd tcp-hashsumcalculator && git checkout dev && mkdir build && \
    cd build && cmake ../ && make -j

# run tests
CMD cd tcp-hashsumcalculator/build/tests/ && ./tests
