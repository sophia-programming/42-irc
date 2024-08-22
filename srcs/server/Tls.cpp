#include "Server.hpp"

WOLFSSL_CTX* createContext() {
    WOLFSSL_CTX* ctx;
    ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());
    if (ctx == NULL) {
        std::cerr << "wolfSSL_CTX_new error." << std::endl;
        exit(EXIT_FAILURE);
    }
    return ctx;
}