#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char** argv)
{
    vector<vector<int>> height(3, vector<int>(3));
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            int h;
            cin >> h;
            height[i][j] = h;
        }
    }

    bool all_equals = false;

    for (int i=0; i<3; i++) {
        if (height[i][0] == height[i][1] && height[i][0] == height[i][2] && height[i][1] == height[i][2]) {
            all_equals = true;
        }
        else if (height[0][i] == height[1][i] && height[0][i] == height[2][i] && height[2][i] == height[1][i]) {
            all_equals = true;
        }
    }
    if (all_equals == true) {
        cout << "0" << endl;
        return 0;
    }
    else {
        int min_cost = 1e9;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                // 6줄만 확인하면 됨. (1, 1), (2, 2), (3, 3) 양 방향 확인하기
                // 여기서 값의 종류는 순서 상관없이 (1, 1, 2) => 1, (1, 1, 3) => 2, 
                //                          (2, 2, 1) => 1, (2, 2, 3) => 1,  
                //                          (3, 3, 1) => 2, (3, 3, 2) => 1, 
                //                          (1, 2, 3) => 2
                if (i == j) {
                    int current_point = height[i][j];
                    int diff = 1e9;
                    // 세 가지 모두 다를 때
                    if (current_point != height[i][(j+1)%3] && 
                        current_point != height[i][(j+2)%3] &&
                        height[i][(j+1)%3] != height[i][(j+2)%3]) {
                            diff = 2;
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    if (current_point != height[(i+1)%3][j] && 
                        current_point != height[(i+2)%3][j] &&
                        height[(i+1)%3][j] != height[(i+2)%3][j]) {
                            diff = 2;
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    // 두 개는 똑같을 때
                    if (current_point == height[i][(j+1)%3]) {
                        diff = abs(current_point - height[i][(j+2)%3]);
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    if (current_point == height[i][(j+2)%3]) {
                        diff = abs(current_point - height[i][(j+1)%3]);
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    if (height[i][(j+1)%3] == height[i][(j+2)%3]) {
                        diff = abs(current_point - height[i][(j+1)%3]);
                        if (diff < min_cost)
                            min_cost = diff;
                    } 
                    if (current_point == height[(i+1)%3][j]) {
                        diff = abs(current_point - height[(i+2)%3][j]);
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    if (current_point == height[(i+2)%3][j]) {
                        diff = abs(current_point - height[(i+1)%3][j]);
                        if (diff < min_cost)
                            min_cost = diff;
                    }
                    if (height[(i+1)%3][j] == height[(i+2)%3][j]) {
                        diff = abs(current_point - height[(i+1)%3][j]);
                        if (diff < min_cost)
                            min_cost = diff;
                    }
    
                }
            }     
        }

        cout << min_cost << endl;
    }
    
    return 0;
}