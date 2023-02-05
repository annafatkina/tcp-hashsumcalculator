FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# install dependencies
RUN apt-get update && apt-get install -y git gcc g++ libboost-all-dev libgtest-dev cmake make netcat

# clone sources
RUN  git clone https://github.com/annafatkina/tcp-hashsumcalculator.git

# build sources
RUN cd tcp-hashsumcalculator && mkdir build && \
    cd build && cmake ../ && make -j

# run tests
CMD cd tcp-hashsumcalculator/build/ && make test && cd ../ && . run.sh 3 && cat out0.tmp
