#include "rtw_stb_image.h"
#include "external/stb_image.h"

rtw_image::rtw_image() {}

rtw_image::rtw_image(const char* image_filename) {
    auto filename = std::string(image_filename);
    auto imagedir = getenv("RTW_IMAGES");

    if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
    if (load(filename)) return;
    if (load("images/" + filename)) return;
    if (load("../images/" + filename)) return;
    if (load("../../images/" + filename)) return;
    if (load("../../../images/" + filename)) return;
    if (load("../../../../images/" + filename)) return;
    if (load("../../../../../images/" + filename)) return;
    if (load("../../../../../../images/" + filename)) return;

    std::cerr << "ERROR: Could not load image file '" << image_filename << "'.\n";
}

rtw_image::~rtw_image() {
    delete[] bdata;
    stbi_image_free(fdata);
}

bool rtw_image::load(const std::string& filename) {
    auto n = bytes_per_pixel;
    fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
    if (!fdata) return false;

    bytes_per_scanline = image_width * bytes_per_pixel;
    convert_to_bytes();
    return true;
}

int rtw_image::width() const {
    return image_width;
}

int rtw_image::height() const {
    return image_height;
}

const unsigned char* rtw_image::pixel_data(int x, int y) const {
    if (bdata == nullptr) return nullptr;

    x = clamp(x, 0, image_width  - 1);
    y = clamp(y, 0, image_height - 1);

    return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int rtw_image::clamp(int x, int low, int high) {
    if (x < low)  return low;
    if (x > high) return high;
    return x;
}

unsigned char rtw_image::float_to_byte(float value) {
    if (value <= 0.0f) return 0;
    if (value >= 1.0f) return 255;
    return static_cast<unsigned char>(256 * value);
}

void rtw_image::convert_to_bytes() {
    if (fdata == nullptr) return;

    auto total_bytes = image_width * image_height * bytes_per_pixel;
    bdata = new unsigned char[total_bytes];

    auto src = fdata;
    auto dst = bdata;

    for (int i = 0; i < total_bytes; i++, src++, dst++) {
        *dst = float_to_byte(*src);
    }
}
