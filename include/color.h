/**
 *   \file color.h
 *   \brief Color related data structures.
 */


#ifndef COLOR_H_B3ZHITXW
#define COLOR_H_B3ZHITXW

/// Stores color represented as integers between 0-255 for each channel.
struct coloru {
    unsigned char r, g, b;
};

/// Stores color represented as floating point values between 0.0-1.0 for each channel
struct colorf {
    float r, g, b;
};

#endif /* end of include guard: COLOR_H_B3ZHITXW */
