#pragma once
#include <iostream>
#include <sstream>

//#define VERBOSE

class LogDebug {
    bool s_actif;
public:  

    LogDebug(){}

    // Surcharge de l'opérateur <<
    template <typename T>
    LogDebug& operator<<(const T& value) {
//#ifdef _DEBUG
        if (s_actif) {
            std::cout << value;
        }
//#endif
        return *this;
    }

    // Manipulateurs comme std::endl
    LogDebug& operator<<(std::ostream& (*manip)(std::ostream&)) {
//#ifdef _DEBUG
        if (s_actif) {
            manip(std::cout);          // Applique le manipulateur (e.g. std::endl)
        }
//#endif        
        return *this;
    }
    
    void setActif(bool a) {
        s_actif = a;
    }
};

class LogWarning {
public:
    LogWarning() {}

    // Surcharge de l'opérateur <<
    template <typename T>
    LogWarning& operator<<(const T& value) {
        std::cout << value;
        return *this;
    }

    // Manipulateurs comme std::endl
    LogWarning& operator<<(std::ostream& (*manip)(std::ostream&)) {
        manip(std::cout);          // Applique le manipulateur (e.g. std::endl)
        return *this;
    }

private:
    std::ostringstream buffer;    // Buffer pour stocker les messages
};

// Instance globale des loggers
extern LogDebug LOGDEBUG;
extern LogWarning LOGWARNING;
