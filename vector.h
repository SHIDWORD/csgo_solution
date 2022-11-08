#pragma once

struct vec2_t {
	float x, y;
};

class vec_t {
public:
    vec_t ( ) = default;
    vec_t ( float x, float y, float z ) : x ( x ), y ( y ), z ( z ) { }

    void init ( ) {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    bool is_zero ( ) {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }

    void init ( float x, float y, float z ) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float &operator( )( const size_t index ) {
        return ( ( float * ) this ) [ index ];
    }

    const float &operator( )( const size_t index ) const {
        return ( ( float * ) this ) [ index ];
    }

    vec_t ( const float *v ) {
        this->x = v [ 0 ]; this->y = v [ 1 ]; this->z = v [ 2 ];
    }

    vec_t operator+( const vec_t &other ) const {
        return vec_t ( x + other.x, y + other.y, z + other.z );
    }

    vec_t operator*=( const vec_t &other ) {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;

        return *this;
    }

    vec_t operator*=( float other ) {
        this->x *= other;
        this->y *= other;
        this->z *= other;

        return *this;
    }

    vec_t operator-( const vec_t &other ) const {
        return vec_t ( x - other.x, y - other.y, z - other.z );
    }

    vec_t operator*( const vec_t &other ) {
        return { x * other.x, y * other.y, z * other.z };
    }

    vec_t operator*( float other ) const {
        return { x * other, y * other, z * other };
    }

    vec_t operator/( const vec_t &other ) {
        return { x / other.x, y / other.y, z / other.z };
    }

    vec_t operator*( const float &other ) {
        return { x * other, y * other, z * other };
    }

    vec_t operator/( const float &other ) {
        return { x / other, y / other, z / other };
    }

    vec_t operator+=( const vec_t &v ) {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    vec_t operator-=( const vec_t &v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    float &operator[ ]( int i ) {
        return ( ( float * ) this ) [ i ];
    }

    float operator[ ]( int i ) const {
        return ( ( float * ) this ) [ i ];
    }

    float length_2d ( ) const {
        return std::sqrtf ( this->x * this->x + this->y * this->y );
    }

    float length ( ) const {
        return std::sqrtf ( this->x * this->x + this->y * this->y + this->z * this->z );
    }

    float dist_to ( const vec_t &other ) {
        vec_t delta { };

        delta.x = this->x - other.x;
        delta.y = this->y - other.y;
        delta.z = this->z - other.z;

        return delta.length ( );
    }

    void normalize ( ) {
        float length = this->length ( );

        if ( length != 0.0f ) {
            this->x /= length;
            this->y /= length;
            this->z /= length;
        }
    }

    void normalize_place ( ) {
        auto res = *this;
        auto radius = std::sqrtf ( x * x + y * y + z * z );
        auto iradius = 1.0f / ( radius + FLT_EPSILON );

        res.x *= iradius;
        res.y *= iradius;
        res.z *= iradius;
    }

    vec_t normalized ( ) const {
        auto vec = *this;
        vec.normalize ( );
        return vec;
    }

    vec_t cross ( const vec_t &other ) const {
        return { this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x };
    }

    float dot ( const vec_t &other ) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    float length_2d_sqr ( ) const {
        return this->x * this->x + this->y * this->y;
    }

    float length_sqr ( ) const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    float x, y, z;
};

class __declspec( align( 16 ) ) vec_aligned_t : public vec_t {
public:
    inline vec_aligned_t ( ) { }
    inline vec_aligned_t ( float x, float y, float z ) {
        init ( x, y, z );
    }

    explicit vec_aligned_t ( const vec_t &other ) {
        init ( other.x, other.y, other.z );
    }

    vec_aligned_t &operator=( const vec_t &other ) {
        init ( other.x, other.y, other.z );
        return *this;
    }

    float w;
};