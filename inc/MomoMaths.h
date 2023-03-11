/*!************************************************************************
\file MomoMaths.h
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 18-01-2023
\brief
This header file declares

**************************************************************************/

#pragma once
#include <iostream>
#include <AEEngine.h>

template<class T>
struct Vec2 {
	T x;
	T y;

	Vec2& operator+=(Vec2 const& rhs);
	Vec2& operator-=(Vec2 const& rhs);
	Vec2& operator*=(T  rhs);
};

template<class T>
struct Vec3 {
	T x;
	T y;
	T z;

	Vec3& operator+=(Vec2<T> const& rhs);
	Vec3& operator-=(Vec2<T> const& rhs);
	Vec3& operator+=(Vec3 const& rhs);
	Vec3& operator-=(Vec3 const& rhs);
	Vec3& operator*=(T  rhs);
};

template<class T>
struct Vec4
{
	T w;
	T x;
	T y;
	T z;

	Vec4& operator-=(Vec2<T> const& rhs);
	Vec4& operator+=(Vec2<T> const& rhs);
	Vec4& operator-=(Vec3<T> const& rhs);
	Vec4& operator+=(Vec3<T> const& rhs);
	Vec4& operator+=(Vec4 const& rhs);
	Vec4& operator-=(Vec4 const& rhs);
	Vec4& operator*=(T rhs);
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

/*!***********************************************************************
\brief
	Compare the equality of both Vector 3
\return
	Return true if both vector are equal. Else return false.
*************************************************************************/
template<class T>
bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
}

/*!***********************************************************************
\brief
	Compare the equality of both Vector 2
\return
	Return true if both vector are equal. Else return false.
*************************************************************************/
template<class T>
bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}
template<class T>
bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs)
{
	return !(lhs.x == rhs.x && lhs.y == rhs.y);
}

/*============================================================================
* OPERATOR OVER LOADS FOR VECTOR ADDITION
*/
template<class T>
Vec2<T>& Vec2<T>::operator+=(Vec2<T>const& rhs) {	//vec2+=vec2
	x += rhs.x;
	y += rhs.y;
	return*this;

}
template <class T>
Vec3<T>& Vec3<T>::operator+=(Vec2<T> const& rhs)	//vec3+=vec2
{
	x += rhs.x;
	y += rhs.y;
	return*this;
}

template <class T>
Vec3<T>& Vec3<T>::operator+=(Vec3<T> const& rhs) //vec3+=vec3
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return*this;
}

template <class T>
Vec4<T>& Vec4<T>::operator+=(Vec2<T> const& rhs) //vec4+=vec2
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator+=(Vec3<T> const& rhs) //vec4+=vec3
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator+=(Vec4<T> const& rhs) //vec4+=vec4
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return*this;
}

//BINARY ADDITION
template<class T>
Vec2<T> operator+(Vec2<T> const& lhs, Vec2<T>const& rhs) {	//vec2+vec2
	Vec2<T> temp{ lhs };
	temp += rhs;
	return temp;
}
template<class T>
Vec3<T> operator+(Vec3<T> const& lhs, Vec3<T>const& rhs) {	//vec3+vec3
	Vec3<T> temp{ lhs };
	temp += rhs;
	return temp;
}
template<class T>
Vec3<T> operator+(Vec3<T> const& lhs, Vec2<T>const& rhs) {	//vec3+vec2
	Vec3<T> temp{ lhs };
	temp += rhs;
	return temp;
}
template<class T>
Vec3<T> operator+(Vec2<T> const& lhs, Vec3<T>const& rhs) {	//vec2+vec3
	Vec3<T> temp{ rhs };
	temp += lhs;
	return temp;
}
template<class T>
Vec4<T> operator+(Vec3<T> const& lhs, Vec4<T>const& rhs) {	//vec3+vec4
	Vec4<T> temp{ rhs };
	temp += lhs;
	return temp;
}
template<class T>
Vec4<T> operator+(Vec2<T> const& lhs, Vec4<T>const& rhs) {	//vec2+vec4
	Vec4<T> temp{ rhs };
	temp += lhs;
	return temp;
}
template<class T>
Vec4<T> operator+(Vec4<T> const& lhs, Vec2<T>const& rhs) {	//vec4+vec2
	Vec4<T> temp{ lhs };
	temp += rhs;
	return temp;
}
template<class T>
Vec4<T> operator+(Vec4<T> const& lhs, Vec3<T>const& rhs) {	//vec4+vec3
	Vec4<T> temp{ lhs };
	temp += rhs;
	return temp;
}
template<class T>
Vec4<T> operator+(Vec4<T> const& lhs, Vec4<T>const& rhs) {	//vec4+vec4
	Vec4<T> temp{ lhs };
	temp += rhs;
	return temp;
}

/*============================================================================
* OPERATOR OVER LOADS FOR VECTOR SUBTRACTION
*/
template<class T>
Vec2<T>& Vec2<T>::operator-=(Vec2<T>const& rhs) {	//vec2-=vec2
	x -= rhs.x;
	y -= rhs.y;
	return*this;
}

template <class T>
Vec3<T>& Vec3<T>::operator-=(Vec2<T> const& rhs) // vec3-=vec2
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
template<class T>
Vec3<T>& Vec3<T>::operator-=(Vec3<T>const& rhs) { //vec3-=vec3
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return*this;
}
template <class T>
Vec4<T>& Vec4<T>::operator-=(Vec2<T> const& rhs) //vec4-=vec2
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator-=(Vec3<T> const& rhs) //vec4-=vec3
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}
template<class T>
Vec4<T>& Vec4<T>::operator-=(Vec4<T>const& rhs) { //vec4-=vec4
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return*this;
}

