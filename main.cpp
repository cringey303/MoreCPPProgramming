#include <fstream> //for getting file
#include <iostream> //for cout, cin
#include <utility> //for swap (std::swap) - not used here, but in your header
#include <iomanip> //for output formatting (std::setw) - not used here
#include <sstream> //for parsing integers from lines
#include <string> //for std::string
#include <algorithm> //for std::reverse
#include <cctype> //for isdigit

// --- Foward Declarations ---
std::string intAdd(std::string s1, std::string s2);
std::string intSubtract(std::string s1, std::string s2);
bool isGreater(std::string s1, std::string s2);
void normalize(std::string& intPart, std::string& decPart);


//takes a string and breaks it into sign, integer, and decimal parts
void format(std::string num, std::string& intPart, std::string& decPart, bool& isNeg) {
    isNeg = false; //initialize
    
    if (num[0] == '-') {
        //if negative sign, classify as negative and omit sign
        isNeg = true;
        num = num.substr(1);
    } else if (num[0] == '+') {
        //otherwise just omit sign
        isNeg = false;
        num = num.substr(1);
    }

    // FIX: Changed 'int decPos' to 'size_t decPos' to match 'npos'
    size_t decPos = num.find('.');
    if (decPos == std::string::npos) {
        //if no decimal, assign whole num as intPart
        intPart = num;
        decPart = ""; //FIX: must be double quotes for string
    } else {
        //if decimal, take left of decimal as int
        intPart = num.substr(0,decPos);
        //and right as decimal part
        decPart = num.substr(decPos+1);
    }
}

//removes leading zeros from int and trailing zeros from dec
void normalize(std::string& intPart, std::string& decPart) {
    //remove leading zeros from integer part
    //(but leave a single '0' if that's all there is)
    // FIX: Changed 'int i' to 'size_t i'
    size_t i = 0;
    while (i < intPart.length() - 1 && intPart[i] == '0') {
        i++;
    }
    intPart = intPart.substr(i);

    //remove trailing zeros from decimal part
    // FIX: Rewrote loop to be safer and use size_t
    size_t j = decPart.length();
    while (j > 0 && decPart[j - 1] == '0') {
        j--;
    }
    decPart = decPart.substr(0, j);
}

//checks if positive int string s1 is greater than s2
bool isGreater(std::string s1, std::string s2) {
    if (s1.length() > s2.length()) return true;
    if (s1.length() < s2.length()) return false;
    
    //if lengths are equal, check char by char
    // FIX: Changed 'int i' to 'size_t i'
    for (size_t i = 0; i < s1.length(); i++) {
        if (s1[i] > s2[i]) return true;
        if (s1[i] < s2[i]) return false;
    }
    return false; //they are equal
}


//add strings as integers
//must have no decimals and be equal length
std::string intAdd(std::string s1, std::string s2) {
    std::string result = "";
    // Using 'int' here is fine, as we are counting *down* to 0
    // and not comparing against a .length()
    int i = s1.length() - 1;
    int j = s2.length() - 1;
    int carry = 0;

    //loop from the end of the strings
    while (i>=0 || j>=0 || carry) {
        //get digits, or 0 if string has been processed
        int digit1 = (i >= 0) ? (s1[i] - '0') : 0;
        int digit2 = (j >= 0) ? (s2[j] - '0') : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        
        //add the last digit of sum to our result
        result.push_back((sum % 10) + '0');

        i--;
        j--;
    }
    
    //result was built backwards, so reverse it
    std::reverse(result.begin(), result.end());
    
    if (result.empty()) return "0";
    return result;
}

