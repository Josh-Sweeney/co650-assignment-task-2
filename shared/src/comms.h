#include <winsock2.h>

#include <iostream>

// TODO: Supposed to be abstract
static class Comms {
    public:
       static int initializeWinsock();
};