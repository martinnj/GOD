#ifndef CAMERA_H
#define CAMERA_H
//
// Graphics Framework.
// Copyright (C) 2007 Department of Computer Science, University of Copenhagen
//
#include "graphics/graphics.h"
#include "solution/transformations.h"

#define PRINT_MATRICES 0


namespace graphics {
    
    /**
     * A virtual pin-hole camera.
     * The camera class produces the matrices necessary for the virtual world
     * to be rasterized.
     */
    template<typename math_types>
    class MyCamera : public Camera<math_types>
    {
    public:
        /// The actual type of the elements of vectors and matrices.
	typedef typename math_types::real_type      real_type;

	/// The actual type of a vector2.
	typedef typename math_types::vector2_type   vector2_type;

	/// The actual type of a vector3.
	typedef typename math_types::vector3_type   vector3_type;

	/// The actual type of a matrix4x4.
	typedef typename math_types::matrix4x4_type matrix4x4_type;
	
    public:

	/**
	 * Creates a Camera with a default (empty) state.
	 */
	MyCamera() : Camera<math_types>()
	{}
	


	/**
	 * Destroys the Camera.
	 */
	virtual ~MyCamera()
	{}

    protected:

	/**
	 * Computes a matrix which transforms coordinates from the World-coordinate system 
	 * to the Eye-coordinate system.
	 * Foley et al. (p.229-284; 2nd edition)
	 * @param vrp The View Reference Point.
	 * @param vpn The View Plane Normal.
	 * @param vup The View Up Vector.
	 * @return    The view_orientation matrix.
	 */
	matrix4x4_type compute_view_orientation_matrix(vector3_type const& vrp,
						       vector3_type const& vpn,
						       vector3_type const& vup)
	{
	    matrix4x4_type M(Identity());
	    
	    // Do your magic stuff here.

	    matrix4x4_type ViewOrient(M);
            

	    // Rotate so the Eye coordinate system coincides with the World coordinate system
            //Transelate VRP to origin
            matrix4x4_type T(M);
            T = Translate(-vrp);

            //Rotate so it fits the world system.
            vector3_type rz, rx, ry;
            int vpnlen = sqrt(vpn[1]*vpn[1]+vpn[2]*vpn[2]+vpn[3]*vpn[3]);
            rz = vpn/vpnlen;

            vector3_type vupc = Cross(vup, rz);
            int vupclen = sqrt(vupc[1]*vupc[1]+vupc[2]*vupc[2]+vupc[3]*vupc[3]);
            rx = vupc/vupclen;

            vector3_type rzx = Cross(rz,rx);
            int rzxlen = sqrt(rzx[1]*rzx[1]+rzx[2]*rzx[2]+rzx[3]*rzx[3]);
            ry = rzx/rzxlen;
            matrix4x4_type R(M);
            R[1][1] = rx[1];
            R[1][2] = rx[2];
            R[1][3] = rx[3];
            R[2][1] = ry[1];
            R[2][2] = ry[2];
            R[2][3] = ry[3];
            R[3][1] = rz[1];
            R[3][2] = rz[2];
            R[3][3] = rz[3];

            ViewOrient = R * T;
            
	    // save x_eye, y_eye, z_eye in graphics_state
            this->m_state->x_eye_axis() = rx;
            this->m_state->y_eye_axis() = ry;
            this->m_state->z_eye_axis() = rz;
	    
#if PRINT_MATRICES
	    //std::cout << "R    = " << R << std::endl;
	    //std::cout << "InvR = R.T() = " << InvR << std::endl;
#endif

	    // Save the ViewOrientation matrix in state().
            this->m_state->view_orientation() = ViewOrient;

#if PRINT_MATRICES
	    //std::cout << "ViewOrient = R * T(-vrp) = " << ViewOrient << std::endl;
#endif

	    // Also, save the inverse ViewOrientation matrix in state().
            matrix4x4_type invvieworient = Inverse(ViewOrient);
            this->m_state->inv_view_orientation() = invvieworient;

#if PRINT_MATRICES
	    //std::cout << "InvViewOrient = T(vrp) * R.T() = " << InvViewOrient << std::endl;
#endif

	    return ViewOrient;
	}

