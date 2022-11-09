#pragma once

enum class life_states_t : uint8_t {
	alive = 0,
	dying,
	dead,
	reborn,
	discard
};

class entity_t {
public:
	OFFSET ( void *, renderable, 0x4 );
	OFFSET ( bool, dormant, 0xED );
	OFFSET ( void *, networkable, 0x8 );
	NETVAR ( int, team, "DT_BaseEntity", "m_iTeamNum" );
	NETVAR ( int, effects, "DT_BaseEntity", "m_fEffects" );
	
	__forceinline const vec_t &abs_origin ( ) {
		using abs_origin_fn = const vec_t &( __thiscall * )( void * );
		return util::get_method < abs_origin_fn > ( this, 10 )( this );
	}

	__forceinline const vec_t &abs_angles ( ) {
		using abs_angles_fn = const vec_t &( __thiscall * )( void * );
		return util::get_method < abs_angles_fn > ( this, 11 )( this );
	}
};

class player_t : public entity_t {
public:
	NETVAR ( vec_t, mins, "DT_CSPlayer", "m_vecMins" );
	NETVAR ( vec_t, maxs, "DT_CSPlayer", "m_vecMaxs" );
	NETVAR ( int, health, "DT_BasePlayer", "m_iHealth" );
	NETVAR ( life_states_t, life_state, "DT_BasePlayer", "m_lifeState" );
	NETVAR ( int, tick_base, "DT_BasePlayer", "m_nTickBase" );

	__forceinline bool is_alive ( ) {
		return ( life_state ( ) == life_states_t::alive ) && health ( ) > 0;
	}

	void set_abs_origin ( const vec_t &abs_origin );
	void set_abs_angles ( const vec_t &abs_angles );
};