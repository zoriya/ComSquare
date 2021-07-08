//
// Created by cbihan on 5/13/20.
//

#pragma once

#include <ostream>
#include <SFML/System/Vector2.hpp>

namespace ComSquare
{
	template<typename T>
	class Vector2
	{
	public:
		T x;
		T y;

		Vector2()
			: x(0), y(0)
		{}

		Vector2(T _x, T _y)
			: x(_x), y(_y)
		{}

		Vector2(sf::Vector2<T> v) // NOLINT(google-explicit-constructor)
			: x(v.x), y(v.y)
		{}

		template<typename T2>
		Vector2<T> &operator+=(const Vector2<T2> &vec)
		{
			this->x += vec.x;
			this->y += vec.y;
			return *this;
		}

		template<typename T2>
		Vector2<T> operator+(const Vector2<T2> &vec) const
		{
			return Vector2<T>(this->x + vec.x, this->y + vec.y);
		}

		template<typename T2>
		Vector2<T> &operator-=(const Vector2<T2> &vec)
		{
			this->x -= vec.x;
			this->y -= vec.y;
			return *this;
		}

		template<typename T2>
		Vector2<T> operator-(const Vector2<T2> &vec) const
		{
			return Vector2<T>(this->x - vec.x, this->y - vec.y);
		}

		template<typename T2>
		Vector2<T> &operator*=(T2 d)
		{
			this->x *= d;
			this->y *= d;
			return *this;
		}

		template<typename T2>
		Vector2<T> operator*(T2 d) const
		{
			return Vector2<T>(this->x * d, this->y * d);
		}

		template<typename T2>
		T operator*(Vector2<T2> &b) const
		{
			return this->x * b.x + this->y * b.y;
		}

		template<typename T2>
		Vector2<T> operator/=(Vector2<T2> &b)
		{
			this->x /= b.x;
			this->y /= b.y;
			return this;
		}

		template<typename T2>
		Vector2<T> operator/(Vector2<T2> &b) const
		{
			return Vector2<T>(this->x / b.x, this->y / b.y);
		}

		template<typename T2>
		Vector2<T> operator/=(T2 b)
		{
			this->x /= b;
			this->y /= b;
			return this;
		}

		template<typename T2>
		Vector2<T> operator/(T2 b) const
		{
			return Vector2<T>(this->x / b, this->y / b);
		}
	};

	typedef Vector2<float> Vector2f;

	typedef Vector2<unsigned> Vector2u;

	typedef Vector2<int> Vector2i;
}

template<typename T>
std::ostream &operator<<(std::ostream &s, const ComSquare::Vector2<T> &v)
{
	s << v.x << " " << v.y;
	return s;
}
