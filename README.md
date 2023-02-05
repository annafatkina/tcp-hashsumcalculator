# Tcp hashsum calculator

TCP server for hashsum calculation. Clients connect to this server and send 
strings devided by '\n' symbol. For each string hashsum is calculated, and its 
hex representation to be received as a response. Hashsum is calculated with 
'std::hash' which guarantees that the collision probability is 
'1/max(size\_t)'.

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

This will run unit tests and print its output to command line.

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
./tests/tests
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