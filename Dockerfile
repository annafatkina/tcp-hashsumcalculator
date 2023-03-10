FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# install dependencies
RUN apt-get update && apt-get install -y git gcc g++ libboost-all-dev \ 
    libgtest-dev cmake make netcat psmisc libssl-dev

# clone sources
RUN  git clone https://github.com/annafatkina/tcp-hashsumcalculator.git

# build sources
RUN cd tcp-hashsumcalculator && mkdir build && \
    cd build && cmake ../ && make -j

# run tests
CMD cd tcp-hashsumcalculator/build/ && ctest --verbose && cd ../ && \
    chmod +x run.sh && ./run.sh 3
