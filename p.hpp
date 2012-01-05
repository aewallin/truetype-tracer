
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

struct P { 
    P() : x(0), y(0) {}
    P(double x,double y) : x(x), y(y) {}
    P(const FT_Vector *v): x(v->x) , y(v->y) { }
    double x;
    double y;
    P scale( double b) { 
        P r(x*b, y*b); 
        return r; 
    }
    double mag(P a) { return sqrt( this->dot( *this) ); }
    double dot(P b) { return x * b.x + y * b.y; }
    
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
    P operator+( P p) const {
         P r( x+p.x, y+p.y);
         return r;
    }
    P operator-( P p) const {
         P r( x-p.x, y-p.y);
         return r;
    }
    P operator*(double a) const {
        P r( x*a , y*a );
        return r;
    }
}; 

//static double max(double a, double b) { if(a < b) return b; else return a; }
//static P ft2p(const FT_Vector *v) { P r = {v->x, v->y}; return r; }
//static double dot(P a, P b) { return a.x * b.x + a.y * b.y; }
//static double mag(P a) { return sqrt(dot(a, a)); }
//static P scale(P a, double b) { P r = {a.x*b, a.y*b}; return r; }

/*
static P add(P a, P b) { P r = {a.x + b.x, a.y + b.y}; return r; }
static P add3(P a, P b, P c) { P r = {a.x + b.x + c.x, a.y + b.y + c.y}; return r; }
static P add4(P a, P b, P c, P d) { P r = {a.x + b.x + c.x + d.x, a.y + b.y + c.y + d.y}; return r; }
*/

//static P sub(P a, P b) { P r = {a.x - b.x, a.y - b.y}; return r; }

/*static P unit(P a) {
    double m = mag(a);
    if(m) {
        P r = {a.x/m, a.y/m };
        return r;
    } else {
        P r = {0, 0};
        return r;
    }
}
*/
