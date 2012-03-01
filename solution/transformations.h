#ifndef TRANSFORMATIONS
#define TRANSFORMATIONS

#include <iostream>
#include <iomanip>
#include <cmath>

#include "graphics/graphics.h"
#include "solution/math_types.h"


/*******************************************************************\
*                                                                   *
*                   T r a n s f o r m a t i o n s                   *
*                                                                   *
\*******************************************************************/


namespace graphics {

/*******************************************************************\
*                                                                   *
*                           E p s i l o n                           *
*                                                                   *
\*******************************************************************/

#define Epsilon 1.0e-06


/*******************************************************************\
*                                                                   *
*                  Z e r o ( r e a l _ t y p e & )                  *
*                                                                   *
\*******************************************************************/

    bool Zero(MyMathTypes::real_type const& value, MyMathTypes::real_type epsilon = Epsilon)
    {
	return std::fabs(value) < epsilon;
    }


/*******************************************************************\
*                                                                   *
*               Z e r o ( v e c t o r 2 _ t y p e & )               *
*                                                                   *
\*******************************************************************/

    bool Zero(MyMathTypes::vector2_type const& vec, MyMathTypes::real_type epsilon = Epsilon)
    {
	bool result = false;
	if ((Zero(vec[1]), epsilon) && (Zero(vec[2], epsilon))) result = true;

	return result;
    }


/*******************************************************************\
*                                                                   *
*               Z e r o ( v e c t o r 3 _ t y p e & )               *
*                                                                   *
\*******************************************************************/

    bool Zero(MyMathTypes::vector3_type const& vec, MyMathTypes::real_type epsilon = Epsilon)
    {
	bool result = Zero(vec[1], epsilon);
	for (int i = 2; i <= 3; ++i) {
	    result = result && Zero(vec[i], epsilon);
	}

	return result;
    }


/*******************************************************************\
*                                                                   *
*       L e s s T h a n Z e r o ( 2   x   r e a l _ t y p e )       *
*                                                                   *
\*******************************************************************/

bool LessThanZero(MyMathTypes::real_type const& value,  double const& epsilon = Epsilon)
    {
	bool result;
	double dvalue = static_cast<double>(value);
       
	result = (dvalue < epsilon) ? true : false;
	
	return result;
    }



/*******************************************************************\
*                                                                   *
*    G r e a t e r T h a n Z e r o ( 2   x   r e a l _ t y p e )    *
*                                                                   *
\*******************************************************************/

    bool GreaterThanZero(MyMathTypes::real_type const& value,  double const& epsilon = Epsilon)
    {
	bool result;
	double dvalue = static_cast<double>(value);
       
	result = (dvalue > epsilon) ? true : false;
	
	return result;
    }


/*******************************************************************\
*                                                                   *
*          H o m V e c t o r ( v e c t o r 3 _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::vector4_type HomVector(MyMathTypes::vector3_type const& vec3d)
    {
	MyMathTypes::vector4_type homvector;

	for (int i = 1; i <= 3; ++i) {
	    homvector[i] = vec3d[i];
	}
	homvector[4] = 1;

	return homvector;
    }


/*******************************************************************\
*                                                                   *
*           V e c t o r 3 D ( v e c t o r 4 _ t y p e & )           *
*                                                                   *
\*******************************************************************/

    MyMathTypes::vector3_type Vector3D(MyMathTypes::vector4_type const& homvector)
    {
	MyMathTypes::vector3_type Euclidean_vector;
	
	if (homvector[4] == 0)
	    throw std::runtime_error("Vector3D(vector4_type&): The forth coordinate is equal to zero --- a point at infinity");
	else {
	    MyMathTypes::real_type factor = homvector[4];

	    for (int i = 1; i <= 3; ++i) {
		Euclidean_vector[i] = homvector[i] / factor;
	    }	    
	}
	return Euclidean_vector;
    }


/*******************************************************************\
*                                                                   *
*          H o m N o r m a l ( v e c t o r 3 _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::vector4_type HomNormal(MyMathTypes::vector3_type const& normal3d)
    {
	MyMathTypes::vector4_type homnormal;
    
	for (int i = 1; i <= 3; ++i) {
	    homnormal[i] = normal3d[i];
	}
	homnormal[4] = 0;
	
	return homnormal;
    }


/*******************************************************************\
*                                                                   *
*           N o r m a l 3 D ( v e c t o r 3 _ t y p e & )           *
*                                                                   *
\*******************************************************************/

