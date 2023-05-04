#include <stdio.h>
#include <unistd.h>

int main() {
    
    // fork a chile process
    fork();

    // fork another child process
    fork();

    // and fork another
    fork();

    return 0;
}