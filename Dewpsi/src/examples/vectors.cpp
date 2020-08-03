//! [Add Vectors]
Vector3D vec1(5, 3, 1);
Vector3D vec2(1, 1, 1);
Vector3D vec3 = vec1 + vec2;
//! [Add Vectors]

//! [Subtract Vectors]
Vector3D vec1(5, 3, 1);
Vector3D vec2(1, 1, 1);
Vector3D vec3 = vec1 - vec2;
//! [Subtract Vectors]

//! [Multiply Vectors]
Vector3D vec1(5, 3, 1);
Vector3D vec2(1, 1, 1);
Vector3D vec3 = vec1 * vec2;
//! [Multiply Vectors]

//! [Divide Vectors]
Vector3D vec1(5, 3, 1);
Vector3D vec2(1, 1, 1);
Vector3D vec3 = vec1 / vec2;
//! [Divide Vectors]

//! [Initialize Vector]
int x = 1, y = 2, z = 3;
Vector3D vec = {x, y, z};
//! [Initialize Vector]

//! [Bool Equal]
Vector3D vecA = {1, 2, 0};
Vector3D vecB = vecA;
Vector3D vecC = {vecA.x, 5, vecA.z};

if (vecA == vecB)
    // do something

if (vecA == vecC)
    // this code is not executed because vecA is not equal to vecC
//! [Bool Equal]

//! [Bool Less]
Vector3D vecA = {1, 2, 0};
Vector3D vecB = {5, 5, 5};

if (vecA < vecB)
    // code that will be jumped to
//! [Bool Less]

//! [Bool LessEqual]
Vector3D vecA = {4, 5, 1};
Vector3D vecB = {5, 5, 5};

if (vecA <= vecB)
    // code that will be jumped to
//! [Bool LessEqual]

//! [Bool Great]
Vector3D vecA = {6, 7, 8};
Vector3D vecB = {5, 5, 5};

if (vecA > vecB)
    // code that will be jumped to
//! [Bool Great]

//! [Bool GreatEqual]
Vector3D vecA = {6, 7, 8};
Vector3D vecB = {6, 6, 6};

if (vecA >= vecB)
    // code that will be jumped to
//! [Bool GreatEqual]
