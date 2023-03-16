#pragma once

#define LIGHT
#include "Directional.hpp"
#include "Point.hpp"
#include "Spot.hpp"
#undef LIGHT

#include "amUtilities/Colors.hpp"

class LightSourceAdapter
{
public:
	enum LightType
	{
		NONE = 0,
		DIRECTIONAL = 1,
		SPOT = 2,
		POINT = 3
	};

public:
	virtual const Color& GetLightColor() const = 0;
	virtual const float& GetLightIntensity() const = 0;
	virtual const glm::vec3& GetPosition() const = 0;
	virtual int GetLightTypeInt() const = 0;
	virtual LightSourceAdapter::LightType GetLightType() const = 0;

	virtual void SetPosition(const glm::vec3&) = 0; 

public:
	static constexpr unsigned int materialShiness = 20;

	static constexpr float materialKd = 0.5f;
	static constexpr float materialKs = 0.2f;
};

template <class T>
class LightSource : public T, public LightSourceAdapter
{
public:
	LightSource() = delete;

	// Directional light constructor
	LightSource(
		const Color& color,
		const glm::vec3& direction,
		const float& intensity)
		: m_lightColor(color), m_lightIntensity(intensity), T(direction)
	{
		this->SetType(LightType::DIRECTIONAL);
	}

	// Point light constructor
	LightSource(const glm::vec3& position, const Color& color, const float& intensity)
		: m_position(position), m_lightColor(color), m_lightIntensity(intensity), T()
	{
		this->SetType(LightType::POINT);
	}

	// Spot light constructor
	LightSource(
		const glm::vec3& position,
		const Color& color,
		const float& intensity,
		const float& cutOff,
		const glm::vec3& direction,
		const glm::vec3& right)
		: m_position(position),
		  m_lightColor(color),
		  m_lightIntensity(intensity),
		  T(cutOff, direction, right)
	{
		this->SetType(LightType::SPOT);
	}

	const Color& GetLightColor() const override { return m_lightColor; }
	const float& GetLightIntensity() const override { return m_lightIntensity; }
	const glm::vec3& GetPosition() const override { return m_position; }
	int GetLightTypeInt() const override { return static_cast<int>(m_lightType); }
	LightSourceAdapter::LightType GetLightType() const override { return m_lightType; }

	void SetPosition(const glm::vec3& pos) override { m_position = pos; }

private:
	// Verifier for the type of light
	void SetType(LightType expectedType)
	{
		if (std::is_same<T, Directional>::value == true)
		{
			m_lightType = LightType::DIRECTIONAL;
			if (m_lightType != expectedType)
			{
				throw std::exception("Expected another type of light");
			}
			return;
		}

		if (std::is_same<T, Spot>::value == true)
		{
			m_lightType = LightType::SPOT;
			if (m_lightType != expectedType)
			{
				throw std::exception("Expected another type of light");
			}
			return;
		}

		if (std::is_same<T, Point>::value == true)
		{
			m_lightType = LightType::POINT;
			if (m_lightType != expectedType)
			{
				throw std::exception("Expected another type of light");
			}
			return;
		}

		throw std::exception("Dfk????");
	}

protected:
	glm::vec3 m_position = glm::vec3(0);

private:
	float m_lightIntensity = 0.f;
	Color m_lightColor;
	LightType m_lightType = LightType::NONE;
};

typedef LightSource<Directional> DirectionalLight;
typedef LightSource<Spot> SpotLight;
typedef LightSource<Point> PointLight;