#ifndef FRAGMENT_PROGRAM_H
#define FRAGMENT_PROGRAM_H
//
// Graphics Framework.
// Copyright (C) 2007 Department of Computer Science, University of Copenhagen
//
#include <iostream>
#include <iomanip>
#include "graphics/graphics.h"
#include "solution/math_types.h"

namespace graphics {

/*******************************************************************\
*                                                                   *
*                       P a s s T h r o u g h                       *
*                                                                   *
\*******************************************************************/

    template<typename math_types>
    class MyIdentityFragmentProgram : public FragmentProgram<math_types>
    {
    public:

	typedef typename FragmentProgram<math_types>::graphics_state_type graphics_state_type;
	typedef typename math_types::vector3_type                         vector3_type;
	typedef typename math_types::real_type                            real_type;

    public:
	void run(graphics_state_type const& state,
		 vector3_type const& in_position,
		 vector3_type const& in_normal,
		 vector3_type const& in_color,
		 vector3_type&       out_color)
	{
	    // >> TODO ADD YOUR OWN MAGIC HERE <<

	    //std::cout << "fragment: [" << in_position << "]" << std::endl;
	    out_color = in_color;
	}
    };


/*******************************************************************\
*                                                                   *
*                             P h o n g                             *
*                                                                   *
\*******************************************************************/

template<typename math_types>
    class MyPhongFragmentProgram : public FragmentProgram<math_types>
    {
    public:

	typedef typename FragmentProgram<math_types>::graphics_state_type graphics_state_type;
	typedef typename math_types::vector3_type                         vector3_type;
	typedef typename math_types::real_type                            real_type;

    public:
	void run(graphics_state_type const& state,
		 vector3_type const& in_position,
		 vector3_type const& in_normal,
		 vector3_type const& in_color,
		 vector3_type&       out_color)
	{
	    // >> TODO ADD YOUR OWN MAGIC HERE <<

	    // Implement the Phong reflection model using the values saved in state
            vector3_type ambient;
            ambient[1] = state.ambient_intensity() * state.ambient_color()[1] * state.I_a()[1];
            ambient[2] = state.ambient_intensity() * state.ambient_color()[2] * state.I_a()[2];
            ambient[3] = state.ambient_intensity() * state.ambient_color()[3] * state.I_a()[3];

            vector3_type L = state.light_position() - in_position;
            L =  L/Norm(L);
            vector3_type V = state.eye_position() - in_position;
            V =  V/Norm(V);
            vector3_type N = in_normal;
            N =  Zero(N) ? N : N/Norm(N);
            

            vector3_type diffuse;
            diffuse[1] = state.I_p()[1] * state.diffuse_intensity() * state.diffuse_color()[1] * Dot(N, L);
            diffuse[2] = state.I_p()[2] * state.diffuse_intensity() * state.diffuse_color()[2] * Dot(N, L);
            diffuse[3] = state.I_p()[3] * state.diffuse_intensity() * state.diffuse_color()[3] * Dot(N, L);

            vector3_type R;
            R[1] = 2*N[1] * Dot(N,L) - L[1];
            R[2] = 2*N[2] * Dot(N,L) - L[2];
            R[3] = 2*N[3] * Dot(N,L) - L[3];

            vector3_type specular;
            specular[1] = state.I_p()[1] * state.specular_intensity() * state.specular_color()[1] * pow(Dot(R,V),state.fall_off());
            specular[2] = state.I_p()[2] * state.specular_intensity() * state.specular_color()[2] * pow(Dot(R,V),state.fall_off());
            specular[3] = state.I_p()[3] * state.specular_intensity() * state.specular_color()[3] * pow(Dot(R,V),state.fall_off());
            
           
            /*
              Blanding mellem phong1 og 4. K ganges med O.
              L = light positiom
              N = normal vektor for punktet / in_normal
              V = er position man kigger fra
              R = 2N(N*L)-L
              n = falloff i state
              O = farven fra state.
             */

            out_color = ambient + diffuse + specular;
            out_color[1] = Clamp(out_color[1]);
            out_color[2] = Clamp(out_color[2]);
            out_color[3] = Clamp(out_color[3]);    
	}


    private:


/*******************************************************************\
*                                                                   *
*                  C l a m p ( r e a l _ t y p e )                  *
*                                                                   *
\*******************************************************************/

	real_type Clamp(real_type const& value)
	{
	    real_type result = value;
	    if (value < 0.0) result = 0.0;
	    if (value > 1.0) result = 1.0;
	    
	    return result;
	}
    };


}// end namespace graphics

// FRAGMENT_PROGRAM_H
#endif
