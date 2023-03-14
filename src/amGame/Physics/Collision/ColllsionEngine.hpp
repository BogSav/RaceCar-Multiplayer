#pragma once

class CollisionEngine
{
private:
	CollisionEngine() = default;

public:
	static bool AreColliding(const class BoundingBox& lhs, const class BoundingBox& rhs);
};