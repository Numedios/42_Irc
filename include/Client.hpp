#ifndef DEF_CLIENTS
#define DEF_CLIENTS

#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <string>
#include <algorithm>


//class Channel;

class Client {
public:
    Client(int socket, int id) : _id(id), _socket(socket), _isConnected(true), _authenticated(false), _pendingCommand(""), _currentChannel(""), _nick(""), _userName(""), _ip(""), _authState(NotAuthenticated) {}
    
    void printInfoClient();
    std::string returnPrefixe();

    enum AuthState {
        NotAuthenticated,
        CapLs,
        Pass,
        Nick,
        User,
        Authenticated
    };

    bool operator==(const Client& other) const {
        return _id == other._id;
    }

    int getId() const {
        return _id;
    }

    int getSocket() const {
        return _socket;
    }

    AuthState getAuthState() const {
        return _authState;
    }
    
    std::string getNick() const {
        return _nick;
    }

    bool isConnected() const {
        return _isConnected;
    }

    bool isAuthenticated() const {
        return _authenticated;
    }


    std::string getPendingCommand() const {
        return _pendingCommand;
    }

    void setId(int id)
    {
        _id = id;
    }

    void setDisconnected() {
        _isConnected = false;
    }

    void setAuthenticated(bool value) {
        _authenticated = value;
    }

    void setPendingCommand(const std::string& command) {
        _pendingCommand = command;
    }

    void setNick(const std::string& nick) {
        _nick = nick;
    }

    void setUserName(const std::string& userName) {
        _userName = userName;
    }

    void setAuthState(AuthState state) {
        _authState = state;
    }

private:
    int _id;
    int _socket;          // Le socket du client
    bool _isConnected;    // Indique si le client est connecté ou déconnecté
    bool _authenticated;  // Indique si le client est authentifié
    std::string _pendingCommand; // Commande non terminer en attente d'un caractere de fin \n
    std::string _currentChannel;  // Le canal courant du client
    std::string _nick;     // Le nom d'utilisateur (nick) du client
    std::string _userName; // username utile pour le prefixe des message
    std::string _ip;       // L'adresse IP du client
    AuthState _authState;
};

#endif