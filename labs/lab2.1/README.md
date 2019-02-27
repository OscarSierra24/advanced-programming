# Lab 2.1 - Clocks

clockwall.go serves as a client that is listening the server clock2.go. 
The idea is simple, clock2.go stores timezones and writes them to their clients whereas clockwall listents what clock2 has to say.

## Getting Started

You can test this program by building both go files with:
go build clockWall.go
go build clock2.go

Then, test the program with the following input

First, run the server the amount of times that you wish (following this structure):

```
TZ={timezone} {build} -port {port}
```

Note: currently, the flag port is harcoded

This is an example of expected input to init clock servers:

```
TZ=US/Eastern    ./clock2 -port 8010 &
TZ=Asia/Tokyo    ./clock2 -port 8020 &
TZ=Europe/London ./clock2 -port 8030 &
```

Then, you can run the clockwall client by typing the following:

```
./clockWall NewYork=localhost:8010 Tokyo=localhost:8020 London=localhost:8030
```

### Prerequisites

```
Go
```
