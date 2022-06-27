#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <stdexcept>
#include "utils.h"
#include "main.h"

using namespace std;

string parse(string packet, int &versionSum);



/**
 * @brief Parses payload of literal packet.
 * 
 * @param payload Payload of literal packet.
 * @return Remainder string which was not processed.
 */
string literal(string payload) {
    int groupLen = 5;
    int digitLen = groupLen - 1;
    stringstream ss;
    int index = 0;
    string binDigit;
    do {
        binDigit = payload.substr(index, groupLen);
        index += groupLen;
    }while(binDigit[0] == '1');
    ss << payload.substr(index, string::npos);
    return ss.str();
}

/**
 * @brief Parses sequence of packets by length in bits.
 * 
 * @param payload Sequence of packets.
 * @return Remainder string which was not processed.
 */
string byBits(string sequence, int len, int &versionSum) {
    string remainder = sequence;
    while(len != 0) {
        int oldLen = sequence.length();
        remainder = parse(sequence, versionSum);
        int diff = oldLen - remainder.length();
        len -= diff;
        sequence = remainder;
    }
    return remainder;
}

/**
 * @brief Parses sequence of packets by count.
 * 
 * @param payload Sequence of packets.
 * @return Remainder string which was not processed.
 */
string byPackets(string sequence, int count, int &versionSum) {
    string remainder = sequence;
    for(int i = 0; i < count; i++) {
        remainder = parse(sequence, versionSum);
        sequence = remainder;
    }
    return remainder;
}

/**
 * @brief Parses payload of operator packet.
 * 
 * @param payload Payload of operator packet.
 * @return Remainder string which was not processed.
 */
string op(string payload, int &versionSum) {
    char lti = payload[0];
    payload = payload.substr(1,string::npos);
    string remainder;
    int lfieldLen;
    if(lti == '1') {
        lfieldLen = 11;
        int numSubPackets = binStrToInt(payload.substr(0, lfieldLen));
        payload = payload.substr(11, string::npos);
        remainder = byPackets(payload, numSubPackets, versionSum);
    }else{
        lfieldLen = 15;
        int bitsSubPackets = binStrToInt(payload.substr(0, lfieldLen));
        remainder = payload.substr(lfieldLen+bitsSubPackets, string::npos);
        payload = payload.substr(lfieldLen, bitsSubPackets);
        byBits(payload, bitsSubPackets, versionSum);
    }
    return remainder;
}

/**
 * @brief Parses part a string of 1s and 0s.
 * 
 * @param packet String of 1s and 0s.
 * @return Part of string was not parsed and is yet to be parsed.
 */
string parse(string packet, int &versionSum) {
    int vFieldLen = 3, tFieldLen = 3;
    if(packet.length() < vFieldLen + tFieldLen) {
        throw invalid_argument( string("Packet too short"));
    }
    string version = packet.substr(0, vFieldLen);
    string type = packet.substr(vFieldLen,tFieldLen);
    string payload = packet.substr(vFieldLen + tFieldLen, string::npos);
    versionSum += stoi(version, 0, 2);
    string remainder;
    if(type == "100") {
        // if version type = 4, then packet is literal
        remainder = literal(payload);
    }else {
        // operator packet
        remainder = op(payload, versionSum);
    }
    return remainder;
}

/**
 * @brief Gets sum of version values of a packet and subpackets.
 * 
 * @param hexStr Hexadecimal string to be processed.
 * @return Sum of versions.
 */
int sumOfVers(string hexStr) {
    string binStr = hexToBin(hexStr);
    int versionSum = 0;
    parse(binStr, versionSum);
    return versionSum;
}
