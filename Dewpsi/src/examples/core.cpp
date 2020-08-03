//! [Offsetof]
struct Vector3D {
    float x, y, z;
};

void main()
{
    size_t szOffset = PD_OFFSETOF(Vector3D, y); // should be 4
}
//! [Offsetof]
