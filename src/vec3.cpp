#include "vec3.hpp"

#include <cmath>

vec3::vec3() : x(0.0), y(0.0), z(0.0) {}
vec3::vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
vec3::vec3(const vec3 &that) : x(that.x), y(that.y), z(that.z) {}
vec3::~vec3() {}

vec3 vec3::operator+() const {
	return *this;
}

vec3 vec3::operator+(const vec3 &that) const {
	return vec3(
			this->x + that.x,
			this->y + that.y,
			this->z + that.z);
}

vec3 vec3::operator-() const {
	return vec3(
			-this->x,
			-this->y,
			-this->z);
}

vec3 vec3::operator-(const vec3 &that) const {
	return vec3(
			this->x - that.x,
			this->y - that.y,
			this->z - that.z);
}

vec3 vec3::operator*(const vec3 &that) const {
	return vec3(
			this->x * that.x,
			this->y * that.y,
			this->z * that.z);
}

vec3 vec3::operator*(f32 that) const {
	return vec3(
			this->x * that,
			this->y * that,
			this->z * that);
}

vec3 vec3::operator/(const vec3 &that) const {
	return vec3(
			this->x / that.x,
			this->y / that.y,
			this->z / that.z);
}

vec3 vec3::operator/(f32 that) const {
	return vec3(
			this->x / that,
			this->y / that,
			this->z / that);
}

vec3 vec3::abs() const {
	return vec3(
			std::abs(this->x),
			std::abs(this->y),
			std::abs(this->z));
}

f32 vec3::angle(const vec3 &that) const {
	return std::atan2(this->cross(that).length(), this->dot(that));
}

vec3 vec3::ceil() const {
	return vec3(
			std::ceil(this->x),
			std::ceil(this->y),
			std::ceil(this->z));
}

vec3 vec3::clamp(const vec3 &min, const vec3 &max) const {
	return this->max(min).min(max);
}

vec3 vec3::cross(const vec3 &that) const {
	return vec3(
			this->y * that.z - this->z * that.y,
			this->z * that.x - this->x * that.z,
			this->x * that.y - this->y * that.x);
}

vec3 vec3::direction(const vec3 &that) const {
	return (that - *this).normalized();
}

f32 vec3::distance(const vec3 &that) const {
	return (that - *this).length();
}

f32 vec3::distance_squared(const vec3 &that) const {
	return (that - *this).length_squared();
}

f32 vec3::dot(const vec3 &that) const {
	return this->x * that.x + this->y * that.y + this->z * that.z;
}

vec3 vec3::floor() const {
	return vec3(
			std::floor(this->x),
			std::floor(this->y),
			std::floor(this->z));
}

vec3 vec3::inverse() const {
	return vec3(
			1.0 / this->x,
			1.0 / this->y,
			1.0 / this->z);
}

f32 vec3::length() const {
	return std::sqrt(this->length_squared());
}

f32 vec3::length_squared() const {
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

vec3 vec3::lerp(const vec3 &that, f32 t) const {
	return vec3(
			std::lerp(this->x, that.x, t),
			std::lerp(this->y, that.y, t),
			std::lerp(this->z, that.z, t));
}

vec3 vec3::max(const vec3 &that) const {
	return vec3(
			std::fmax(this->x, that.x),
			std::fmax(this->y, that.y),
			std::fmax(this->z, that.z));
}

vec3 vec3::min(const vec3 &that) const {
	return vec3(
			std::fmin(this->x, that.x),
			std::fmin(this->y, that.y),
			std::fmin(this->z, that.z));
}

vec3 vec3::move(const vec3 &that, f32 delta) const {
	// come back later lol
	return vec3();
}

vec3 vec3::normalized() const {
	f32 l = this->length();
	if (l <= 0.0) {
		return vec3(0.0, 0.0, 0.0);
	}
	return vec3(x / l, y / l, z / l);
}

//mat3 outer(const vec3 &that) const;

vec3 vec3::project(const vec3 &that) const {
	f32 dot_product = this->dot(that);
	f32 that_length_squared = that.length_squared();
	if (that_length_squared <= f32_EPSILON) {
		return vec3(0.0, 0.0, 0.0);
	}
	return that * (dot_product / that_length_squared);
}

vec3 vec3::reflect(const vec3 &normal) const {
	//return *this - normal * (2.0 * this->dot(normal));
}

vec3 vec3::rotated(const vec3 &axis, f32 angle) const {
	// come back later lol
	return vec3();
}

vec3 vec3::round() const {
	return vec3(std::round(x), std::round(y), std::round(z));
}

vec3 vec3::sign() const {
	return vec3(std::signbit(x) ? -1.0 : 1.0, std::signbit(y) ? -1.0 : 1.0, std::signbit(z) ? -1.0 : 1.0);
}

vec3 vec3::slerp(const vec3 &that, f32 t) const {
	//?
}

vec3 vec3::slide(const vec3 &normal) const {
	return *this - normal * this->dot(normal);
}

vec3 vec3::snapped(const vec3 &step) const {
	return (*this / step).round() * step;
}