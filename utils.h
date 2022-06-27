#include <string>

using namespace std;


/**
 * @brief Takes hex character as input, outputs string of 1s and 0s.
 * 
 * @param binChar Hexadecimal character to convert.
 * @return String of 1s and 0s representing hex char in binary.
 */
string hexToBin(char hexChar);


/**
 * @brief Takes string of hex characters, outputs string of 1s and 0s.
 * 
 * @param hexStr String of hexadecimal characters to convert.
 * @return String of 1s and 0s representing hex string in binary.
 */
string hexToBin(string hexStr);

/**
 * @brief Takes string of 1s and 0s, outputs integer with represented value.
 * 
 * @param binStr String of 1s and 0s.
 * @return Integer with represented value.
 */
int binStrToInt(string binStr);