#pragma once

#include "core/engine.h"

#include <random>

class Color
{
public:
	unsigned int dword;

public:
	constexpr Color() : dword() {}
	constexpr Color(const Color& col) : dword(col.dword) {}
	constexpr Color& operator=(const Color& col)
	{
		if (&col != this)
			this->dword = col.dword;

		return *this;
	}
	constexpr Color(unsigned int dw) : dword(dw) {}
	constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		: dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{
	}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		: dword((r << 16u) | (g << 8u) | b)
	{
	}
	constexpr Color(Color col, unsigned char x) : Color((x << 24u) | col.dword) {}
	constexpr unsigned char GetX() const { return dword >> 24u; }
	constexpr unsigned char GetA() const { return GetX(); }
	constexpr unsigned char GetR() const { return (dword >> 16u) & 0xFFu; }
	constexpr unsigned char GetG() const { return (dword >> 8u) & 0xFFu; }
	constexpr unsigned char GetB() const { return dword & 0xFFu; }
	void SetX(unsigned char x) { dword = (dword & 0xFFFFFFu) | (x << 24u); }
	void SetA(unsigned char a) { SetX(a); }
	void SetR(unsigned char r) { dword = (dword & 0xFF00FFFFu) | (r << 16u); }
	void SetG(unsigned char g) { dword = (dword & 0xFFFF00FFu) | (g << 8u); }
	void SetB(unsigned char b) { dword = (dword & 0xFFFFFF00u) | b; }

	glm::vec3 operator()() const
	{
		return glm::vec3{
				   static_cast<float>(GetR()),
				   static_cast<float>(GetG()),
				   static_cast<float>(GetB())}
		/ 255.f;
	}
};

namespace Colors
{
static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 16) | (g << 8) | b;
}
inline static Color MakeRandomRGB()
{
	std::mt19937 randEngine(std::random_device{}());
	std::uniform_int_distribution<int> uDistrib(0, 255);

	return MakeRGB(
		static_cast<unsigned char>(uDistrib(randEngine)),
		static_cast<unsigned char>(uDistrib(randEngine)),
		static_cast<unsigned char>(uDistrib(randEngine)));
}

// Cateva culori default
static constexpr Color White = MakeRGB(255u, 255u, 255u);
static constexpr Color Black = MakeRGB(0u, 0u, 0u);
static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
static constexpr Color Red = MakeRGB(255u, 0u, 0u);
static constexpr Color Green = MakeRGB(0u, 255u, 0u);
static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
static constexpr Color Asfalt = MakeRGB(42u, 41u, 34u);
static constexpr Color DarkGreen = MakeRGB(0x20u, 0x59u, 0x21u);
static constexpr Color Brown = MakeRGB(0x55u, 0x34u, 0x2bu);
static constexpr Color TreeGreen = MakeRGB(0x6au, 0x6cu, 0x00u);
static constexpr Color DarkGray = MakeRGB(0x3au, 0x3bu, 0x3cu);
}