#ifndef TRIANGLE_RASTERIZER_H
#define TRIABGLE_RASTERIZER_H
//
// Graphics Framework.
// Copyright (C) 2010 Department of Computer Science, University of Copenhagen
//

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include "graphics/graphics.h"
#include "solution/edge_rasterizer.h"
#include "solution/linear_interpolator.h"
#include "solution/transformations.h"


// remove: #define NEWSCANLINESEARCH


namespace graphics {

    template<typename math_types>
    class MyTriangleRasterizer : public Rasterizer<math_types>
    {
    public:
	typedef typename math_types::vector3_type vector3_type;
	typedef typename math_types::real_type    real_type;


    public:


/*******************************************************************\
*                                                                   *
*            M y T r i a n g l e R a s t e r i z e r ( )            *
*                                                                   *
\*******************************************************************/

	MyTriangleRasterizer() : valid(false), Debug(false)
	{}

/*******************************************************************\
*                                                                   *
*           ~ M y T r i a n g l e R a s t e r i z e r ( )           *
*                                                                   *
\*******************************************************************/

	virtual ~MyTriangleRasterizer()
	{}


/*******************************************************************\
*                                                                   *
*                         i n i t ( . . . )                         *
*                                                                   *
\*******************************************************************/

	void init( vector3_type const& in_vertex1,
		   vector3_type const& in_normal1,
		   vector3_type const& in_worldpoint1,
		   vector3_type const& in_color1,
		   vector3_type const& in_vertex2,
		   vector3_type const& in_normal2,
		   vector3_type const& in_worldpoint2,
		   vector3_type const& in_color2,
		   vector3_type const& in_vertex3,
		   vector3_type const& in_normal3,
		   vector3_type const& in_worldpoint3,
		   vector3_type const& in_color3) 
	{
	    // This is a triangle rasterizer

	    // Save the original parameters
            org_vertex[0] = in_vertex1;
            org_vertex[1] = in_vertex2;
            org_vertex[2] = in_vertex3;
            org_normal[0] = in_normal1;
            org_normal[1] = in_normal2;
            org_normal[2] = in_normal3;
            org_worldpoint[0] = in_worldpoint1;
            org_worldpoint[1] = in_worldpoint2;
            org_worldpoint[2] = in_worldpoint3;
            org_color[0] = in_color1;
            org_color[1] = in_color2;
            org_color[2] = in_color3;

            ivertex[0] = in_vertex1;
            ivertex[1] = in_vertex2;
            ivertex[2] = in_vertex3;

            int lower_left = LowerLeft(); //returns 0-indexed!
            int upper_left = UpperLeft();
            int the_other = 3 - lower_left - upper_left;

            ll = org_vertex[lower_left];
            ul = org_vertex[upper_left];
            other = org_vertex[the_other];

            vector3_type e1, e2;
            e1 = ll - ul;
            e2 = other - ll;
            int x1, y1, x2, y2;
            x1 = e1[1];
            y1 = e1[2];
            x2 = e2[1];
            y2 = e2[2];

            cred = vector3_type(1, 0, 0);
            cyellow = vector3_type(1, 1, 0);
            cgreen = vector3_type(0,1,0);

            
	    if (((x1*y2)-(x2*y1)) < 0) {
                leftedge.init(ll,ll,ll,org_color[lower_left],
                              other,other,other,org_color[the_other],
                              ul,ul,ul,org_color[upper_left]);

                rightedge.init(ll,ll,ll,org_color[lower_left],
                               ul,ul,ul,org_color[upper_left]);
            }
            if  (((x1*y2)-(x2*y1)) > 0){
                rightedge.init(ll,ll,ll,org_color[lower_left],
                              other,other,other,org_color[the_other],
                              ul,ul,ul,org_color[upper_left]);

                leftedge.init(ll,ll,ll,org_color[lower_left],
                               ul,ul,ul,org_color[upper_left]);
            }

            
            x_start = leftedge.x();
            y_start = leftedge.y();
            z_start = 0;

            x_current = x_start;
            y_current = y_start;
            z_current = z_start;

            x_stop = rightedge.x();
            y_stop = leftedge.y();
            z_stop = 0;
            
            Nleft = org_normal[lower_left];
            Nright = org_normal[the_other];
            Ncurrent = Nleft;

            Worldleft = org_worldpoint[lower_left];
            Worldright = org_worldpoint[the_other];
            Worldcurrent = Worldleft;
	    
	    // Do your magic stuff here.
            this->degen = false;
	    if (this->degenerate()) {
		this->valid = false;
                this->degen = true;
		//throw std::runtime_error("MyTriangleRasterizer:: The triangle is degenerate, i.e. all three points are collinear");
	    }
	    else {
		//std::cout << "MyTriangleRasterizer::init(...): Triangle not degenerate" << std::endl;
		this->initialize_triangle();
	    }
	}

/*******************************************************************\
*                          
                return;                                         *
*                         D e b u g O n ( )                         *
*                                                                   *
\*******************************************************************/

