#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned int)time(NULL)); //seed값으로 현재시간 부여 
    printf("난수 : %d\n", rand());
    printf("난수 : %d\n", rand());
    printf("난수 : %d\n", rand());
    printf("난수 : %d\n", rand());
    printf("난수 : %d\n", rand());
    return 0;
}