//BINARY SUBTRACTION
template<class T>
Vec2<T> operator-(Vec2<T> const& lhs, Vec2<T>const& rhs) {	//vec2-vec2
	Vec2<T> temp{ lhs };
	temp -= rhs;
	return temp;
}
template<class T>
Vec3<T> operator-(Vec3<T> const& lhs, Vec2<T>const& rhs) {	//vec3-vec2
	Vec3<T> temp{ lhs };
	temp -= rhs;
	return temp;
}
// template<class T>
// Vec3<T> operator-(Vec2<T> const& lhs, Vec3<T>const& rhs){	//vec2-vec3
// 	Vec3<T> temp;
// 	temp-=rhs;
// 	return temp;
// } 
template<class T>
Vec3<T> operator-(Vec3<T> const& lhs, Vec3<T>const& rhs) {	//vec3-vec3
	Vec3<T> temp{ lhs };
	temp -= rhs;
	return temp;
}
// template<class T>
// Vec4<T> operator-(Vec2<T> const& lhs, Vec4<T>const& rhs){	//vec2-vec4
// 	Vec4<T> temp{lhs};
// 	temp-=rhs;
// 	return temp;
// } 
// template<class T>
// Vec4<T> operator-(Vec3<T> const& lhs, Vec4<T>const& rhs){	//vec3-vec4
// 	Vec4<T> temp{lhs};
// 	temp-=rhs;
// 	return temp;
// } 
template<class T>
Vec4<T> operator-(Vec4<T> const& lhs, Vec2<T>const& rhs) {	//vec4-vec2
	Vec4<T> temp{ lhs };
	temp -= rhs;
	return temp;
}
template<class T>
Vec4<T> operator-(Vec4<T> const& lhs, Vec3<T>const& rhs) {	//vec4-vec3
	Vec4<T> temp{ lhs };
	temp -= rhs;
	return temp;
}
template<class T>
Vec4<T> operator-(Vec4<T> const& lhs, Vec4<T>const& rhs) {	//vec4-vec4
	Vec4<T> temp{ lhs };
	temp -= rhs;
	return temp;
}

/*============================================================================
* OPERATOR OVER LOADS FOR VECTOR MULTIPLICATION
*/
template <class T>
Vec2<T>& Vec2<T>::operator*=(T rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}
template <class T>
Vec3<T>& Vec3<T>::operator*=(T rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator*=(T rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

template<class T>
Vec2<T> operator*(Vec2<T> const& lhs, T rhs) {
	Vec2<T> temp{ lhs };
	temp.x *= rhs;
	temp.y *= rhs;
	return temp;
}
template<class T>
Vec3<T> operator*(Vec3<T> const& lhs, T rhs) {
	Vec3<T> temp{ lhs };
	temp.x *= rhs;
	temp.y *= rhs;
	temp.z *= rhs;
	return temp;
}
template<class T>
Vec4<T> operator*(Vec4<T> const& lhs, T rhs) {
	Vec4<T> temp{ lhs };
	temp.x *= rhs;
	temp.y *= rhs;
	temp.z *= rhs;
	temp.w *= rhs;
	return temp;
}
/*============================================================================
* OPERATOR OVERLOADS FOR VECTOR COMPARISONS
*/
template<class T>
bool operator<(Vec2<T> const& lhs, Vec2<T> const& rhs) {
	if (lhs.x < rhs.x) return true;
	if (lhs.x == rhs.x) {
		return (lhs.y < rhs.y);
	}
	return false;
}

/*============================================================================
* OPERATOR OVER LOADS FOR VECTOR TO OUTPUT STREAM
*/
// template<class T>
// std::ostream& operator<<(std::ostream& os, Vec2<T> const&);
// template<class T>
// std::ostream& operator<<(std::ostream& os, Vec3<T> const&);
// template<class T>
// std::ostream& operator<<(std::ostream& os, Vec4<T> const&);

template <class T>
inline std::ostream& operator<<(std::ostream& os, Vec2<T> const& rhs)
{
	os << "(x,y) : ";
	return os << "(" << rhs.x << "," << rhs.y << ")";
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, Vec3<T> const& rhs)
{
	os << "(x,y,z) : ";
	return os << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, Vec4<T> const& rhs)
{
	os << "(x,y,z,w) : ";
	return os << "(" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << ")";
}

template <typename T>
inline bool IsPointWithinRect(Vec2<T> point, Vec2<T> pos, Vec2<T> size) {
	if (point.x < pos.x || point.x > pos.x + size.x ||
		point.y < pos.y - size.y || point.y > pos.y) return false;
	return true;
}

// How many percentage from the left.
// Left side of world pos is negative.
// Right side of world pos is positive.
inline float GetWorldXByPercentage(float percent) {
	return AEGfxGetWinMinX() * ((50 - percent) / 50.0f);
}

// How many percent from the left
// Top side of world pos is positive.
// Bottom side of world pos is negative.
inline float GetWorldYByPercentage(float percent) {
	return AEGfxGetWinMinY() * ((50 - percent) / 50.0f);
}

inline bool MouseInsideButton(Vec2<int> mousePos, Vec2<float> btnPos, Vec2<float> btnSize) {
	// LEFT / RIGHT BOUNDS
	if ((mousePos.x > btnPos.x &&
		mousePos.x < btnPos.x + btnSize.x) &&

		// TOP / BOTTOM BOUNDS
		(mousePos.y > -btnPos.y &&
			mousePos.y < -btnPos.y + btnSize.y)) {

		return true;
	}
	return false;
}