	bool DebugOn()
	{
	    bool oldvalue = this->Debug;
	    this->Debug = true;

	    return oldvalue;
	}


/*******************************************************************\
*                                                                   *
*                        D e b u g O f f ( )                        *
*                                                                   *
\*******************************************************************/

	bool DebugOff()
	{
	    bool oldvalue = this->Debug;
	    this->Debug = false;

	    return oldvalue;
	}


/*******************************************************************\
*                                                                   *
*                           V a l i d ( )                           *
*                                                                   *
\*******************************************************************/

	bool Valid() const
	{
	    return !(this->degenerate());
	}


/*******************************************************************\
*                                                                   *
*                      D e g e n e r a t e ( )                      *
*                                                                   *
\*******************************************************************/

	bool Degenerate() const
	{
            return this->degenerate();
	}


/*******************************************************************\
*                                                                   *
*                               x ( )                               *
*                                                                   *
\*******************************************************************/

	int x() const      
	{
            //if (!this->valid) {
            //    throw std::runtime_error("MyTriangleRasterizer::x(): Invalid State/Not Initialized");
            //}
            return this->x_current;
	}


/*******************************************************************\
*                                                                   *
*                               y ( )                               *
*                                                                   *
\*******************************************************************/

	int y() const
	{
            //if (!this->valid) {
            //    throw std::runtime_error("MyTriangleRasterizer::y(): Invalid State/Not Initialized");
            //}
            return this->y_current;
	}


/*******************************************************************\
*                                                                   *
*                           d e p t h ( )                           *
*                                                                   *
\*******************************************************************/

	real_type depth() const     
	{
	  //if (!this->valid) {
	  //    throw std::runtime_error("MyTriangleRasterizer::depth(): Invalid State/Not Initialized");
	  //}

	  //if (!this->depth_interpolator.more_values()) {
	  //	throw std::runtime_error("MyTriangleRasterizer::depth(): Invalid depth_interpolator");
	  //}

	    return this->depth_interpolator.value();
	}


/*******************************************************************\
*                                                                   *
*                        p o s i t i o n ( )                        *
*                                                                   *
\*******************************************************************/

	vector3_type position() const 
        {
	  //if (!this->valid) {
	  //    throw std::runtime_error("MyTriangleRasterizer::position(): Invalid State/Not Initialized");
	  //}

	  //if (!this->worldpoint_interpolator.more_values()) {
	  //	throw std::runtime_error("MyEdgeRasterizer::position(): Invalid worldpoint_interpolator");
	  //}

	    return this->worldpoint_interpolator.value();
	}


/*******************************************************************\
*                                                                   *
*                          n o r m a l ( )                          *
*                                                                   *
\*******************************************************************/

	vector3_type const& normal() const     
	{
	  //if (!this->valid) {
	  //    throw std::runtime_error("MyTriangleRasterizer::normal(): Invalid State/Not Iitialized");
	  //}
	    
	  //if (!this->normal_interpolator.more_values()) {
	  //	throw std::runtime_error("MyTriangleRasterizer::normal(): Invalid normal_interpolator");
	  //}

	    return this->normal_interpolator.value();
	}


/*******************************************************************\
*                                                                   *
*                           c o l o r ( )                           *
*                                                                   *
\*******************************************************************/

	vector3_type const& color() const 
	{
	  //if (!this->valid) {
	  //    throw std::runtime_error("MyTriangleRasterizer::color(): Invalid State/Not Initialized");
	  //}

	    //vector3_type return_color;
	    if (this->Debug) {
                return this->color_current;
	    }

	    //if (!this->color_interpolator.more_values()) {
	    //	throw std::runtime_error("MyEdgeRasterizer::color(): Invalid color_interpolator");
	    //}
            return this->color_current;
            //TODO: FIXME: OBS: The interpolator should do the coloring!
            // This is a hack to simulate colors. It will properly fuck
            // up if vertexes are different colors! :P
	    //return this->color_interpolator.value();
	}


/*******************************************************************\
*                                                                   *
*                 p r i n t _ v a r i a b l e s ( )                 *
*                                                                   *
\*******************************************************************/

