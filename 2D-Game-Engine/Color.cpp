#include "Color.h"

const Color Color::BLACK = createColorHexRGB(0x000000);
const Color Color::WHITE = createColorHexRGB(0xFFFFFF);
const Color Color::GREY = createColorHexRGB(0x808080);
const Color Color::RED = createColorHexRGB(0xFF0000);
const Color Color::GREEN = createColorHexRGB(0x00FF00);
const Color Color::BLUE = createColorHexRGB(0x0000FF);
const Color Color::CYAN = createColorHexRGB(0x00FFFF);
const Color Color::MAGENTA = createColorHexRGB(0xFF00FF);
const Color Color::YELLOW = createColorHexRGB(0xFFFF00);

Color::Color() : r(0), g(0), b(0), a(1) {}

Color::Color(float32 r, float32 g, float32 b, float32 a) : r(r), g(g), b(b), a(a) {}

Color::Color(uint8 r, uint8 g, uint8 b, uint8 a) : Color( r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f) {}

Color::~Color() {}

Color Color::createColorHexRGB(uint32 rgb) {
	return { (uint8)(rgb >> 16), (rgb & 0x00FF00) >> 8, (rgb & 0x0000FF)};
}
