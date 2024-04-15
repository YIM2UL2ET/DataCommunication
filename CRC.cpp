#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class dataWordManage
{
protected:
    vector <bool> ChangeDataWord(uint8_t data) {
        vector <bool> dataWord(7);
        for (int i = 0; i < 7; i++) {
            dataWord[6-i] = data & (uint8_t(0x1) << i);
        }
        return dataWord;
    }

    vector <vector <bool>> ChangeDataWord(vector <uint8_t> datas) {
        vector <vector <bool>> dataWord;
        for (bool data : datas) dataWord.push_back(ChangeDataWord(data));
        return dataWord;
    }

    vector <vector <bool>> ChangeDataWord(string datas) {
        vector <vector <bool>> dataWord;
        for (auto data : datas) dataWord.push_back(ChangeDataWord(data));
        return dataWord;
    }
};

class CRC: dataWordManage
{
private:
    int polySize;
    vector <bool> polynomial;

    void Calculate(vector <bool> &dataCode) {
        while (dataCode.size() > polySize-1) {
            if (dataCode.front()) {
                for (int i = 0; i < polySize; i++) {
                    if (dataCode[i] != polynomial[i]) dataCode[i] = 1;
                    else dataCode[i] = 0;
                }
            }
            dataCode.erase(dataCode.begin());
        }
    }

public:
    CRC (string poly) {
        for (char bit : poly) {
            if (bit == '1') polynomial.push_back(1);
            else polynomial.push_back(0);
        }
        polySize = poly.size();
    }

    vector <vector <bool>> ReturnCW(string data) {
        return ReturnCW(ChangeDataWord(data));
    }

    vector <vector <bool>> ReturnCW(vector <vector <bool>> dataCodes) {
        for (auto &dataCode : dataCodes) dataCode = ReturnCW(dataCode);
        return dataCodes;
    }

    vector <bool> ReturnCW(uint8_t data) {
        return ReturnCW(ChangeDataWord(data));
    }

    vector <bool> ReturnCW(vector <bool> dataWord) {
        vector <bool> codeWord = dataWord;
        for (int i = 0; i < polySize-1; i++) dataWord.push_back(0);
        Calculate(dataWord);
        for (auto bit : dataWord) codeWord.push_back(bit);
        return codeWord;
    }

    bool Check(vector <bool> codeWord) {
        Calculate(codeWord);
        for (auto bit : codeWord) {
            if (bit) return false;
        }
        return true;
    }

    bool Check(vector <vector <bool>> codeWords) {
        for (auto codeWord : codeWords) {
            if (!Check(codeWord)) return false;
        }
        return true;
    }

    int CWSize(void) {
        return polySize + 7;
    }
};

int main(void) {
    string data, poly;
    char ch;

    ifstream in_file("text.txt");
    if (!in_file.is_open()) exit(1);
    while (in_file.get(ch)) data.push_back(ch);
    in_file.close();

    cout << "Input polynomial in bit : ";
    cin >> poly;

    CRC crc(poly);

    auto codeWords = crc.ReturnCW(data);
    codeWords[0][1] = !codeWords[0][1];
    codeWords[0][2] = !codeWords[0][2];
    
    int cnt = 0, cws = crc.CWSize();
    cout << "data's codeWords : \n";
    for (auto cw : codeWords) {
        for (auto bit : cw) cout << bit;
        if (cnt > 100) cout << '\n', cnt = 0;
        else cout << " ", cnt += cws;
    }
    cout << '\n';

    if(crc.Check(codeWords)) cout << "Error is nothing";
    else cout << "this code word is Error";

    return 0;
}