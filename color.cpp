#include "cprocessing.hpp"
#include "color.hpp"


using namespace cprocessing;

inline unsigned char clamp(double v) {
    return v > 255 ? 255 : v < 0 ? 0 : (unsigned char) v;
}

// Simple hsv to rgb conversion. Assumes components specified in range 0.0-1.0.
static void hsb_to_rgb (double h, double s, double v,
                        double& r, double& g, double& b)
{
    double tmp = h*5.99999999;
    int hi = int (tmp);
    double f = tmp-hi;
    double p = v * (1-s);
    double q = v * (1-f*s);
    double t = v * (1-(1-f)*s);
    if (hi==0) {
        r = v; g = t; b = p;
    } else if (hi==1) {
        r = q; g = v; b = p;
    } else if (hi==2) {
        r = p; g = v; b = t;
    } else if (hi == 3) {
        r = p; g = q; b = v;
    } else if (hi == 4) {
        r = t; g = p; b = v;
    } else {
        r = v; g = p; b = q;
    }
}
    
    
// Simple rgb to hsb conversion. Assumes components specified in range 0.0-1.0.
static void rgb_to_hsb (double r, double g, double b,
                        double& h, double& s, double& v)
{
    double maxval = max(max(r,g),b);
    double minval = min(min(r,g),b);
    if (maxval==minval) {
        h = 0.0;
    } else if (maxval==r) {
        h = fmod (60 * (g-b)/(maxval-minval) + 360, 360) / 360.0;
    }
    else if (maxval==g) {
        h = (60 * (b-r)/(maxval-minval) + 120) / 360.0;
    } else {
        h = (60 * (r-g)/(maxval-minval) + 240) / 360.0;
    }

    if (maxval==0.0) {
        s = 0.0;
    } else {
        s = (maxval-minval)/maxval;
    }
    v = maxval;
}

/// Tells which color space is actually used for color specification
static unsigned globColorMode = RGB;


/// Maximum range values for color components. Specified by the colorMode() function.
static double max1 = 255;  // First component
static double max2 = 255;  // Second component
static double max3 = 255;  // Third component
static double maxA = 255;  // Alpha component

namespace cprocessing {
    //
    // Attributes
    //
    /// Constructor for the color class
    color::color(double val1, double val2, double val3, double valA) {
		//SCale the values to a range of 255
		val1 = val1/max1; 
		val2 = val2/max2;
		val3 = val3/max3;
		if (valA == MAXCOLOR) valA = 1.0;
		else valA = valA/maxA;
		
		if (globColorMode != RGB) {
			hsb_to_rgb(val1, val2, val3, val1, val2, val3);
		}
		
		rgba[0] = clamp(val1*255);
		rgba[1] = clamp(val2*255);
		rgba[2] = clamp(val3*255);
		rgba[3] = clamp(valA*255);
    };

    // Constructor from a gray value
    color::color(double gray, double alpha){
	    if (alpha == MAXCOLOR) { alpha = maxA;}
	    unsigned char val = clamp(gray/max1*255);
	    rgba[0] = val;
	    rgba[1] = val;
	    rgba[2] = val;
	    rgba[3] = clamp(alpha/maxA*255);
    }
    
    //TODO::make this disseminate #aaaaaa or #aaa words or else throw error
    color::color(std::string) {
        rgba[0] = 0;
        rgba[1] = 0;
        rgba[2] = 0;
        rgba[3] = maxA;
    }

    /// Converts color to a float array
    void color::toFloat(float a[]) {
        a [0] = rgba[0]*1.0/255;
        a [1] = rgba[1]*1.0/255;
        a [2] = rgba[2]*1.0/255;
        a [3] = rgba[3]*1.0/255;
    }

    /// Converts color to a double array
    void color::toDouble(double a[]) {
        a [0] = rgba[0]*1.0/255;
        a [1] = rgba[1]*1.0/255;
        a [2] = rgba[2]*1.0/255;
        a [3] = rgba[3]*1.0/255;
    }


    /// Changes the way Processing interprets color data.
    /// The colorMode() function is used to change the numerical range used for specifying colors and to switch color systems.
    void colorMode(unsigned mode, double range1, double range2, double range3, double range4){
        assert(mode == RGB || mode == HSB);
        globColorMode = mode;

        max1 = range1;
        max2 = range2;
        max3 = range3;
        if (range4 != MAXCOLOR) maxA = range4;
    }
    
    // When changing just the color system, ranges are kept as is
    void colorMode(unsigned mode) {
        assert(mode == RGB || mode == HSB);
        globColorMode = mode;
     }
	
	/// Extracts the alpha value from a color, scaled to match current colorMode()
    double alpha(const color & color){
        return double(color.rgba[3])/255*maxA;
    }
	
	/// Extracts the red value from a color, scaled to match current colorMode()
    double red(const color & color){
        return double(color.rgba[0])/255*max1;
    }

	/// Extracts the green value from a color, scaled to match current colorMode()
    double green(const color & color){
        return double(color.rgba[1])/255*max2;
    }

	/// Extracts the blue value from a color, scaled to match current colorMode()
    double blue(const color & color){
        return double(color.rgba[2])/255*max3;
    }

	/// Extracts the hue value from a color, scaled to match current colorMode()
    double hue(const color & color){
        double h, s, v;
        rgb_to_hsb(color.rgba[0]/255.0,color.rgba[1]/255.0,color.rgba[2]/255.0,h,s,v);
        return h*max1;
    }
	
	/// Extracts the saturation value from a color, scaled to match current colorMode()
    double saturation(const color & color){
        double h, s, v;
        rgb_to_hsb(color.rgba[0]/255.0,color.rgba[1]/255.0,color.rgba[2]/255.0,h,s,v);
        return s*max2;
    }
	
	/// Extracts the brightness value from a color, scaled to match current colorMode()
    double brightness(const color & color){
        double h, s, v;
        rgb_to_hsb(color.rgba[0]/255.0,color.rgba[1]/255.0,color.rgba[2]/255.0,h,s,v);
        return v*max3;
    }
}