	/**
	 * Computes a matrix which transforms from the Eye-coordinate system to the Canonical View Volume.
	 * Foley et al. (p.229-284; 2nd edition)
	 * @param prp         The Projection Reference Point in Eye-coordinates.
	 * @param lower_left  The lower left corner of the window in Eye-coordinates.
	 * @param upper_right The upper right corner of the window in Eye-coordinates.
	 * @param front_plane The closest distance from the origin in Eye-coordinates.
	 * @param back_plane  The farthest distance from the origin in Eye-coordinates.
	 * @return The view_projection matrix.
	 */
	matrix4x4_type compute_view_projection_matrix(vector3_type const& prp,
						      vector2_type const& lower_left,
						      vector2_type const& upper_right,
						      real_type    const& front_plane,
						      real_type    const& back_plane)
        {
	    matrix4x4_type M(Identity());

	    // Do your magic stuff here.

	    matrix4x4_type ViewProject(M);

#if PRINT_MATRICES
	    //std::cout << "T(-prp) = T(" << -prp << ") = " << T << std::endl;
#endif
            matrix4x4_type T = Translate(-prp);
	    // Shear_xy so the view volume is symmetric around the z-axis
            real_type height = upper_right[2] - lower_left[2];
            //printf("H = %f\n", height);
            real_type width = upper_right[1] - lower_left[1];
            //printf("W = %f\n",width);
            vector3_type CW;
            CW[1] = lower_left[1] + (width/2);
            CW[2] = lower_left[2] + (height/2);
            CW[3] = 0;

            vector3_type dop = prp - CW;
            matrix4x4_type shper = XY_Shear(-(dop[1]/dop[3]),-(dop[2]/dop[3])); //step 4

#if PRINT_MATRICES
	    //std::cout << "lower_left  = " << lower_left << std::endl;
	    //std::cout << "upper_right = " << upper_right << std::endl;
	    //std::cout << "cw  = " << cw  << std::endl;
	    //std::cout << "prp = " << prp << std::endl;
	    //std::cout << "dop = prp - cw = " << dop << std::endl;
	    //std::cout << "Sh_xy(-dop_u/dop_n, -dop_v/dop_n) = Sh_xy("
	    //	      << - dop[1] / dop[3] << " " << - dop[2] / dop[3] << ") = " << Sh_xy << std::endl;
#endif


	    // Scale such that the planes make 45 degrees the z-axis
	    // First step: scale in x- and y-directions
                       
	  
#if PRINT_MATRICES  
	    //std::cout << "upper_right - lower_left = [" << window_dimensions << "]^T" << std::endl;
	    //std::cout << "S(s_x, s_y, 1) = S(" << s_x << " " << s_y << " 1) = " << S_xy << std::endl;
#endif

	    // Second step: scale uniformly so the view-volume gets the right dimensions, i.e.
	    // Scale uniformly so the back plane is mapped to -1.
            real_type sx = (-2*prp[3])/(width*(back_plane-prp[3]));
            real_type sy = (-2*prp[3])/(height*(back_plane-prp[3]));
            real_type sz = -1/(back_plane-prp[3]);
            matrix4x4_type sper = Scale(sx,sy,sz);

#if PRINT_MATRICES	    
	    //std::cout << "back_plane = " << back_plane << std::endl;
	    //std::cout << "prp = [" << prp << "]^T" << std::endl;
	    //std::cout << "s = " << s << std::endl;
	    //std::cout << "S_uniform(s) = S(" << s << ") = " << S_uniform << std::endl;
	    
#endif

	    // Now, transform the perspective view-volume to the parallel view-volume
	    real_type vrpz = -(prp[3]);
            real_type zmin = -1;
            real_type zmax = -((front_plane-prp[3])/(back_plane-prp[3]));
            real_type zp   = prp[3]/(back_plane-prp[3]);

            matrix4x4_type per2par = Perspective_to_Parallel(zmax);

            ViewProject = per2par * sper * shper * T;

#if PRINT_MATRICES
	    //std::cout << "Perp2Par = " << Perp2Par << std::endl;	    
#endif

#if PRINT_MATRICES
	    //std::cout << "ViewProject = " << ViewProject << std::endl;
#endif

            this->m_state->view_projection() = ViewProject;
            matrix4x4_type invproj = Inverse(ViewProject);
            this->m_state->inv_view_projection() = invproj;
	    return ViewProject;
	}


