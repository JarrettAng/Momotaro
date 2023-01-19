/*!************************************************************************
\file MomoMaths.hpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\date 18/01/2023
\brief
This header file declares

**************************************************************************/

#pragma once
template<class T>
struct Vec4
{
	T w;
	T x;
	T y;
	T z;
};

template<class T>
struct Vec3 {
	T x;
	T y;
	T z;
};

/*!***********************************************************************
\brief
	Compare the equality of both Vector 4
\return
	Return true if both vector are equal. Else return false.
*************************************************************************/
template<class T>
bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
{
	if (lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z) {
		return true;
	}
	return false;
}
