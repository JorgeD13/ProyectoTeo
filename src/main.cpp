#include "execute.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    int key;
    std::cin >> key;
    execute exe(key);
    exe.exe();

    return EXIT_SUCCESS;
}
