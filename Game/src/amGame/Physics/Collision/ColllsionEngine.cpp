//#include "ColllsionEngine.hpp"
//
//#include "BoundingBox.hpp"
//
//bool CollisionEngine::AreColliding(const BoundingBox& lhs, const BoundingBox& rhs)
//{
//	return (
//		lhs.m_minCorner.x <= rhs.m_maxCorner.x && lhs.m_maxCorner.x >= rhs.m_minCorner.x
//		&& lhs.m_minCorner.y <= rhs.m_maxCorner.y && lhs.m_maxCorner.y >= rhs.m_minCorner.y
//		&& lhs.m_minCorner.z <= rhs.m_maxCorner.z && lhs.m_maxCorner.z >= rhs.m_minCorner.z);
//}