#include <bl/vector.h>
#include <iostream>

int main() {
    bl::MyVector vec;
    vec.push_back(42);

    std::cout << "Application Running. Vector size: "
              << vec.data.size() << "\n";
    return 0;
}
