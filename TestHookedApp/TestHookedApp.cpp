#include <Windows.h>
#include <iostream>

int main() {
    while (true) {
        Sleep(1000);
        std::cout << "sleeping...\n";
    }

    return 0;
}
