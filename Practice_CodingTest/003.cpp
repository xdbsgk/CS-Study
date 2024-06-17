#include <string>
#include <vector>

using namespace std;

long long solution(int r1, int r2) {
    long long count = 0;
    for (int x=-r2; x<=r2; x++) {
        for (int y=-r2; y<=r2; y++) {
            if (x*x + y*y <= r2*r2) {
                count++;
            }
        }
    }
    for (int x=-r1; x<=r1; x++) {
        for (int y=-r1; y<=r1; y++) {
            if (x*x + y*y < r1*r1) {
                count--;
            }
        }
    }
    
    return count;
}

