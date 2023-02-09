# Tcp hashsum calculator

TCP server for hashsum calculation. Clients connect to this server and send 
strings devided by '\n' symbol. For each string hashsum is calculated, and its 
hex representation to be received as a response. Hashsum is calculated with 
`sha256` algorithm from `openssl` by chunks.

When client connects to server, the new session is opened for this client. 
Client can send messagies as long as his buffer size allows it, without 
wainting a respone. Server sends responses in the same order that requests 
were received. Single client requests are processes consequentially, different 
clients requests can be processed in parallel.

## Build instruction

Before building, download the repo:

```
git clone https://github.com/annafatkina/tcp-hashsumcalculator.git
```

### Docker

```
cd <repo/dir>
docker build -t tcpserver . 
docker run tcpserver
```

This will run unit tests and print its output to command line, and then run
tcp server with multiple clients.

### Locally

```
cd <repo/dir>
mkdir build
cd build
cmake ../
make -j
```

## Run

### Tests

To run tests, build the project and the run the following from the build 
directory:

```
ctest --verbose
```
or

```
./tests/hasher
./tests/session
./tests/tcpserver
```

### Server

To run server:

```
cd <build/directory>/src
./tcp_server <port>
```

Then add some clients, e.g. with netcat. You can install it with 
`sudo apt-get install netcat` command. Then run the client.

```
nc localhost <port>
```

And write any string ends with line break there.

OR simply run 
```
. run.sh <n>
```
from the repo directory. This will run the tcp server and `<n>` netcat
clients. Each client sends strings from `teststrings.txt` to server. Note that 
this scripts kills `nc` and `tcp_server` processes in the end.

