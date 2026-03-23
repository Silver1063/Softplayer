#include "types.hpp"

// container for floating point 3D vectors, used for positions, colors, etc. in the renderer
struct vec3f {
	f32 x, y, z;
	vec3f(vec3 v) : x((f32)v.x), y((f32)v.y), z((f32)v.z) {}
};

class vec3 {
public:
	union {
		struct {
			f64 x, y, z;
		};
		f64 v[3]{ 0.0, 0.0, 0.0 };
	};
	vec3();
	vec3(f64 x, f64 y, f64 z);
	vec3(const vec3 &that);
	~vec3();

	vec3 operator+() const;
	vec3 operator+(const vec3 &that) const;
	vec3 operator-() const;
	vec3 operator-(const vec3 &that) const;
	vec3 operator*(const vec3 &that) const;
	vec3 operator*(f64 that) const;
	vec3 operator/(const vec3 &that) const;
	vec3 operator/(f64 that) const;

	vec3 abs() const;
	f64 angle(const vec3 &that) const;
	vec3 ceil() const;
	vec3 clamp(const vec3 &min, const vec3 &max) const;
	vec3 cross(const vec3 &that) const;
	vec3 direction(const vec3 &that) const;
	f64 distance(const vec3 &that) const;
	f64 distance_squared(const vec3 &that) const;
	f64 dot(const vec3 &that) const;
	vec3 floor() const;
	vec3 inverse() const;
	f64 length() const;
	f64 length_squared() const;
	vec3 lerp(const vec3 &that, f64 t) const;
	vec3 max(const vec3 &that) const;
	vec3 min(const vec3 &that) const;
	vec3 move(const vec3 &that, f64 delta) const;
	vec3 normalized() const;
	//mat3 outer(const vec3 &that) const;
	vec3 project(const vec3 &that) const;
	vec3 reflect(const vec3 &normal) const;
	vec3 rotated(const vec3 &axis, f64 angle) const;
	vec3 round() const;
	vec3 sign() const;
	vec3 slerp(const vec3 &that, f64 t) const;
	vec3 slide(const vec3 &normal) const;
	vec3 snapped(const vec3 &step) const;
};