#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#define STBI_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>
#include <string>

class rtw_image {
  public:
    rtw_image();
    rtw_image(const char* image_filename);
    ~rtw_image();

    bool load(const std::string& filename);

    int width()  const;
    int height() const;

    const unsigned char* pixel_data(int x, int y) const;

  private:
    const int      bytes_per_pixel = 3;
    float         *fdata = nullptr;
    unsigned char *bdata = nullptr;
    int            image_width = 0;
    int            image_height = 0;
    int            bytes_per_scanline = 0;

    static int clamp(int x, int low, int high);
    static unsigned char float_to_byte(float value);
    void convert_to_bytes();
};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