    MyMathTypes::vector3_type Normal3D(MyMathTypes::vector4_type const& homnormal)
    {
	MyMathTypes::vector3_type Euclidean_normal;
	
	//if (homnormal[4] != 0) {
	if (!Zero(homnormal[4])) {
	    //throw std::runtime_error("Normal3D(vector4_type&): The forth coordinate is not equal to zero --- a point at infinity");
	    std::cout << "Normal3D(vector4_type&): N = [" << homnormal << "] - The forth coordinate is not equal to zero." << std::endl;

	    MyMathTypes::real_type factor = homnormal[4];

	    for (int i = 1; i <= 3; ++i) {
		Euclidean_normal[i] = homnormal[i] / factor;
	    }
	}
	else {
	    for (int i = 1; i <= 3; ++i) {
		Euclidean_normal[i] = homnormal[i];
	    }	    
	}
	return Euclidean_normal;
    }



/*******************************************************************\
*                                                                   *
*                        I d e n t i t y ( )                        *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Identity()
    {
	MyMathTypes::matrix4x4_type I;    // The identity matrix.
	
	I[1][1] = 1, I[1][2] = 0, I[1][3] = 0, I[1][4] = 0;
	I[2][1] = 0, I[2][2] = 1, I[2][3] = 0, I[2][4] = 0; 
	I[3][1] = 0, I[3][2] = 0, I[3][3] = 1, I[3][4] = 0; 
	I[4][1] = 0, I[4][2] = 0, I[4][3] = 0, I[4][4] = 1;

	return I;
    }


/*******************************************************************\
*                                                                   *
*          T r a n s l a t e ( 3   x   r e a l _ t y p e )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Translate(MyMathTypes::real_type const& dx,
					  MyMathTypes::real_type const& dy,
					  MyMathTypes::real_type const& dz)
    {
	MyMathTypes::matrix4x4_type T(Identity());

	// Do your own magic here.
        T[1][4] = dx;
        T[2][4] = dy;
        T[3][4] = dz;

	return T;
    }


/*******************************************************************\
*                                                                   *
*          T r a n s l a t e ( v e c t o r 3 _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Translate(MyMathTypes::vector3_type const& d)
    {
	MyMathTypes::matrix4x4_type T(Translate(d[1], d[2], d[3]));
	return T;
    }


/*******************************************************************\
*                                                                   *
*      I n v T r a n s l a t e ( 3   x   r e a l _ t y p e & )      *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvTranslate(MyMathTypes::real_type const& dx,
					     MyMathTypes::real_type const& dy,
					     MyMathTypes::real_type const& dz)
    {
	MyMathTypes::matrix4x4_type invT(Identity());

	// Do your own magic here.
        invT[1][4] = -dx;
        invT[2][4] = -dy;
        invT[3][4] = -dz;

	return invT;
    }


/*******************************************************************\
*                                                                   *
*       I n v T r a n s l a t e ( v e c t o r 3 _ t y p e & )       *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvTranslate(MyMathTypes::vector3_type const& d)
    {
	MyMathTypes::matrix4x4_type InvT(InvTranslate(d[1], d[2], d[3]));
	return InvT;
    }


/*******************************************************************\
*                                                                   *
*              S c a l e ( 3   x   r e a l _ t y p e )              *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Scale(MyMathTypes::real_type const& sx,
				      MyMathTypes::real_type const& sy,
				      MyMathTypes::real_type const& sz)
    {
	MyMathTypes::matrix4x4_type S(Identity());

	// Do your own magic here.
        S[1][1] = sx;
        S[2][2] = sy;
        S[3][3] = sz;

	return S;
    }


/*******************************************************************\
*                                                                   *
*          I n v S c a l e ( 3   x   r e a l _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvScale(MyMathTypes::real_type const& sx,
					 MyMathTypes::real_type const& sy,
					 MyMathTypes::real_type const& sz)
    {
	MyMathTypes::matrix4x4_type invS(Identity());

	if ((sx == 0.0) || (sy == 0.0) || (sz == 0.0))
	    throw std::runtime_error("InvScale(3 x real_type&): one or more of the scale factors are equal to zero");
	else {
	    // Do your own magic here.
        invS[1][1] = 1/sx;
        invS[2][2] = 1/sy;
        invS[3][3] = 1/sz;
	}
	
	return invS;
    }


/*******************************************************************\
*                                                                   *
*              S c a l e ( v e c t o r 3 _ t y p e & )              *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Scale(MyMathTypes::vector3_type const& s)
    {
	MyMathTypes::matrix4x4_type S(Scale(s[1], s[2], s[3]));
	return S;
    }


/*******************************************************************\
*                                                                   *
*           I n v S c a l e ( v e c t o r 3 _ t y p e & )           *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvScale(MyMathTypes::vector3_type const& s)
    {
	MyMathTypes::matrix4x4_type InvS(InvScale(s[1], s[2], s[3]));
	return InvS;
    }


/*******************************************************************\
*                                                                   *
*              X _ R o t a t e ( r e a l _ t y p e & )              *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type X_Rotate(MyMathTypes::real_type const& angle)
    {
	MyMathTypes::matrix4x4_type R_x(Identity());

	// Do your own magic here.
        R_x[2][2] = cos(angle);
        R_x[2][3] = -sin(angle);
        R_x[3][2] = sin(angle);
        R_x[3][3] = cos(angle);

	return R_x;
    }


/*******************************************************************\
*                                                                   *
*        I n v _ X _ R o t a t i o n ( r e a l _ t y p e & )        *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_X_Rotate(MyMathTypes::real_type const& angle)
    {
        MyMathTypes::matrix4x4_type Inv_R_x(Identity());

	// Do your own magic here.
        Inv_R_x[2][2] = cos(angle);
        Inv_R_x[2][3] = sin(angle);
        Inv_R_x[3][2] = -sin(angle);
        Inv_R_x[3][3] = cos(angle);

	return Inv_R_x;
    }


/*******************************************************************\
*                                                                   *
*              Y _ R o t a t e ( r e a l _ t y p e & )              *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Y_Rotate(MyMathTypes::real_type const& angle)
    {
	MyMathTypes::matrix4x4_type R_y(Identity());

	// Do your own magic here.
        R_y[1][1] = cos(angle);
        R_y[1][3] = sin(angle);
        R_y[3][1] = -sin(angle);
        R_y[3][3] = cos(angle);

	return R_y;
    }


/*******************************************************************\
*                                                                   *
*        I n v _ Y _ R o t a t i o n ( r e a l _ t y p e & )        *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_Y_Rotate(MyMathTypes::real_type const& angle)
    {
        MyMathTypes::matrix4x4_type Inv_R_y(Identity());

	// Do your own magic here.
        Inv_R_y[1][1] = cos(angle);
        Inv_R_y[1][3] = -sin(angle);
        Inv_R_y[3][1] = sin(angle);
        Inv_R_y[3][3] = cos(angle);

	return Inv_R_y;
    }



/*******************************************************************\
*                                                                   *
*              Z _ R o t a t e ( r e a l _ t y p e & )              *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Z_Rotate(MyMathTypes::real_type const& angle)
    {
	MyMathTypes::matrix4x4_type R_z(Identity());

	// Do your own magic here.
	R_z[1][1] = cos(angle);
	R_z[1][2] = -sin(angle);
	R_z[2][1] = sin(angle);
	R_z[2][2] = cos(angle);

	return R_z;
    }


/*******************************************************************\
*                                                                   *
*        I n v _ Z _ R o t a t i o n ( r e a l _ t y p e & )        *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_Z_Rotate(MyMathTypes::real_type const& angle)
    {
        MyMathTypes::matrix4x4_type Inv_R_z(Identity());

	// Do your own magic here.
	Inv_R_z[1][1] = cos(angle);
	Inv_R_z[1][2] = sin(angle);
	Inv_R_z[2][1] = -sin(angle);
	Inv_R_z[2][2] = cos(angle);

	return Inv_R_z;
    }


/*******************************************************************\
*                                                                   *
* R o t a t e ( r e a l _ t y p e & ,   v e c t o r 3 _ t y p e & ) *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Rotate(MyMathTypes::real_type    const& angle,
				       MyMathTypes::vector3_type const& axis)
    {	
	MyMathTypes::matrix4x4_type R(Identity());
	
	// Do your own magic here.
        double ux = axis[1];
        double uy = axis[2];
        double uz = axis[3];

        R[1][1] = ux*ux + (1 - ux*ux)      * cos(angle);
        R[1][2] = ux*uy * (1 - cos(angle)) - uz*sin(angle);
        R[1][3] = ux*ux * (1 - cos(angle)) + uy*sin(angle);
        R[2][1] = ux*uy * (1 - cos(angle)) + uz*sin(angle);
        R[2][2] = uy*uy + (1 - uy*uy)      * cos(angle);
        R[2][3] = uy*uz * (1 - cos(angle)) - ux*sin(angle);
        R[3][1] = ux*uz * (1 - cos(angle)) - uy*sin(angle);
        R[3][2] = uy*uz * (1 - cos(angle)) + ux*sin(angle);
        R[3][3] = uz*uz * (1 - uz*uz)      * cos(angle);

	return R;
    }

/*******************************************************************\
*                                                                   *
* I n v R o t a t e ( r e a l _ t y p e & ,   v e c t o r 3 _ t y p e & )*
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvRotate(MyMathTypes::real_type    const& angle,
					  MyMathTypes::vector3_type const& axis)
    {
        MyMathTypes::matrix4x4_type Inv_R(Identity());

	Inv_R = Inverse(Rotate(angle,axis));

	return Inv_R;
    }


/*******************************************************************\
*                                                                   *
*          X Y _ S h e a r ( 2   x   r e a l _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type XY_Shear(MyMathTypes::real_type const& sh_x,
					 MyMathTypes::real_type const& sh_y)
    {
	MyMathTypes::matrix4x4_type XY_shear(Identity());

	// Do your own magic here.
        XY_shear[1][3] = sh_x;
        XY_shear[2][3] = sh_y;

	return XY_shear;
    }


/*******************************************************************\
*                                                                   *
*      I n v _ X Y _ S h e a r ( 2   x   r e a l _ t y p e & )      *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_XY_Shear(MyMathTypes::real_type const& sh_x,
					     MyMathTypes::real_type const& sh_y)
    {
	MyMathTypes::matrix4x4_type inv_XY_shear(Identity());

	// Do your own magic here.
        inv_XY_shear[1][3] = -sh_x;
        inv_XY_shear[2][3] = -sh_y;

	return inv_XY_shear;
    }


/*******************************************************************\
*                                                                   *
*          X Z _ S h e a r ( 2   x   r e a l _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type XZ_Shear(MyMathTypes::real_type const& sh_x,
					 MyMathTypes::real_type const& sh_z)
    {
	MyMathTypes::matrix4x4_type XZ_shear(Identity());

	// Do your own magic here.
        XZ_shear[1][2] = sh_x;
        XZ_shear[3][2] = sh_z;

	return XZ_shear;
    }


/*******************************************************************\
*                                                                   *
*      I n v _ X Z _ S h e a r ( 2   x   r e a l _ t y p e & )      *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_XZ_Shear(MyMathTypes::real_type const& sh_x,
					     MyMathTypes::real_type const& sh_z)
    {
	MyMathTypes::matrix4x4_type inv_XZ_shear(Identity());

	// Do your own magic here.
        inv_XZ_shear[1][2] = -sh_x;
        inv_XZ_shear[3][2] = -sh_z;

	return inv_XZ_shear;
    }


/*******************************************************************\
*                                                                   *
*          Y Z _ S h e a r ( 2   x   r e a l _ t y p e & )          *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type YZ_Shear(MyMathTypes::real_type const& sh_y,
					 MyMathTypes::real_type const& sh_z)
    {
	MyMathTypes::matrix4x4_type YZ_shear(Identity());

	// Do your own magic here.
        YZ_shear[2][1] = sh_y;
        YZ_shear[3][1] = sh_z;

	return YZ_shear;
    }


/*******************************************************************\
*                                                                   *
*      I n v _ Y Z _ S h e a r ( 2   x   r e a l _ t y p e & )      *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Inv_YZ_Shear(MyMathTypes::real_type const& sh_y,
					     MyMathTypes::real_type const& sh_z)
    {
	MyMathTypes::matrix4x4_type inv_YZ_shear(Identity());

	// Do your own magic here.
        inv_YZ_shear[2][1] = -sh_y;
        inv_YZ_shear[3][1] = -sh_z;

	return inv_YZ_shear;
    }


/*******************************************************************\
*                                                                   *
*P e r s p e c t i v e _ t o _ P a r a l l e l (r e a l _ t y p e &)*
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Perspective_to_Parallel(MyMathTypes::real_type const& Zmax)
    {
	MyMathTypes::matrix4x4_type M(Identity());

	// Do your own magic here.

	return M;
    }


/*******************************************************************\
*                                                                   *
* I n v P e r s p e c t i v e _ t o _ P a r a l l e l ( r e a l _ t y p e & )*
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type InvPerspective_to_Parallel(MyMathTypes::real_type const& Zmax)
    {
	MyMathTypes::matrix4x4_type M(Identity());

	if (Zero(Zmax)) {
	    throw std::runtime_error("InvPerspective_to_Parallel(real_type&): The matrix is not invertable, because Zmax == 0");
	}

	// Do your own magic here.

	return M;
    }

/*******************************************************************\
*                                                                   *
*   W i n d o w _ V i e w P o r t ( 2   x   r e a l _ t y p e & )   *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Window_ViewPort(MyMathTypes::real_type const& width,
						MyMathTypes::real_type const& height)
    {
      MyMathTypes::matrix4x4_type window_viewport(Identity());
	
	// Do your own magic here.

	return window_viewport;
    }


/*******************************************************************\
*                                                                   *
*    W i n d o w _ V i e w P o r t ( v e c t o r 2 _ t y p e & )    *
*                                                                   *
\*******************************************************************/

    MyMathTypes::matrix4x4_type Window_ViewPort(MyMathTypes::vector2_type const& window_dimensions)
    {
	MyMathTypes::matrix4x4_type WindowViewPort = Window_ViewPort(window_dimensions[1],
								     window_dimensions[2]);
	return WindowViewPort;
    }

}

#endif
