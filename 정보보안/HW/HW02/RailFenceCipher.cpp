#include <iostream>
#include <vector>
using namespace std;

void Encryption(vector<int>*, string, int);
void Decryption(vector<int>*, string, int);

int main() {

    int mode, rail;
    string ptext, ctext;
    cout << "암호화는 [1], 복호화는 [2]를 입력하세요 :" << endl;
    cin >> mode;

    if (mode == 1) {
        cout << "평문을 입력하세요" << endl;
            cin.ignore();
        getline(cin, ptext);
        cout << "rail 개수를 입력하세요." << endl;
        cin >> rail;
        vector<int>* v = new vector<int>[rail];
        Encryption(v, ptext, rail);
        delete[] v;
    } else if (mode == 2) {
        cout << "암호문을 입력하세요" << endl;
        cin.ignore();
        getline(cin, ctext);
        cout << "rail 개수를 입력하세요." << endl;
        cin >> rail;
        vector<int>* v = new vector<int>[rail];
        Decryption(v, ctext, rail);
        delete[] v;
    } else {
        cout << "[INPUT ERROR] 제대로 입력하지 않았습니다." << endl;
    }

    return 0;
}

void Encryption(vector<int> *v, string plaintext, int r) {
    string ciphertext;
    int j = 0;
    bool order = false;

    for (int i = 0; i < plaintext.length(); i++) {
        v[j].push_back(i);
        if (j==0 || j==r - 1) {
            order = !order;
        }
        if (order) {
            j++;
        } else {
            j--;
        }
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < v[i].size(); j++) {
            ciphertext += plaintext[v[i][j]];
        }
    }
    cout << "암호문: " << ciphertext << endl;
}

void Decryption(vector<int> *v, string ciphertext, int r) {
    string plaintext(ciphertext.length(), ' ');
    int j = 0;
    bool order = false;
    for (int i = 0; i < ciphertext.length(); i++) {
        v[j].push_back(i);
        if (j == 0 || j == r - 1) {
            order = !order;
        }
        if (order) {
            j++;
        } else {
            j--;
        }
    }
    int idx = 0;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < v[i].size(); j++) {
            plaintext[v[i][j]] = ciphertext[idx++];
        }
    }
    cout << "평문: " << plaintext << endl;
}
