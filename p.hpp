#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

// a 2D point in the xy-plane
struct P { 
    P() : x(0), y(0) {}
    P(double x,double y) : x(x), y(y) {}
    P(const FT_Vector *v): x(v->x) , y(v->y) { }
    double x;
    double y;
    // scalar multiplication
    // FIXME: replace all calls to this with operator* below. Then remove this.
    P scale( double b) { 
        P r(x*b, y*b); 
        return r; 
    }
    // magnitude, or norm
    double mag(P a) { return sqrt( this->dot( *this) ); }
    // dot product
    double dot(P b) { return x * b.x + y * b.y; }
    // return point normalised so that mag(p)==1
    P unit() {
        double m = mag(*this);
        if(m) {
            P r( x/m, y/m );
            return r;
        } else {
            P r(0, 0);
            return r;
        }
    }
    // vector addition
    P operator+( P p) const {
         P r( x+p.x, y+p.y);
         return r;
    }
    // vector subtraction
    P operator-( P p) const {
         P r( x-p.x, y-p.y);
         return r;
    }
    // scalar multiplication.
    P operator*(double a) const {
        P r( x*a , y*a );
        return r;
    }
}; 
