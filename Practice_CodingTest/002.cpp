#include<iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
    vector<int> dirr(8);
    for (int i=0; i<8; i++) {
        int curr;
        cin >> curr;
        dirr[i] = curr;
    }

    int check_asc = 0;
    int check_dsc = 0;
    
    for (int i=0; i<8; i++) {
        if (dirr[i] == i+1) {
            check_asc++;
        } else if (dirr[i] == 8-i)
            check_dsc++;
        else {
            continue;
        }
    }
    
    if (check_asc == 8) {
        cout << "ascending\n"; 
    } else if (check_dsc == 8) {
        cout << "descending\n";
    } else {
        cout << "mixed\n";
    }
    
    return 0;
}