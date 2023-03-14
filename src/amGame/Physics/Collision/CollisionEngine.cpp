#include "CollisionEngine.hpp"

#include "MainMenu/GameComponents/Car.hpp"
#include "MainMenu/Lighting/StreetLight.hpp"
#include "MainMenu/GameComponents/NPC.hpp"
#include "MainMenu/GameComponents/Track.hpp"
#include "MainMenu/GameComponents/Tree.hpp"

bool CollisionEngine::IsOnTrack(const Track* track, const Car* car)
{
	return CollisionEngine::IsOnTrack(track, car->GetPosition());
}

bool CollisionEngine::IsOnTrack(const Track* track, const Tree* tree)
{
	return CollisionEngine::IsOnTrack(track, tree->GetTrunkCenter());
}

bool CollisionEngine::IsOnTrack(const Track* track, const StreetLight* streetLight)
{
	return CollisionEngine::IsOnTrack(track, streetLight->GetBasePosition());
}

bool CollisionEngine::IsCollidingWithNPC(const Car* masina, const NPC* npc)
{
	return CollisionEngine::IsInside(
		npc->m_position,
		npc->m_position + npc->GetNormalToDirection() * npc->m_width,
		npc->m_position + npc->GetAntiMoveDirection() * npc->m_length
			+ npc->GetNormalToDirection() * npc->m_width,
		npc->m_position + npc->GetAntiMoveDirection() * npc->m_length,
		masina->m_position);
}

int CollisionEngine::GetCurrentPlaceOnTrackForCar(const Track* track, const Car* car)
{
	size_t it;
	for (it = 0; it < track->m_trackPoints.size() - 1; it++)
	{
		if (CollisionEngine::IsInside(
				track->m_exteriorPoints[it],
				track->m_trackPoints[it],
				track->m_trackPoints[it + 1],
				car->GetPosition())
			|| CollisionEngine::IsInside(
				track->m_exteriorPoints[it],
				track->m_trackPoints[it + 1],
				track->m_exteriorPoints[it + 1],
				car->GetPosition()))
		{
			break;
		}
	}
	return static_cast<int>(it);
}

inline bool CollisionEngine::IsOnTrack(const Track* track, const glm::vec3& pos)
{
	size_t it;
	for (it = 0; it < track->m_trackPoints.size() - 1; it++)
	{
		if (CollisionEngine::IsInside(
				track->m_exteriorPoints[it],
				track->m_trackPoints[it],
				track->m_trackPoints[it + 1],
				pos)
			|| CollisionEngine::IsInside(
				track->m_exteriorPoints[it],
				track->m_trackPoints[it + 1],
				track->m_exteriorPoints[it + 1],
				pos))
		{
			return true;
		}
	}
	
	if (CollisionEngine::IsInside(
			track->m_exteriorPoints[it], track->m_trackPoints[it], track->m_trackPoints[0], pos)
		|| CollisionEngine::IsInside(
			track->m_exteriorPoints[it], track->m_trackPoints[0], track->m_exteriorPoints[0], pos))
		return true;

	return false;
}

constexpr inline bool CollisionEngine::IsInside(
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

inline bool CollisionEngine::IsInside(
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

float inline constexpr CollisionEngine::sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
}