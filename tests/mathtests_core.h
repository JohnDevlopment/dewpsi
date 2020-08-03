#ifndef MATHTESTS_CORE_H
#define MATHTESTS_CORE_H

#include <Dewpsi_Vector.h>
//#include <Dewpsi_Core.h>

/// A 4x4 matrix.
class Mat4 {
public:
    Mat4();
    
private:
    float m_faValues[4][4];
};

#endif /* MATHTESTS_CORE_H */
