#pragma once

struct color_t {
    color_t ( int r, int g, int b, int a ) : r ( r ), g ( g ), b ( b ), a ( a ) { }
    color_t ( int r, int g, int b ) : r ( r ), g ( g ), b ( b ), a ( 255 ) { }

    unsigned char r, g, b, a;
};