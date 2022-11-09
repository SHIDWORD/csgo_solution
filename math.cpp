#include "includes.h"

math_t math { };

void math_t::vec_transform ( const float *in1, const matrix3x4_t &in2, float *out ) {
	out [ 0 ] = dot_product ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
	out [ 1 ] = dot_product ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
	out [ 2 ] = dot_product ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
}

void math_t::vec_transform ( const vec_t &in1, const matrix3x4_t &in2, vec_t &out ) {
	vec_transform ( &in1.x, in2, &out.x );
}

float math_t::distance_to_ray ( const vec_t &pos, const vec_t &ray_start, const vec_t &ray_end, float *along, vec_t *point_on_ray ) {
	vec_t to = ( pos - ray_start );
	vec_t dir = ( ray_end - ray_start );

	float length = dir.length_2d ( );
	float range_along = to.dot ( dir ) / length;

	if ( along )
		*along = range_along;

	float range;

	if ( range_along < 0.0f ) {
		range = -( pos - ray_start ).length_2d ( );

		if ( point_on_ray )
			*point_on_ray = ray_start;
	}
	else if ( range_along > length ) {
		range = -( pos - ray_end ).length_2d ( );

		if ( point_on_ray )
			*point_on_ray = ray_end;
	}
	else {
		vec_t on_ray = ray_start + dir * range_along;
		range = ( pos - on_ray ).length_2d ( );

		if ( point_on_ray )
			*point_on_ray = on_ray;
	}

	return range;
}

vec_t math_t::ang_vector ( vec_t angle ) {
	vec_t vec { };

	vec.x = cos ( angle.y ) * cos ( angle.x );
	vec.y = sin ( angle.y ) * cos ( angle.x );
	vec.z = sin ( angle.x );

	return vec;
}

void math_t::sin_cos ( float radians, float *sin, float *cos ) {
	*sin = std::sin ( radians );
	*cos = std::cos ( radians );
}

void math_t::ang_vector ( const vec_t &angle, vec_t *forward, vec_t *right, vec_t *up ) {
	float sr, sp, sy, cr, cp, cy;

	sin_cos ( deg2rad ( angle [ 1 ] ), &sy, &cy );
	sin_cos ( deg2rad ( angle [ 0 ] ), &sp, &cp );
	sin_cos ( deg2rad ( angle [ 2 ] ), &sr, &cr );

	if ( forward ) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if ( right ) {
		right->x = ( -1 * sr * sp * cy + -1 * cr * -sy );
		right->y = ( -1 * sr * sp * sy + -1 * cr * cy );
		right->z = -1 * sr * cp;
	}

	if ( up ) {
		up->x = ( cr * sp * cy + -sr * -sy );
		up->y = ( cr * sp * sy + -sr * cy );
		up->z = cr * cp;
	}
}

float math_t::dot_product ( const vec_t &a, const vec_t &b ) {
	return { a.x * b.x + a.y * b.y + a.z * b.z };
}

vec_t math_t::vec_angle ( vec_t vec ) {
	vec_t ret;

	if ( vec.x == 0.0f && vec.y == 0.0f ) {
		ret.x = ( vec.z > 0.0f ) ? 270.0f : 90.0f;
		ret.y = 0.0f;
	}
	else {
		ret.x = rad2deg ( std::atan2f ( -vec.z, vec.length_2d ( ) ) );
		ret.y = rad2deg ( std::atan2f ( vec.y, vec.x ) );

		if ( ret.y < 0.0f )
			ret.y += 360.0f;

		if ( ret.x < 0.0f )
			ret.x += 360.0f;
	}

	ret.z = 0.0f;

	clamp ( ret );

	return ret;
}

void math_t::clamp ( vec_t &ang ) {
	auto flt_valid = [ ] ( float val ) {
		return std::isfinite ( val ) && !std::isnan ( val );
	};

	for ( auto i = 0; i < 3; i++ )
		if ( !flt_valid ( ang [ i ] ) )
			ang [ i ] = 0.0f;

	ang.x = std::clamp ( normalize ( ang.x ), -89.0f, 89.0f );
	ang.y = std::clamp ( normalize ( ang.y ), -180.0f, 180.0f );
	ang.z = 0.0f;
}

float math_t::normalize ( float ang ) {
	if ( isnan ( ang ) || isinf ( ang ) )
		ang = 0.0f;

	while ( ang > 180.0f )
		ang -= 360.0f;

	while ( ang < -180.0f )
		ang += 360.0f;

	return ang;
}

vec_t math_t::calc_angle ( vec_t src, vec_t dst ) {
	auto ret = vec_angle ( dst - src );

	clamp ( ret );

	return ret;
}