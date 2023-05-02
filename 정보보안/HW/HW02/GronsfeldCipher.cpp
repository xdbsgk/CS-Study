#include <iostream>
#include <string>

using namespace std;

void Encryption(string, string);
void Decryption(string, string);

int main() {
    int mode;
    string ptext, ctext, key;
    cout << "암호화는 [1], 복호화는 [2]를 입력하세요 :" << endl;
    cin >> mode;
    switch (mode)
    {
    case 1:
    {
        cout << "평문을 입력하세요" << endl;
        cin >> ptext;
        cout << "키를 입력하세요." << endl;
        cin >> key;
        Encryption(ptext, key);
        break;
    }
    case 2:
    {
        cout << "암호문을 입력하세요" << endl;
        cin >> ctext;
        cout << "키를 입력하세요." << endl;
        cin >> key;
        Decryption(ctext, key);
        break;
    }
    default:
    {
        cout << "[INPUT ERROR] 제대로 입력하지 않았습니다." << endl;
        return 0;
    }
    }

    return 0;
}

void Encryption(string plaintext, string key) {
    string ciphertext;
    for (int i=0; i<plaintext.length(); i++) {
        int key_value = key[i % key.length()] - '0';
        int plain_value = (int)plaintext[i];
        int cipher_value = ((plain_value - 32 + key_value) % 94) + 32;
        char cipher_char = (char)cipher_value;
        ciphertext += cipher_char;
    }
    cout << "Ciphertext : "<< ciphertext << endl;
}

void Decryption(string ciphertext, string key) {
    string plaintext;
    for (int i=0; i<ciphertext.length(); i++) {
        int key_value = key[i % key.length()] - '0';
        int cipher_value = (int)ciphertext[i];
        int plain_value = ((cipher_value - 32 - key_value + 94) % 94) + 32;
        char plain_char = (char)plain_value;
        plaintext += plain_char;
    }
    cout << "Plaintext : " << plaintext << endl;
}