	/**
	 * Computes a matrix which transforms from the Canonical-Volume to the Screen coordinates.
	 * @param viewport_width  The width of the viewport on the screen.
	 * @param viewport_height The height of the viewport on the screen.
	 */
	virtual matrix4x4_type compute_window_viewport_matrix(real_type const& viewport_width,
							      real_type const& viewport_height,
	                                                      vector3_type const& translation
							                          = vector3_type(0.0, 0.0, 0.0))
	{
	    matrix4x4_type WindowViewport(Identity());

	    // Do your magic stuff here.

	    // The translation and its inverse
            WindowViewport = Scale(viewport_width/2, viewport_height/2, 1) * Translate(1,1,0);
#if PRINT_MATRICES
	    //std::cout << "T_wv(1.0, 1.0, 0.0) = " << T_wv << std::endl;
#endif

	    /// The scaling and its inverse

#if PRINT_MATRICES
	    //std::cout << "S_screen("
	    //	      << viewport_width  / 2.0 << " "
	    //	      << viewport_height / 2.0 << " "
	    //	      << 0.0
	    //	      << ") = " << S_screen << std::endl;
#endif

	    /// The translation on the screen and its inverse

	    /// The final window-viewport transformation

#if PRINT_MATRICES
	    //std::cout << "WindowViewport = " << WindowViewport << std::endl;
#endif
            matrix4x4_type invview = Inverse(WindowViewport);
            this->m_state->window_viewport() = WindowViewport;
            this->m_state->inv_window_viewport() = invview;

	    return WindowViewport;
	}


	/**
	 * Computes a projection matrix using the parameters for a perspective camera 
	 * given by Foley et al. (p.229-284; 2nd edition)
	 * The projection matrix should transforms from world coordinates to 
	 * normalized projection coordinates.
	 * This is done in a number of steps: compute_view_orientation, compute_view_projection, and compute_window_viewport.
	 * The final matrix is stored in the graphics_state.
	 *
	 * @param vrp	        View reference point
	 * @param vpn           View-plane normal 
	 * @param vup	        View up vector
	 * @param prp	        Projection reference point
	 * @param lower_left    Lower left corner of the view-plane
	 * @param upper_right   Upper right corner of the view-plane
	 * @param front_plane   Distance to front clipping plane
	 * @param back_plane    Distance to back clipping plane
	 * @param screen_width  The width in pixels of the screen
	 * @param screen_height The height in pixels of the screen
	 */
	matrix4x4_type compute_projection_matrix(vector3_type const& vrp, vector3_type const& vpn,
						 vector3_type const& vup, vector3_type const& prp,
						 vector2_type const& lower_left,
						 vector2_type const& upper_right,
						 real_type    const& front_plane,
						 real_type    const& back_plane,
						 real_type    const& screen_width,
						 real_type    const& screen_height)
	{
	    // ToDo add your magic here!

	    matrix4x4_type Id = Identity();

            
            Id = Id * compute_view_projection_matrix(prp, lower_left, upper_right,
                                                     front_plane, back_plane);
            Id = Id * compute_view_orientation_matrix(vrp,vpn,vup);
            Id = Id * compute_window_viewport_matrix(screen_width, screen_height);
            compute_window_viewport_matrix(screen_width, screen_height);

            matrix4x4_type invproj = Inverse(Id);
            this->m_state->projection() = Id;
            this->m_state->inv_projection() = invproj;

	    return Id;
	}
    };

}// end namespace graphics

// CAMERA_H
#endif
