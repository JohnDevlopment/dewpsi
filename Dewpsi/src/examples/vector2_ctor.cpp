default             (1) TVector2D();
copy                (2) TVector2D(const TVector2D& rhs);
move                (3) TVector2D(TVector2D&& rhs);
from x & y          (4) TVector2D(const value_type& x, const value_type& y);
from val            (5) TVector2D(const value_type& val);
initializer list    (6) TVector2D(std::initializer_list<Type> il);
