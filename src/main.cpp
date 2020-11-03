#include <iostream>

#include "matrix4.hpp"
#include "vector3.hpp"

int main() {
    Vector3 vec3(1, 2, 3);
    vec3.print(); 
    Vector4 vec4(0, 1, 2, 3);
    vec4.print();
    vec4 += vec3;
    vec4.print();
    return 0;
}
