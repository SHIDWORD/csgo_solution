#pragma once

class entity_t {
public:
	OFFSET ( void *, renderable, 0x4 );
	OFFSET ( void *, networkable, 0x8 );
	OFFSET ( int, idx, 0x64 );
	OFFSET ( bool, dormant, 0xED );
	OFFSET ( float, fall_vel, 0xC09 );
	OFFSET ( int, think_tick, 0xFC );
	OFFSET ( ucmd_t *, current_cmd, 0x3348 );
	OFFSET ( ucmd_t, last_cmd, 0x3298 );
	OFFSET ( int, buttons, 0x31FC );
	NETVAR ( int, team, "DT_BaseEntity", "m_iTeamNum" );
	NETVAR ( int, effects, "DT_BaseEntity", "m_fEffects" );
	NETVAR ( vec_t, origin, "DT_BaseEntity", "m_vecOrigin" );

	__forceinline vec_t &abs_origin ( ) {
		using abs_origin_fn = vec_t & ( __thiscall * )( void * );
		return util::get_method < abs_origin_fn > ( this, 10 )( this );
	}

	__forceinline vec_t &abs_angles ( ) {
		using abs_angles_fn = vec_t & ( __thiscall * )( void * );
		return util::get_method < abs_angles_fn > ( this, 11 )( this );
	}
};