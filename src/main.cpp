/*
struct sockaddr_in {
    sa_family_t sin_family;     // Famille d'adresse (AF_INET pour IPv4)
    in_port_t sin_port;         // Numéro de port (en réseau byte order)
    struct in_addr sin_addr;    // Adresse IP en format binaire
    char sin_zero[8];           // Remplissage pour le même alignement que sockaddr
};
*/


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <cstdlib>
#include <signal.h>
#include "../include/Serveur.hpp"

Serveur server;

void    handle_sigint(int sig) {
    server.closeServer();
    (void) sig;
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Error number arguments !" << std::endl;
        return (0);
    }
    Serveur server = Serveur(std::atoi(argv[1]), argv[2]);

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Erreur lors de l'installation du gestionnaire SIGINT");
        exit(1);
    }
    server.run();
    return 0;
}


/*
    15:40:19:   <--{4}[MODE aaaa +i
    15:40:19:   {4}[:aaaa!sbelabba@127.0.0.1 221 aaaa +i
    15:40:22:   <--{4}
*/