	void print_variables()
        {
	    std::cout << "MyTriangleRasterizer: local variables" << std::endl;
	    std::cout << "=====================================" << std::endl;
	    std::cout << "\tvalid     == " << this->valid    << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tV1 = [" << this->org_vertex[0] << "]" << std::endl;
	    std::cout << "\tV2 = [" << this->org_vertex[1] << "]" << std::endl;
	    std::cout << "\tV3 = [" << this->org_vertex[2] << "]" << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tx_start   == " << this->x_start   << std::endl;
	    std::cout << "\ty_start   == " << this->y_start   << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tx_current == " << this->x_current << std::endl;
	    std::cout << "\ty_current == " << this->y_current << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tx_stop    == " << this->x_stop    << std::endl;
	    std::cout << "\ty_stop    == " << this->y_stop    << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tcolor_left    == " << this->color_left    << std::endl;
	    std::cout << "\tcolor_right   == " << this->color_right   << std::endl;
	    std::cout << "\tcolor_current == " << this->color_current << std::endl;
	    std::cout << std::endl;
	    std::cout << "\tDebug == " << this->Debug << std::endl;
	    std::cout << std::endl;
	}


/*******************************************************************\
*                                                                   *
*                  m o r e _ f r a g m e n t s ( )                  *
*                                                                   *
\*******************************************************************/

	bool more_fragments() const 
	{
            bool left = leftedge.more_fragments();
            bool right = rightedge.more_fragments();

            return (left && right && !this->degen );
	}


/*******************************************************************\
*                                                                   *
*                   n e x t _ f r a g m e n t ( )                   *
*                                                                   *
\*******************************************************************/

	void next_fragment()    
	{
	    // Do your magic stuff here
            if(x_current < x_stop - 1) {
                x_current++;
            } else {
                if (leftedge.more_fragments()) {
                    leftedge.next_fragment();
                    rightedge.next_fragment();
                    x_start = leftedge.x();
                    x_current = x_start;
                    x_stop = rightedge.x();
                    y_current = leftedge.y();
                } else {
                    this->valid = false;
                }
            }
            SearchForNonEmptyScanline();
            if( Debug ) {
                this->choose_color(x_current);
            }
        }



    private:

/*******************************************************************\
*                                                                   *
*             i n i t i a l i z e _ t r i a n g l e ( )             *
*                                                                   *
*         Initialize the current triangle for rasterization         *
*                                                                   *
\*******************************************************************/

	void initialize_triangle()
	{
	    // Do your magic stuff here.
            while (x_current >= x_stop && leftedge.more_fragments()) {
                next_fragment();
            }


	    if (this->Debug) {
	    	this->choose_color(this->x_current); // this->org_color[0];  // this->cgreen;
	    }
	    else {
		// "Get the right color"
		this->color_current = this->org_color[0];
	    }
	    if (this->x_start < this->x_stop - 1 && this->valid != false)
	        this->valid = true;
	    else {
		// Should only be called if the scanline is empty
		this->valid = this->valid && this->SearchForNonEmptyScanline();
            }
	}

/*******************************************************************\
*                                                                   *
*                      d e g e n e r a t e ( )                      *
*                                                                   *
*     A triangle is degenerate if all three points are co-linear    *
*                                                                   *
\*******************************************************************/

	bool degenerate()
	{

	    vector3_type e1, e2;
            e1 = ll - ul;
            e2 = other - ll;
            int x1, y1, x2, y2;
            x1 = e1[1];
            y1 = e1[2];
            x2 = e2[1];
            y2 = e2[2];
            
	    return (((x1 * y2) - (x2 * y1)) == 0);
	}


/*******************************************************************\
*                                                                   *
*                       L o w e r L e f t ( )                       *
*                                                                   *
\*******************************************************************/

	// LowerLeft() returns the index of the vertex with the smallest y-coordinate
	// If there is a horizontal edge, the vertex with the smallest 
	// x-coordinate is chosen.
	// The computations should be done in integer coordinates.
	int LowerLeft()
        {
            int ll = 0;
            for (int i = ll + 1; i < 3; ++i) {
                if ((this->ivertex[i][2] < this->ivertex[ll][2])
                    ||
                    ((this->ivertex[i][2] == this->ivertex[ll][2])
                     &&
                     (this->ivertex[i][1] < this->ivertex[ll][1])
                     )
                    )
                    { ll = i; }
            }
            return ll;
        }


/*******************************************************************\
*                                                                   *
*                       U p p e r L e f t ( )                       *
*                                                                   *
\*******************************************************************/

