//
// Created by System Administrator on 9/29/16.
//

#ifndef CIT_MONOMAKE_PORTAPI_H
#define CIT_MONOMAKE_PORTAPI_H

#include <string>

using namespace std;

class PortAPI {
public:
    static string getService(int port);
};

#endif //CIT_MONOMAKE_PORTAPI_H
