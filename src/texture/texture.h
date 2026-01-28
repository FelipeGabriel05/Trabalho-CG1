#ifndef TEXTURE_H
#define TEXTURE_H

#include "../vectors/vec.h"
#include "../colors/color.h"
#include "rtw_stb_image.h"
#include <algorithm>
#include <memory>
#include <cmath>

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point4& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color(const color& c) : c(c) {}

        solid_color(double red, double green, double blue, double w) : solid_color(color(red, green, blue, w)) {}

        color value(double u, double v, const point4& p) const override {
            return c;
        }
    private:
    color c;
};

class checker_texture : public texture {
  public:
    checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color& c1, const color& c2)
      : checker_texture(scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point4& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    double inv_scale;
    std::shared_ptr<texture> even;
    std::shared_ptr<texture> odd;
};

class image_texture : public texture {
  public:
    image_texture(const char* filename) : image(filename) {}

    color value(double u, double v, const point4& p) const override {
        // Se não tiver dados de textura, retorna ciano sólido para debug.
        if (image.height() <= 0) return color(0,1,1,0);

        // Clamp manual das coordenadas de textura para [0,1] x [1,0]
        // Substitui o std::clamp que estava dando erro
        if (u < 0.0) u = 0.0;
        if (u > 1.0) u = 1.0;

        if (v < 0.0) v = 0.0;
        if (v > 1.0) v = 1.0;
        
        // Inverte V para coordenadas de imagem
        v = 1.0 - v;

        auto i = int(u * image.width());
        auto j = int(v * image.height());

        // Garante que i e j não estourem os limites da imagem
        if (i >= image.width())  i = image.width() - 1;
        if (j >= image.height()) j = image.height() - 1;

        auto pixel = image.pixel_data(i,j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2], 0);
    }

  private:
    rtw_image image;
};

#endif