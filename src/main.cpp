#include <iostream>

#include "matrix4.hpp"
#include "vector3.hpp"
#include "openglMatrix.hpp"
 
int main() {
    OpenGlMatrix mat;
    mat.rotate(1);
    mat.print();
    return 0;
}
