#include <iostream>
#include <fstream>
#include <vector>
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

class Parity : dataWordManage
{
public:
    bool ParityBit(uint8_t byte) {
        return ParityBit(ChangeDataWord(byte));
    }

    bool ParityBit(vector <bool> dataWord) {
        bool Parity = true;
        for (bool data : dataWord) {
            if (data) Parity = !Parity;
        }
        return Parity;
    }

    vector <bool> FindCW(uint8_t byte) {
        return FindCW(ChangeDataWord(byte));
    }

    vector <bool> FindCW(vector <bool> dataWord) {
        vector <bool> codeWord = dataWord;
        codeWord.push_back(ParityBit(dataWord));
        return codeWord;
    }

    vector <vector <bool>> FindCW(vector <uint8_t> datas) {
        return FindCW(ChangeDataWord(datas));
    }

    vector <vector <bool>> FindCW(string datas) {
        return FindCW(ChangeDataWord(datas));
    }

    vector <vector <bool>> FindCW(vector <vector <bool>> dataWords) {
    int size = dataWords.size();
        vector <vector <bool>> codeWord;
        vector <bool> colParity(8, true);
        for (auto dataWord : dataWords) {
            codeWord.push_back(FindCW(dataWord));
            for (int i = 0; i < 7; i++) {
                if (dataWord[i]) colParity[i] = !colParity[i];
            }
        }
        codeWord.push_back(colParity);
        return codeWord;
    }

    bool Inspection(vector <vector <bool>> codeWord) {
        int size = codeWord.size()-1;
        vector <bool> colParity(7, true);
        for (int i = 0; i < size; i++) {
            if (!Inspection(codeWord[i])) return false;
            for (int j = 0; j < 7; j++) {
                if (codeWord[i][j]) colParity[j] = !colParity[j];
            }
        }
        
        for (int i = 0; i < 7; i++) {
            if (colParity[i] != codeWord.back()[i]) return false;
        }
        return true;
    }

    bool Inspection(vector <bool> codeWord) {
        bool Parity = true;
        for (int i = 0; i < codeWord.size()-1; i++) {
            if (codeWord[i]) Parity = !Parity;
        }
        return codeWord.back() == Parity;
    }
};

void PrintParityMatrix(vector <vector <bool>> Parity2D) {
    for (int i = 0; i < Parity2D.size()-1; i++) {
        for (int j = 0; j < Parity2D[i].size()-1; j++) {
            cout << ' ' << Parity2D[i][j];
        }
        cout << '|' << Parity2D[i].back() << '\n';
    }
    cout << "--------------+--\n";
    for (int j = 0; j < Parity2D.back().size()-1; j++) {
        cout << ' ' << Parity2D.back()[j];
    }
    cout << '|' << Parity2D.back().back() << '\n';
}

void Parity1D(void) {
    char ch;
    Parity p;

    cout << "Input character : ";
    cin >> ch;
    auto codeWord = p.FindCW(ch);

    codeWord[0] = !codeWord[0];
    codeWord[1] = !codeWord[1];
    
    cout << ch << "'s parity code word is : ";
    for (bool bit : codeWord) cout << bit;
    if (p.Inspection(codeWord)) cout << "\nError is nothing\n\n";
    else cout << "\nthis code word is Error\n\n";
}

void Parity2D(void) {
    string data;
    char ch;
    Parity p;

    cout << "Input file name : ";
    cin >> data;

    ifstream in_file(data);
    if (!in_file.is_open()) exit(1);
    while (in_file.get(ch)) data.push_back(ch);
    in_file.close();

    vector <vector <bool>> Parity2D = p.FindCW(data);

    Parity2D[2][2] = !Parity2D[2][2];
    Parity2D[2][3] = !Parity2D[2][3];
    Parity2D[3][2] = !Parity2D[3][2];
    //Parity2D[3][3] = !Parity2D[3][3];

    PrintParityMatrix(Parity2D);
    if (p.Inspection(Parity2D)) cout << "Error is nothing\n";
    else cout << "this code word is Error\n";
}

int main(void) {
    Parity1D();
    Parity2D();

    return 0;
}