#pragma once

#include "components/simple_scene.h"
#include "core/engine.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace utils
{

template <typename T>
struct VLSpace
{
	VLSpace(T x, T y, T width, T height)
		: x(x), y(y), width(width), height(height), rightX(x + width), upperY(y + height)
	{
	}
	VLSpace() : VLSpace(0, 0, 1, 1) {}

public:
	void SetX(T x)
	{
		this->x = x;
		UpdateParams();
	}
	void SetY(T y)
	{
		this->y = y;
		UpdateParams();
	}
	void SetWidth(T width)
	{
		this->width = width;
		UpdateParams();
	}
	void SetHeight(T height)
	{
		this->height = height;
		UpdateParams();
	}
	T GetRightX() const { return rightX; }
	T GetUpperY() const { return upperY; }
	T GetX() const { return x; }
	T GetY() const { return y; }
	T GetWidth() const { return width; }
	T GetHeight() const { return height; }

private:
	void UpdateParams()
	{
		rightX = x + width;
		upperY = y + height;
	}

private:
	T x;
	T y;
	T width;
	T height;
	T rightX;
	T upperY;
};
typedef VLSpace<float> LogicSpace;
typedef VLSpace<int> ViewportSpace;

// Functions to check if a points is inside a polygone or triangle
float inline constexpr sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
}
inline bool CheckIfPointIsInside(
	const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& point)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(point, v1, v2);
	d2 = sign(point, v2, v3);
	d3 = sign(point, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}
inline float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0f);
}
inline bool CheckIfPointIsInside(
	const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec2& pt, const float& tolerance = 1e-3f)
{
	float A = area(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);

	float A1 = area(pt.x, pt.y, v2.x,v2.y, v3.x, v3.y);
	float A2 = area(v1.x, v1.y, pt.x, pt.y, v3.x, v3.y);
	float A3 = area(v1.x, v1.y, v2.x, v2.y, pt.x, pt.y);

	return abs(A - (A1 + A2 + A3)) < tolerance;
}
inline bool CheckIfPointIsInside(
	const glm::vec3& v1,
	const glm::vec3& v2,
	const glm::vec3& v3,
	const glm::vec3& v4,
	const glm::vec3& point)
{
	return (
		glm::cross(point - v1, v2 - v1).y >= 0 && glm::cross(point - v2, v3 - v2).y >= 0
		&& glm::cross(point - v3, v4 - v3).y >= 0 && glm::cross(point - v4, v1 - v4).y >= 0);
}


// Functii ajutatoare pentru vectori
inline glm::vec3 GetInterpolatedPoint(
	const glm::vec3& v1, const glm::vec3& v2, const float interpolationPoint)
{
	return v1 + (v2 - v1) * interpolationPoint;
}
inline glm::vec3 ShiftPointInDirection(glm::vec3& point, const glm::vec3& direction, const float& distance)
{
	point = point + direction * distance;
}

// Transformare din sptiu din rhs in spatiul din lhs
inline glm::mat3 VisualizationTransf2d(const utils::LogicSpace& rhs, const utils::LogicSpace& lhs)
{
	float sx, sy, tx, ty;
	sx = lhs.GetWidth() / rhs.GetWidth();
	sy = lhs.GetHeight() / rhs.GetHeight();
	tx = lhs.GetX() - sx * rhs.GetX();
	ty = lhs.GetY() - sy * rhs.GetY();

	return glm::transpose(glm::mat3(sx, 0.0f, tx, 0.0f, sy, ty, 0.0f, 0.0f, 1.0f));
}

// Transformari 2d
inline glm::mat3 Translate2d(float translateX, float translateY)
{
	return glm::transpose(glm::mat3(1, 0, translateX, 0, 1, translateY, 0, 0, 1));
}
inline glm::mat3 Scale2d(float scaleX, float scaleY)
{
	return glm::transpose(glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1));
}
inline glm::mat3 Rotate2d(float radians)
{
	float sinp = glm::sinf(radians);
	float cosp = glm::cosf(radians);
	return glm::transpose(glm::mat3(cosp, -sinp, 0, sinp, cosp, 0, 0, 0, 1));
}
}  // namespace utils