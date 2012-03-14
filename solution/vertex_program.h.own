#ifndef VERTEX_PROGRAM_H
#define VERTEX_PROGRAM_H
//
// Graphics Framework.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen
//
#include "graphics/graphics.h"
#include "graphics/graphics_state.h"
#include "solution/transformations.h"


namespace graphics {

/*******************************************************************\
*                                                                   *
*                       P a s s T h r o u g h                       *
*                                                                   *
\*******************************************************************/

    template<typename math_types>
    class MyIdentityVertexProgram : public VertexProgram<math_types>
    {
    public:
	typedef typename VertexProgram<math_types>::graphics_state_type  graphics_state_type;
	typedef typename math_types::vector3_type                        vector3_type;

    public:
	void run(graphics_state_type const& state,
		 vector3_type const& in_vertex,
		 vector3_type const& in_color,
		 vector3_type& out_vertex,
		 vector3_type& out_color)
	{
	    // >> ADD YOUR OWN MAGIC HERE <<
            //& NO!
	    out_vertex = in_vertex;
	    out_color =  in_color;
	}


	void run(graphics_state_type const& state,
		 vector3_type const& in_vertex,
		 vector3_type const& in_normal,
		 vector3_type const& in_color,
		 vector3_type& out_vertex,
		 vector3_type& out_normal,
		 vector3_type& out_color)
	{
	    // >> ADD YOUR OWN MAGIC HERE <<
            // NO!
	    out_vertex = in_vertex;
	    out_normal = in_normal;
	    out_color =  in_color;
	}
    };


/*******************************************************************\
*                                                                   *
*                         T r a n s f o r m                         *
*                                                                   *
\*******************************************************************/

    template<typename math_types>
    class MyTransformVertexProgram : public VertexProgram<math_types>
    {
    public:
	typedef typename VertexProgram<math_types>::graphics_state_type  graphics_state_type;
	typedef typename math_types::vector3_type                        vector3_type;
	typedef typename math_types::vector4_type                        vector4_type;
	typedef typename math_types::matrix4x4_type                      matrix4x4_type;

    public:
	void run(graphics_state_type const& state,
		 vector3_type const& in_vertex,
		 vector3_type const& in_color,
		 vector3_type& out_vertex,
		 vector3_type& out_color)
	{
	    // >> ADD YOUR OWN MAGIC HERE <<
            // NO!
	    out_vertex = this->TransformPoint(state, in_vertex);
	    out_color =  in_color;
	}


	void run(graphics_state_type const& state,
		 vector3_type const& in_vertex,
		 vector3_type const& in_normal,
		 vector3_type const& in_color,
		 vector3_type& out_vertex,
		 vector3_type& out_normal,
		 vector3_type& out_color)
	{
	    // >> ADD YOUR OWN MAGIC HERE <<
            // NO!
	    out_vertex = this->TransformPoint(state,  in_vertex);
	    out_normal = this->TransformNormal(state, in_normal);
	    out_color =  in_color;
	}

    private:
	vector3_type TransformPoint(graphics_state_type const& state, vector3_type const& point)
	{
	    // Transform the point using the state.model()
            // DO MAGIC HERE!

            vector4_type hom = HomVector(point);

	    vector4_type transformed_point = state.projection() * hom;
	    return Vector3D(transformed_point);
	}

	
	vector3_type TransformNormal(graphics_state_type const& state, vector3_type const& normal)
	{
	    // Transform the normal using the state.inv_model()
            // DO MAGIC HERE!!! :D

            vector4_type hom = HomVector(normal);

	    vector4_type transformed_normal = state.projection() * hom;
	    return Vector3D(transformed_normal);
	}
    };

}// end namespace graphics

// VERTEX_PROGRAM_H
#endif
