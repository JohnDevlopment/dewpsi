default             (1) TVector3D();
copy                (2) TVector3D(const TVector3D& rhs);
move                (3) TVector3D(TVector3D&& rhs);
from x & y          (4) TVector3D(const value_type& x, const value_type& y);
from val            (5) TVector3D(const value_type& val);
initializer list    (6) TVector3D(std::initializer_list<Type> il);
