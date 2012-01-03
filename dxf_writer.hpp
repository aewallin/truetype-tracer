#pragma once

#include "writer.hpp"

class DXF_Writer : public Writer {
public:
    DXF_Writer(double scale, bool blockdelete = false ) : scale(scale), bd(blockdelete) {}
    
    virtual void preamble() {
        printf("  0\nSECTION\n  2\nENTITIES\n");
    }
    virtual void postamble(long int offset, extents line_extents) {
        printf("  0\nSEQEND\n");
        printf("  0\nENDSEC\n  0\nEOF\n");
    }
    virtual void move_to(P p) {
        /* every move but the first one means we are starting a new polyline */
        /* make sure we terminate previous polyline with a seqend */
        //if(bootstrap == 0) printf("  0\nSEQEND\n");
        //bootstrap=0;
        //printf("  0\nPOLYLINE\n  8\n0\n 66\n     1\n 10\n0.0\n 20\n0.0\n 30\n0.0\n");
        //printf("  0\nVERTEX\n  8\n0\n 10\n%ld.000\n 20\n%ld.000\n 30\n0.0\n", to->x, to->y);
    }
    virtual void line(P p) {
        //printf("  0\nVERTEX\n  8\n0\n 10\n%.4f\n 20\n%.4f\n 30\n0.0\n", p.x, p.y);
    }
    virtual void line_to(P p) {
        printf("  0\nVERTEX\n  8\n0\n 10\n%ld.000\n 20\n%ld.000\n 30\n0.0\n", to->x,to->y);
    }
    virtual void conic_to(P to, P diff ) {
        //
    }
    virtual void cubic_to(P ctrl1, P ctrl2, P to ) {
        //
    }
    virtual void arc() {
        //     printf("  42\n%.4f\n  70\n1\n"
        //  "  0\nVERTEX\n  8\n0\n  10\n%.4f\n  20\n%.4f\n  30\n0.0\n",
        //  bulge, p2.x, p2.y);
    }
private:
    double scale;
    bool bd; // blockdelete
};
