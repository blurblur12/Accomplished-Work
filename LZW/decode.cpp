#include <iostream>
#include <vector>
#include<string>
using namespace std;

// helper function for bin_to_dec, computes the power of 2
int power_two(int expon) {
    if (expon == 0) {
        return 1;
    }
    int power = 1;
    for (int i = 0; i < expon; i++) {
        power = power * 2;
    }
    return power;
}

// helper function to convert binary back to decimal
int bin_to_dec(string str) {
    int dec = 0;
    for (int i = 0; i < 12; i++) {
        if (str.substr(i,1) == "1") {
            dec = dec + power_two(11-i);
        }
    }
    return dec;
}

// LZW decode algorithm 
void LZW_decode_new(const string& coded_s, string& s) {
    if (coded_s.size() <= 0) {
        return;
    }
    // Initialize the dictionary to contain the lower case
    // english alphabet and the words aa,...,az,...,za,...,zz
    vector<string> d;
    d.resize(702);
    // use this string to initialize the dictionary
    string s_init{"abcdefghijklmnopqrstuvwxyz"};
    for (int i = 0; i < 26; i++) {
        d[i] = s_init.substr(i,1);
    }
    int l = -1;
    // add aa,...,az,...,za,...,zz to dictionary
    for (int j = 26; j < 702; j++) {
        if (j % 26 == 0) {
            l++;
        }
        d[j] = "";
        d[j].append(d[l]);
        d[j].append(d[j-26*(l+1)]);
    }
    
    // next available code word is 702
    int idx = 702;
    // get the first character of the coded text
    int code = bin_to_dec(coded_s.substr(0, 12));
    s.append(d[code]);
    
    // the previous s;
    string s_prev{""};
    string s_cur = d[code];
    // loop while there are more characters in the coded text
    int i = 12;
    while (i + 11 < coded_s.size()) {
        s_prev = s_cur;
        code = bin_to_dec(coded_s.substr(i, 12));
        // if code is already in the dictionary, decode the code word
        if (code < idx) {
            s_cur = d[code];
        }
        else if (code > idx) {
            // when encoding each time we added two new code words
            // into the dictionary, in this case the code word must
            // be the second code word that was added, so the corresponding
            // text must be s_prev + s_prev[0,1], moreover, idx must
            // correspond to the text s_prev + s_prev[0]
            s_cur = s_prev;
            s_cur.append(s_prev.substr(0,2));
        }
        else if (code == idx) {
            s_cur = s_prev;
            s_cur.append(s_prev.substr(0,1));
        }
        else {
            // must be given an invalid coded text
        }
        s.append(s_cur);
        string temp = s_prev;
        // corresponds to the text with code idx
        d.push_back(temp.append(s_cur.substr(0,1)));
        // corresponds to the text with code idx + 1
        d.push_back(s_prev.append(s_cur.substr(0,2)));
        idx = idx + 2;
        i = i + 12;
    }
}


int main()
{  
    string s = "";
    string coded_s;
    getline(cin, coded_s);
    LZW_decode_new(coded_s, s);
    cout << s;
}