//subtracts positive int strings (s1 - s2), assumes s1 >= s2
std::string intSubtract(std::string s1, std::string s2) {
    std::string result = "";
    int i = s1.length() - 1;
    int j = s2.length() - 1;
    int borrow = 0;

    while (i >= 0) {
        int digit1 = s1[i] - '0';
        int digit2 = (j >= 0) ? (s2[j] - '0') : 0;
        
        int diff = digit1 - digit2 - borrow;

        if (diff < 0) {
            //if diff is negative, add 10 and borrow
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back(diff + '0');
        i--;
        j--;
    }

    std::reverse(result.begin(), result.end());
    
    //normalize to remove leading zeros (e.g., 100-99 = 001)
    std::string finalResult = "";
    // FIX: Changed 'int k' to 'size_t k'
    size_t k = 0;
    while (k < result.length() - 1 && result[k] == '0') {
        k++;
    }
    finalResult = result.substr(k);

    return finalResult;
}


//main addition function, handles signs and decimals
std::string add(std::string str1, std::string str2) {
    //break numbers into parts
    std::string int1, dec1, int2, dec2;
    bool isNeg1, isNeg2;
    format(str1, int1, dec1, isNeg1);
    format(str2, int2, dec2, isNeg2);

    //--- Pad Decimal Parts ---
    //find longest decimal part
    // FIX: Changed 'int maxDecLen' to 'size_t maxDecLen'
    size_t maxDecLen = std::max(dec1.length(), dec2.length());
    //pad shorter decimal part with trailing zeros
    // (This will now be a safe unsigned comparison)
    while (dec1.length() < maxDecLen) dec1.push_back('0');
    while (dec2.length() < maxDecLen) dec2.push_back('0');

    //--- Combine Parts ---
    //combine int and dec into a single large integer string
    std::string full1 = int1 + dec1;
    std::string full2 = int2 + dec2;

    std::string resultFull = "";
    std::string resultSign = "";

    //--- Decide Operation based on Sign ---
    if (isNeg1 == isNeg2) {
        //CASE 1: Addition (e.g., 5 + 10 or -5 + -10)
        resultFull = intAdd(full1, full2);
        if (isNeg1) {
            resultSign = "-"; //result is negative
        }
    } else {
        //CASE 2: Subtraction (e.g., 10 + -5 or -10 + 5)
        if (isGreater(full1, full2)) {
            resultFull = intSubtract(full1, full2);
            if (isNeg1) {
                resultSign = "-"; //sign of the larger number
            }
        } else {
            resultFull = intSubtract(full2, full1);
            if (isNeg2) {
                resultSign = "-"; //sign of the larger number
            }
        }
    }

    //--- Format the Result ---
    //ensure result string is long enough to re-insert decimal
    //(e.g., 0.5 - 0.2 = 0.3, resultFull="3", need "03" for "0.3")
    // (This is now a safe unsigned comparison)
    while (resultFull.length() <= maxDecLen) {
        resultFull.insert(0, "0");
    }

    std::string finalInt, finalDec;
    if (maxDecLen == 0) {
        //no decimal part
        finalInt = resultFull;
        finalDec = "";
    } else {
        //re-insert decimal point
        // FIX: 'decPos' is now 'size_t'
        size_t decPos = resultFull.length() - maxDecLen;
        finalInt = resultFull.substr(0, decPos);
        finalDec = resultFull.substr(decPos);
    }
    
    //clean up 0s (e.g., 007.500 -> 7.5)
    normalize(finalInt, finalDec);

    //Handle final string
    //don't add sign to 0
    if (finalInt == "0" && finalDec.empty()) {
        return "0";
    }

    if (finalDec.empty()) {
        return resultSign + finalInt; //e.g., "123"
    } else {
        return resultSign + finalInt + "." + finalDec; //e.g., "123.45"
    }
}


//FIX: rewritten to use 'size_t' for all indices
bool isValidDouble(const std::string& s) {
    if (s.empty()) return false;

    // FIX: 'i' is now 'size_t'
    size_t i = 0;
    //check for optional sign
    if (s[i] == '+' || s[i] == '-') {
        i++;
    }
    
    //must have at least one char after sign
    // (This is now a safe unsigned comparison)
    if (i == s.length()) return false; //e.g., "+" or "-"

    // FIX: 'decPos' is now 'size_t' and uses 'npos'
    size_t decPos = std::string::npos;
    for(size_t j = i; j < s.length(); j++) {
        if (s[j] == '.') {
            if (decPos != std::string::npos) return false; //found second '.', e.g. "5.5.5"
            decPos = j;
        }
    }

    if (decPos == std::string::npos) {
        //--- CASE 1: No Decimal (e.g., "123") ---
        int digitCount = 0; // 'int' is fine for a counter not compared to .length()
        for (size_t j = i; j < s.length(); j++) {
            if (!isdigit(s[j])) return false; //e.g., "12A"
            digitCount++;
        }
        return (digitCount > 0); //must have at least one digit

    } else {
        //--- CASE 2: Decimal Found (e.g., "1.23") ---
        //Rule: must have digits on BOTH sides
        
        //check left side
        int intDigits = 0;
        for (size_t j = i; j < decPos; j++) {
            if (!isdigit(s[j])) return false;
            intDigits++;
        }
        if (intDigits == 0) return false; //e.g., ".5" or "-.5"

        //check right side
        int decDigits = 0;
        for (size_t j = decPos + 1; j < s.length(); j++) {
            if (!isdigit(s[j])) return false;
            decDigits++;
        }
        if (decDigits == 0) return false; //e.g., "5." or "+10."
    }

    //if we passed all checks for the decimal case
    return true;
}

int main() {
    std::string filename;
    std::cout << "Enter a filename: ";
    std::getline(std::cin, filename);
    //make sure filename is not empty or just whitespace
    while(filename.empty() || filename.find_first_not_of(" \t\n\r") == std::string::npos) {
        std::cout << "Invalid input. Please enter a filename: ";
        std::getline(std::cin, filename);
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return 1; //exit with error
        }
    
    std::string lineStr;
    int line = 1; // 'int' is fine for a simple counter

    //FIX: read line by line, not word by word
    while(std::getline(file, lineStr)) {
        std::istringstream iss(lineStr);
        std::string str1, str2;

        //try to parse two numbers from the line
        if (!(iss >> str1 >> str2)) {
            //if line is blank or has only one num
            if (!lineStr.empty() && lineStr.find_first_not_of(" \t\n\r") != std::string::npos) {
                 std::cout << "Line " << line << ": Error - Malformed line. Skipping." << std::endl;
            }
            line++;
            continue;
        }
        
        //check if there's extra data
        std::string extra;
        if (iss >> extra) {
            std::cout << "Line " << line << ": Error - Too many inputs on line. Skipping." << std::endl;
            line++;
            continue;
        }
        
        //validate both numbers
        bool isValid1 = isValidDouble(str1);
        bool isValid2 = isValidDouble(str2);

        if (isValid1 && isValid2) {
            //if both are valid, add them
            std::string sum = add(str1, str2);
            std::cout << "Line " << line << ": " << str1 << " + " << str2 << " = " << sum << std::endl;
        } else {
            //if one or both are invalid, print error
            std::cout << "Line " << line << ": Error - Invalid number." << std::endl;
            if (!isValid1) std::cout << "  '" << str1 << "' is invalid." << std::endl;
            if (!isValid2) std::cout << "  '" << str2 << "' is invalid." << std::endl;
        }

        line++;
    }

    file.close();
    return 0;
}