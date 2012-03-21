#ifndef DINI_H
#define DINI_H

#include "solution/math_types.h"
#include "solution/transformations.h"


/*******************************************************************\
*                                                                   *
*                       K l e i n B o t t l e                       *
*                                                                   *
\*******************************************************************/

class Dini {
public:
    typedef MyMathTypes::real_type    real_type;
    typedef MyMathTypes::vector3_type vector3_type;

public:
    Dini()
    {}

    virtual ~Dini()
    {}

    void init()
    {
        a = 1.0;
        b = 0.2;
    }


    vector3_type Vertex(real_type const& u, real_type const& v) const
    {
	vector3_type point;
	point[1] = a * cos(u) * sin(v);
	point[2] = a * sin(u) * sin(v);
	point[3] = a*(cos(v) + log(tan(0.5*v))) + b*u;

	//std::cout << "KleinBottomPoint(" << u << ", " << v << ") == [" << point << "]" << std::endl;

	return point;
    }
    
    vector3_type Normal(real_type const& u,real_type const&  v) const
    {
	vector3_type normal, n1, n2;

        n1[1] = -a * sin(u) * sin(v);
        n1[2] =  a * cos(u) * cos(v);
        n1[3] =  b;

        n2[1] = a * cos(u) * cos(v);
        n2[2] = -a * sin(u) * sin(v);
        n2[3] = (0.5*a*cos(v))/(sin(0.5*v)*cos(0.5*v));

        normal = Cross(n1,n2);
	
	
	if (!Zero(normal)) normal /= Norm(normal);
	normal = - normal;

	//std::cout << "KleinBottomNormal(" << u << ", " << v << ") == [" << normal << "]" << std::endl;

	return normal;
    }

private:
    real_type a;
    real_type b;

};

#endif
