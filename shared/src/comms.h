#include <winsock2.h>

// TODO: Supposed to be abstract
class Comms {
    public:
        SOCKET createSocket();
};