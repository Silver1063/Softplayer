#include "types.hpp"


class vec3 {
public:
	union {
		struct {
			f32 x, y, z;
		};
		f32 v[3]{ 0.0f, 0.0f, 0.0f };
	};
	vec3();
	vec3(f32 x, f32 y, f32 z);
	vec3(const vec3 &that);
	~vec3();

	vec3 operator+() const;
	vec3 operator+(const vec3 &that) const;
	vec3 operator-() const;
	vec3 operator-(const vec3 &that) const;
	vec3 operator*(const vec3 &that) const;
	vec3 operator*(f32 that) const;
	vec3 operator/(const vec3 &that) const;
	vec3 operator/(f32 that) const;

	vec3 abs() const;
	f32 angle(const vec3 &that) const;
	vec3 ceil() const;
	vec3 clamp(const vec3 &min, const vec3 &max) const;
	vec3 cross(const vec3 &that) const;
	vec3 direction(const vec3 &that) const;
	f32 distance(const vec3 &that) const;
	f32 distance_squared(const vec3 &that) const;
	f32 dot(const vec3 &that) const;
	vec3 floor() const;
	vec3 inverse() const;
	f32 length() const;
	f32 length_squared() const;
	vec3 lerp(const vec3 &that, f32 t) const;
	vec3 max(const vec3 &that) const;
	vec3 min(const vec3 &that) const;
	vec3 move(const vec3 &that, f32 delta) const;
	vec3 normalized() const;
	//mat3 outer(const vec3 &that) const;
	vec3 project(const vec3 &that) const;
	vec3 reflect(const vec3 &normal) const;
	vec3 rotated(const vec3 &axis, f32 angle) const;
	vec3 round() const;
	vec3 sign() const;
	vec3 slerp(const vec3 &that, f32 t) const;
	vec3 slide(const vec3 &normal) const;
	vec3 snapped(const vec3 &step) const;
};