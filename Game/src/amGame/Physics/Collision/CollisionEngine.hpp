//#pragma once
//
//#include "MainMenu/GameComponents/GameComponent.hpp"
//
//class CollisionEngine
//{
//private:
//	CollisionEngine();
//
//public:
//	static bool IsOnTrack(const class Track* track, const class Car* car);
//	static bool IsOnTrack(const class Track* track, const class StreetLight* streetLight);
//	static bool IsOnTrack(const class Track* track, const class Tree* tree);
//
//	static bool IsCollidingWithNPC(const class Car* masina, const class NPC* npc);
//
//	static int GetCurrentPlaceOnTrackForCar(const class Track* track, const class Car* car);
//
//private:
//	// Check verification for convex polygons - used for car and NPCs
//	static inline constexpr bool IsInside(
//		const glm::vec3& v1,
//		const glm::vec3& v2,
//		const glm::vec3& v3,
//		const glm::vec3& v4,
//		const glm::vec3& point);
//
//	// Check verification for 2d triangles - used for track
//	static inline constexpr float sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
//	static inline bool IsInside(
//		const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& point);
//
//	// Check if a point belongs to the track
//	static inline bool IsOnTrack(const class Track* track, const glm::vec3& pos);
//};
