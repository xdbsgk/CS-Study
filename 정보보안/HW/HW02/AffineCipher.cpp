#include <iostream>
#include <cstring>
using namespace std;

char *AffineCeasarCipher(int, char *, int, int, int);

int main() {
    int key1, key2, str_size=0;
    char str[50] = {0, };

    int mode;
    cout << "암호화는 [1], 복호화는 [2]를 입력하세요." << endl;
    cin >> mode;
    cin.ignore();
    switch (mode)
    {
    case 1:
    {
        cout << "평문을 입력하세요" << endl;
        cin.getline(str, sizeof(str));
        break;
    }
    case 2:
    {
        cout << "암호문을 입력하세요" << endl;
        cin.getline(str, sizeof(str));
        break;
    }
    default:
    {
        cout << "[INPUT ERROR] 제대로 입력하지 않았습니다." << endl;
        return 0;
    }
    }

    cout << "첫 번째 키(K1 - 정수) 값을 입력하세요 : ";
    cin >> key1;
    cout << "두 번째 키(K2 - 정수) 값을 입력하세요 : ";
    cin >> key2;

    // 복호화를 위해 key1의 inverse 값 구하기
    if (mode == 2) {
        int flag = 0;
        for (int k=0; k<26; k++) {
            flag = (key1 * k) % 26;
            if (flag == 1) {
                key1 = k;
                cout << "key 1 : "<< key1 << endl;
                break;
            }
        }
    }

    str_size = strlen(str);
    AffineCeasarCipher(mode, str, str_size, key1, key2);

    if (mode == 1) {
        cout << "[암호화된 결과 출력]" << endl;
        cout << str << endl;
    } else {
        cout << "[복호화된 결과 출력]" << endl;
        cout << str << endl;
    }

    return 0;
}

char *AffineCeasarCipher(int mode, char *str, int str_size, int k1, int k2) {
    // ToDo : 복호화 과정에서 k1의 inverse값 구하는 과정 삽입하기
    for (int i = 0; i < str_size; i++) {
        // 대문자일 경우
        if ((str[i] >= 'A') && (str[i] <= 'Z')) {
            // 암호화
            if (mode == 1) {
                str[i] = ((k1 * (str[i] - 'A') + k2) % 26) + 'A';
            }
            // 복호화
            else if (mode == 2) {
                int temp = str[i] - 'A' - k2;
                while (temp < 0)
                    temp += 26;
                str[i] = ((k1 * temp) % 26) + 'A';
            }
        }
        // 소문자일 경우
        else if ((str[i] >= 'a') && (str[i] <= 'z')) {
            // 암호화
            if (mode == 1) {
                str[i] = ((k1 * (str[i] - 'a') + k2) % 26) + 'a';
            }
            // 복호화
            else if (mode == 2) {
                int temp = str[i] - 'a' - k2;
                while (temp < 0)
                    temp += 26;
                str[i] = ((k1 * temp) % 26) + 'a';
            }
        }
        else
            ;
    }

    return str;
}
