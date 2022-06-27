#include "utils.h"
#include <string.h>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

string hexToBin(char hexChar) {
    // check if character contains valid hex value
    if (hexChar >= '0' && hexChar <= '9')
        hexChar = hexChar - '0';
    else if (hexChar >= 'A' && hexChar <= 'F')
        hexChar = hexChar - 'A' + 10;
    else {
        // throws std::invalid_argument exception if invalid hex character is used
        throw invalid_argument( string("Received invalid hex value: \'") + string(1, hexChar) + "\'" );
    }
    // binary representation of hex character, indexed by implicit hex value
    string conversionDict[] = {
        "0000",
        "0001",
        "0010",
        "0011",
        "0100",
        "0101",
        "0110",
        "0111",
        "1000",
        "1001",
        "1010",
        "1011",
        "1100",
        "1101",
        "1110",
        "1111"
        };
    return  conversionDict[hexChar];
}

/*
    composes a binary string from hex string
*/
string hexToBin(string hexStr) {
    stringstream ss;

    // consecutively append binary representation of each hex character
    for (const char &c : hexStr) {
        ss << hexToBin(c);
    }
    
    return ss.str();
}


int binStrToInt(string binStr) {
    int dec_value = 0;
    int base = 1;
    int len = binStr.length();
    for (int i = len - 1; i >= 0; i--) {
        if (binStr[i] == '1')
            dec_value += base;
        base = base * 2;
    }
    return dec_value;
}