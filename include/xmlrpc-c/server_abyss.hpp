#ifndef SERVER_ABYSS_HPP_INCLUDED
#define SERVER_ABYSS_HPP_INCLUDED
#include "xmlrpc-c/base.hpp"
#include "abyss.h"

namespace xmlrpc_c {

class server_abyss {
    
public:
server_abyss::server_abyss(
    xmlrpc_c::registry const& registry,
    unsigned int       const  portNumber = 8080,
    string             const& logFileName = NULL,
    unsigned int       const  keepaliveTimeout = 0,
    unsigned int       const  keepaliveMaxConn = 0,
    unsigned int       const  timeout = 0,
    bool               const  dontAdvertise = false
    );
    ~server_abyss();
    
    void run();
    
private:
    // We rely on the creator to keep the registry object around as
    // long as the server object is, so that this pointer is valid.
    // We need to use some kind of automatic handle instead.
    
    const xmlrpc_c::registry * registryP;
    
    string       configFileName;
    string       logFileName;
    unsigned int portNumber;
    unsigned int keepaliveTimeout;
    unsigned int keepaliveMaxConn;
    unsigned int timeout;
    bool         dontAdvertise;
};


void
server_abyss_set_handlers(TServer *          const  srvP,
                          xmlrpc_c::registry const& registry);

} // namespace

#endif
