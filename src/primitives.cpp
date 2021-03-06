/*
 * primitives.cpp
 *
 *  Created on: Jul 20, 2011
 *      Author: claudio
 *
 *  Functions for drawing basic primitives
 */

#include "cprocessing.hpp"

using namespace cprocessing;


//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
//
static double boxVtx [] =  ///< Vertex coordinates of faces of box with size 2 centered at the origin. 
					 {1,1,1,  -1,1,1,  -1,-1,1,  1,-1,1,        // v0-v1-v2-v3
                      1,1,1,  1,-1,1,  1,-1,-1,  1,1,-1,        // v0-v3-v4-v5
                      1,1,1,  1,1,-1,  -1,1,-1,  -1,1,1,        // v0-v5-v6-v1
                      -1,1,1,  -1,1,-1,  -1,-1,-1,  -1,-1,1,    // v1-v6-v7-v2
                      -1,-1,-1,  1,-1,-1,  1,-1,1,  -1,-1,1,    // v7-v4-v3-v2
                      1,-1,-1,  -1,-1,-1,  -1,1,-1,  1,1,-1};   // v4-v7-v6-v5

static double boxNrm [] =  ///< Normal coordinates of faces of box 
					{0,0,1,  0,0,1,  0,0,1,  0,0,1,             // v0-v1-v2-v3
                     1,0,0,  1,0,0,  1,0,0, 1,0,0,              // v0-v3-v4-v5
                     0,1,0,  0,1,0,  0,1,0, 0,1,0,              // v0-v5-v6-v1
                     -1,0,0,  -1,0,0, -1,0,0,  -1,0,0,          // v1-v6-v7-v2
                     0,-1,0,  0,-1,0,  0,-1,0,  0,-1,0,         // v7-v4-v3-v2
                     0,0,-1,  0,0,-1,  0,0,-1,  0,0,-1};        // v4-v7-v6-v5   

namespace cprocessing {