	// UpperLeft() returns the index of the vertex with the greatest y-coordinate
	// If there is a horizontal edge, the vertex with the smallest 
	// x-coordinate is chosen.
	// The computations should be done in integer coordinates.
	int UpperLeft()
        {
            int ul = 0;
            for (int i = ul + 1; i < 3; ++i) {
                if ((this->ivertex[i][2] > this->ivertex[ul][2])
                    ||
                    ((this->ivertex[i][2] == this->ivertex[ul][2])
                     &&
                     (this->ivertex[i][1] < this->ivertex[ul][1])
                     )
                    )
                    { ul = i; }
            }
            return ul;
        }



/*******************************************************************\
*                                                                   *
*       S e a r c h F o r N o n E m p t y S c a n l i n e ( )       *
*                                                                   *
\*******************************************************************/

	bool SearchForNonEmptyScanline()
	{
	    // Assumes that the current scanline is empty!
	    //this->valid = false;
	    // Do your matic stuff here.
            
            if (x_current >= x_stop - 1) {
                this->valid = false;
            }
            //if (leftedge.more_fragments()) {
            //    this->valid = true;
            //}
            while (x_current >= x_stop && leftedge.more_fragments()) {
                next_fragment();
                if (!leftedge.more_fragments()) {
                    this->degen = true;
                }
            }
            
	    return this->valid;
	}


/*******************************************************************\
*                                                                   *
*               c h o o s e _ c o l o r ( i n t   x )               *
*                                                                   *
\*******************************************************************/

	void choose_color(int x)
	{
	    // x is the position on a scanline in a triangle - they all have different colors:
	    //    xstart  : green
	    //    xcurrent: yellow
	    //    xstop   : red
	    // This is like a trafic-light: green->go ahead, yellow->be carefull, red->stop!

	    this->color_current = this->cyellow;
            if (x == this->x_stop - 1) {
	    	this->color_current = this->cred;
	    }
	    if (x == this->x_start) {
	    	this->color_current = this->cgreen;
	    }
	}




/*******************************************************************\
*                                                                   *
*                 P r i v a t e   V a r i a b l e s                 *
*                                                                   *
\*******************************************************************/

	// The Debug variable
	bool Debug;

	// The original 3D vertices
	vector3_type org_vertex[3];

	// The original 3D normals
	vector3_type org_normal[3];

	// The original 3D world coordinates
	vector3_type org_worldpoint[3];

	// The original vertex colors
	vector3_type org_color[3];

        // inserted from slide, used to calculate ll, ul etc.
        vector3_type ivertex[3];


	// Indices into the vertex table
	int lower_left;
	int upper_left;
	int the_other;

        // Screen coordinates
	int       x_start;
	int       y_start;
	real_type z_start;

	int       x_stop;
	int       y_stop;
	real_type z_stop;

	int       x_current;
	int       y_current;
	real_type z_current;


	vector3_type Nleft;
	vector3_type Nright;
	vector3_type Ncurrent;

	vector3_type Worldleft;
	vector3_type Worldright;
	vector3_type Worldcurrent;
	
	vector3_type color_left;
	vector3_type color_right;
	vector3_type color_current;

	vector3_type cred;
	vector3_type cgreen;
	vector3_type cyellow;

        //added for my own amusement
        vector3_type ll;
        vector3_type ul;
        vector3_type other;

	MyEdgeRasterizer<math_types> leftedge;
	MyEdgeRasterizer<math_types> rightedge;

	// The LinearInterpolator is hard-coded into this rasterizer -- must be changed!
	LinearInterpolator<math_types, typename math_types::real_type> depth_interpolator;

	// The LinearInterpolator is hard-coded into this rasterizer -- must be changed!
	LinearInterpolator<math_types, typename math_types::vector3_type> normal_interpolator;

	// The LinearInterpolator is hard-coded into this rasterizer -- must be changed!
	LinearInterpolator<math_types, typename math_types::vector3_type> worldpoint_interpolator;

	// The LinearInterpolator is hard-coded into this rasterizer -- must be changed!
	LinearInterpolator<math_types, typename math_types::vector3_type> color_interpolator;

	bool valid;
        bool degen;
    };

}// end namespace graphics

// TRIANGLE_RASTERIZER_H
#endif
