#pragma once

#include "LightSource.hpp"
#include "core/gpu/shader.h"

class LightSourcesVector : public std::vector<const LightSourceAdapter*>
{
public:
	LightSourcesVector() = default;
	LightSourcesVector(const LightSourcesVector&) = delete;
	LightSourceAdapter& operator=(const LightSourcesVector&) = delete;

	void reserve(size_type new_cap)
	{
		CheckIfOperationIsLegal(new_cap);

		std::vector<const LightSourceAdapter*>::reserve(new_cap);
	}

	void resize(size_type count)
	{
		CheckIfOperationIsLegal(count);

		std::vector<const LightSourceAdapter*>::resize(count);
	}

	void push_back(const LightSourceAdapter* lightSource)
	{
		nrOfLightSources++;

		CheckIfOperationIsLegal(nrOfLightSources);

		std::vector<const LightSourceAdapter*>::push_back(lightSource);
	}
	void push_back(const std::pair<const LightSourceAdapter*, const LightSourceAdapter*>& lights)
	{
		nrOfLightSources += 2;

		CheckIfOperationIsLegal(nrOfLightSources);

		std::vector<const LightSourceAdapter*>::push_back(lights.first);
		std::vector<const LightSourceAdapter*>::push_back(lights.second);
	}

private:
	void CheckIfOperationIsLegal(size_t nr)
	{
		if (nr > MAX_LIGHTING_SOURCES)
			throw std::exception("Prea multe lumini");
	}

private:
	size_t nrOfLightSources = 0;
};