

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

struct extents {
    long int minx;
    long int maxx;
    long int miny;
    long int maxy;
    void reset() {
        maxx = -2000000000;
        maxy = -2000000000;
        minx =  2000000000;
        miny =  2000000000;
    }
    void add_point( const FT_Vector& point ) {
        if ( point.x > maxx ) maxx = point.x;
        if ( point.y > maxy ) maxy = point.y;
        if ( point.x < minx ) minx = point.x;
        if ( point.y < miny ) miny = point.y;
    }
    // updates extents struct e1 to include all of e2
    void add_extents(  extents& e2 ) {
        if ( e2.maxx > maxx ) maxx = e2.maxx;
        if ( e2.maxy > maxy ) maxy = e2.maxy;
        if ( e2.minx < minx ) minx = e2.minx;
        if ( e2.miny < miny ) miny = e2.miny;
    }
}; 
