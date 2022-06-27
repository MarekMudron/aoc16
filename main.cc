#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <stdexcept>
#include "utils.h"
using namespace std;


string parse(string binString, int &versionSum);


string input1 = "D2FE28"; // 6
string input2 = "EE00D40C823060"; // 14
string input3 = "38006F45291200"; // 9
string input4 = "8A004A801A8002F478"; // 16
string input5 = "620080001611562C8802118E34"; // 12
string input6 = "C0015000016115A2E0802F182340";   // 23
string input7 = "A0016C880162017C3686B18A3D4780"; // 31
string input8 = "005532447836402684AC7AB3801A800021F0961146B1007A1147C89440294D005C12D2A7BC992D3F4E50C72CDF29EECFD0ACD5CC016962099194002CE31C5D3005F401296CAF4B656A46B2DE5588015C913D8653A3A001B9C3C93D7AC672F4FF78C136532E6E0007FCDFA975A3004B002E69EC4FD2D32CDF3FFDDAF01C91FCA7B41700263818025A00B48DEF3DFB89D26C3281A200F4C5AF57582527BC1890042DE00B4B324DBA4FAFCE473EF7CC0802B59DA28580212B3BD99A78C8004EC300761DC128EE40086C4F8E50F0C01882D0FE29900A01C01C2C96F38FCBB3E18C96F38FCBB3E1BCC57E2AA0154EDEC45096712A64A2520C6401A9E80213D98562653D98562612A06C0143CB03C529B5D9FD87CBA64F88CA439EC5BB299718023800D3CE7A935F9EA884F5EFAE9E10079125AF39E80212330F93EC7DAD7A9D5C4002A24A806A0062019B6600730173640575A0147C60070011FCA005000F7080385800CBEE006800A30C023520077A401840004BAC00D7A001FB31AAD10CC016923DA00686769E019DA780D0022394854167C2A56FB75200D33801F696D5B922F98B68B64E02460054CAE900949401BB80021D0562344E00042A16C6B8253000600B78020200E44386B068401E8391661C4E14B804D3B6B27CFE98E73BCF55B65762C402768803F09620419100661EC2A8CE0008741A83917CC024970D9E718DD341640259D80200008444D8F713C401D88310E2EC9F20F3330E059009118019A8803F12A0FC6E1006E3744183D27312200D4AC01693F5A131C93F5A131C970D6008867379CD3221289B13D402492EE377917CACEDB3695AD61C939C7C10082597E3740E857396499EA31980293F4FD206B40123CEE27CFB64D5E57B9ACC7F993D9495444001C998E66B50896B0B90050D34DF3295289128E73070E00A4E7A389224323005E801049351952694C000";
//int versionSum = 0;


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

int main() {
    cout<<sumOfVers(input8)<<endl;;
    return 0;
}