	/// Draws a line segment in 3D
	/// @param x0: first vertex x coordinate
	/// @param y0: first vertex y coordinate
	/// @param z0: first vertex z coordinate
	/// @param x1: second vertex x coordinate
	/// @param y1: second vertex y coordinate
	/// @param z1: second vertex z coordinate
	void line (double x0, double y0, double z0, double x1, double y1, double z1) {
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glBegin(GL_LINES);
			glVertex3d (x0, y0, z0);
			glVertex3d (x1, y1, z1);
			glEnd();
		}
	}


	/// Draws a filled triangle given the coordinates
	/// of its vertices.
	void triangle (double x0, double y0, double z0,
				   double x1, double y1, double z1,
				   double x2, double y2, double z2) {
		GLdouble vertices[] = {
				x0, y0, z0,
				x1, y1, z1,
				x2, y2, z2
		};
		// activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vertices);

		if (styles[styles.size()-1].fillColor.rgba[3] > 0) {
			// See if filled triangle is required
			glColor4ubv (styles[styles.size()-1].fillColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES,0,3);
		}
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// See if outline triangle is required
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_TRIANGLES,0,3);
		}
		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

	}

    /// Draws a filled 2D quadrilateral given the coordinates
	/// of its vertices.
    void quad (double x0, double y0,
		 	   double x1, double y1,
			   double x2, double y2,
			   double x3, double y3)
    {
    	GLdouble vertices[] = {
    					x0, y0,
    					x1, y1,
    					x2, y2,
    					x3, y3
    			};
		// activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_DOUBLE, 0, vertices);

		if (styles[styles.size()-1].fillColor.rgba[3] > 0) {
			// See if filled triangle is required
			glColor4ubv (styles[styles.size()-1].fillColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_QUADS,0,4);
		}
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// See if outline triangle is required
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_QUADS,0,4);
		}
		// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);

    }

	/// Draws a point.
	void point (double x, double y, double z)
	{
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// Draw point using the stroke color
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glBegin (GL_POINTS);
			glVertex3d (x,y,z);
			glEnd();
		}
	}
	
	/// Configures the way the 'rect' function interprets its arguments
	/// @arg mode: either CENTER, RADIUS, CORNER or CORNERS
	void rectMode (unsigned mode) {
		assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
		::styles[styles.size()-1].rectMode = mode;
	}

	/// Draws an rectangle. The meaning of the arguments depend on the current
	/// rectMode. By default:
	/// @arg x, y: upper left corner of the rectangle
	/// @arg a, b: width and height of the rectangle
	void rect (double x, double y, double a, double b) 
	{
		// Make changes to arguments to reflect the current rectMode
		switch (::styles[styles.size()-1].rectMode) {
		case CORNER:
			quad (x, y, x+a, y, x+a, y+b, x, y+b);
			break;
		case CENTER:
			quad (x-a/2, y-b/2, x+a/2, y-b/2, x+a/2, y+b/2, x-a/2, y+b/2);
        		break;
        	case RADIUS:
			quad (x-a, y-b, x+a, y-b, x+a, y+b, x-a, y+b);
		   	break;
		case CORNERS:
			quad (x, y, a, y, a, b, x, b);
		   	break;
		}
	}

	/// Configures the number of line segments used for drawing an ellipse
	/// @arg n: number of sides
	void ellipseDetail (unsigned n) {
		styles[styles.size()-1].ellipseVtx.clear();

		for (unsigned i = 0; i < n; i++) {
			double ang = M_PI * 2 * i / n;
			double x = cos(ang)/2;
			double y = sin(ang)/2;
			styles[styles.size()-1].ellipseVtx.push_back (PVector (x, y, 0)); 
		}
	}
	
	/// Configures the way the 'ellipse' function interprets its arguments
	/// @arg mode: either CENTER, RADIUS, CORNER or CORNERS
	void ellipseMode (unsigned mode) {
		assert (mode == CENTER || mode == RADIUS || mode == CORNER || mode == CORNERS);
		::styles[styles.size()-1].ellipseMode = mode;
	}
	
	
	/// Draws an ellipse. The meaning of the arguments depend on the current
	/// ellipseMode. By default:
	/// @arg x, y: center of the ellipse
	/// @arg width, height: size of the ellipse axes
	void ellipse (double x, double y, double width, double height) 
	{
		// Make changes to arguments to reflect the current ellipseMode
		switch (::styles[styles.size()-1].ellipseMode) {
		case CENTER:
			x -= width/2;
        	y -= height/2;
        	break;
        case RADIUS:
		    x -= width;
		    y -= height;
		    width *= 2;
		    height *= 2;
		    break;
		case CORNERS:
		    width = width-x;
		    height = height-y;
		    break;
		}
		
		// Draw ellipse 
		glMatrixMode(GL_MODELVIEW);
	    glPushMatrix();
    	glTranslatef(x,y,0);
	    glScalef(width,height,1);
	    glTranslatef(0.5,0.5,0);
	    // activate and specify pointer to vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, styles[styles.size()-1].ellipseVtx [0].array());
    	if (styles[styles.size()-1].fillColor.rgba[3] > 0) {
			// See if filled ellipse is required
			glColor4ubv (styles[styles.size()-1].fillColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_POLYGON,0, styles[styles.size()-1].ellipseVtx.size());
    	}
    	if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// See if filled ellipse is required
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glDrawArrays(GL_LINE_LOOP,0, styles[styles.size()-1].ellipseVtx.size());
    	}
    	// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
		// Restore modelview
    	glPopMatrix();
    }

    void imageMode(unsigned mode) {
      assert(mode == CORNER || mode == CORNERS || mode == CENTER);
      ::styles[styles.size()-1].imageMode = mode;
    }

    void image(PImage& img, int x, int y) {
        image(img, x, y, img.width, img.height);
    }

    void image(PImage& img, int x, int y, int w, int h) {
    	switch (::styles[styles.size()-1].imageMode) {
			case CORNER:
				img.put(x, y, w, h);
				break;
			case CENTER:
				img.put(x-w/2, y-h/2, w, h);
	        	break;
			case CORNERS:
				img.put(x, y, x-w, y-h);
			   	break;
		}
    }

    void image(PImage * img, int x, int y) {
    	image(*img, x, y);
    }

    void image(PImage * img, int x, int y, int w, int h) {
    	image(*img, x, y, w, h);
    }


	/// Controls the detail used to render a sphere by adjusting the number 
	/// of vertices of the sphere mesh. The default resolution is 30, which creates 
	/// a fairly detailed sphere definition with vertices every 360/30 = 12 degrees.
	///
	/// @arg ures: number of segments used longitudinally per full circle revolution
	/// @arg vres: number of segments used latitudinally from top to bottom
	void sphereDetail (int ur, int vr) {
		styles[styles.size()-1].ures = ur;
		styles[styles.size()-1].vres = vr;
		
		styles[styles.size()-1].sphereVtx.clear();
		for (int itheta = 0; itheta<vr; itheta++) {
			double theta = TWO_PI / (vr-1) * itheta;
			double sintheta = sin(theta);
			double costheta = cos(theta);
			for (int iphi = 0; iphi<ur; iphi++) {
				double phi = PI / (ur-1) * iphi;
				double sinphi = sin(phi);
				double cosphi = cos(phi);
				double y = cosphi;
				double x = sinphi*costheta;
				double z = sinphi*sintheta;
				styles[styles.size()-1].sphereVtx.push_back(PVector(x,y,z));
			}
		}
		styles[styles.size()-1].sphereIdx.clear();
		for (int icol = 0; icol<vr-1; icol++) {
			for (int irow = 0; irow<ur; irow++) {
				styles[styles.size()-1].sphereIdx.push_back(icol*ur+irow);
				styles[styles.size()-1].sphereIdx.push_back((icol+1)*ur+irow);
			}
		}
	}
	

	/// Draws a sphere centered at the origin with the given radius.
	/// @arg radius: radius of the sphere
	void sphere(double radius) {
		glPushMatrix();
		glScaled(radius,radius,radius);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, styles[styles.size()-1].sphereVtx [0].array());
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, styles[styles.size()-1].sphereVtx [0].array());
		if (styles[styles.size()-1].fillColor.rgba[3] > 0) {
			// See if filled sphere is required		
			glColor4ubv (styles[styles.size()-1].fillColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glDrawElements( GL_QUAD_STRIP, 
							styles[styles.size()-1].ures*(styles[styles.size()-1].vres-1)*2, 
							GL_UNSIGNED_INT, 
							&(styles[styles.size()-1].sphereIdx[0]));
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// See if outline  is required
			glPushAttrib (GL_ENABLE_BIT);
			glDisable(GL_LIGHTING); // lines are not illuminated in processing
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glDrawElements(	GL_QUAD_STRIP, 
							styles[styles.size()-1].ures*(styles[styles.size()-1].vres-1)*2, 
							GL_UNSIGNED_INT, 
							&(styles[styles.size()-1].sphereIdx[0]));
			glPopAttrib();
		}
    	// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		// Restore modelview
    	glPopMatrix();
	}


	/// Draws a parallelepiped centered at the origin
	/// @arg width: x size
	/// @arg height: y size
	/// @arg depth: z size
	void box(double width, double height, double depth) {
		glPushMatrix();
		glScaled(width/2,height/2,depth/2);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, boxVtx);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, boxNrm);
		if (styles[styles.size()-1].fillColor.rgba[3] > 0) {
			// See if filled box is required		
			glColor4ubv (styles[styles.size()-1].fillColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glDrawArrays(GL_QUADS, 0, 24);
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
		if (styles[styles.size()-1].strokeColor.rgba[3] > 0) {
			// See if outline  is required
			glPushAttrib (GL_ENABLE_BIT);
			glDisable(GL_LIGHTING); // lines are not illuminated in processing
			glColor4ubv (styles[styles.size()-1].strokeColor.rgba);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glDrawArrays(GL_QUADS, 0, 24);
			glPopAttrib();
		}
    	// deactivate vertex arrays after drawing
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		// Restore modelview
    	glPopMatrix();		
	}
} // namespace cprocessing