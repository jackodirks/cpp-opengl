#include <iostream>

#include "matrix4.hpp"

int main() {
    std::cout << "Hello World!" << std::endl;
    Matrix4 mat;
    mat.print();
    std::cout << std::endl;
    std::array<float, 16> arr = { 2, 0, 0, 0,
        0, 3, 0, 0,
        0, 0, 4, 0,
        0, 0, 0, 5};
    Matrix4 mat2(arr);
    mat2.print();
    std::cout << std::endl;
    arr[4] = 8;
    mat2.print();
    std::cout << std::endl;
    Matrix4 mat3(mat2);
    mat3.print();
    mat3 = mat3 * 2;
    std::cout << std::endl;
    mat3.print();
    std::cout << std::endl;
    Matrix4 mat4 = mat3 * mat2;
    mat4.print();
    std::cout << std::endl;
    mat4 = mat4 + mat2;
    mat4.print();
    std::cout << std::endl;
    mat4 = mat4 - mat2;
    mat4.print();
    std::cout << std::endl;
    return 0;
}
