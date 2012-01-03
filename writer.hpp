
#pragma once

class Writer {
public:
    virtual void preamble() = 0;
    virtual void postamble(long int offset, extents line_extents) = 0;
    virtual void move_to(P p) = 0;
    virtual void line(P p) = 0;
    virtual void line_to(P p) = 0;
    virtual void cubic_to(P c1, P c2, P to) = 0;
    virtual void conic_to(P to, P diff) = 0;
};
