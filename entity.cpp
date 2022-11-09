#include "includes.h"

void player_t::set_abs_origin ( const vec_t &abs_origin ) {
	using set_abs_origin_fn = void ( __thiscall * )( void *, const vec_t & );
	static auto o_set_abs_origin = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) ).as< set_abs_origin_fn > ( );
	o_set_abs_origin ( this, abs_origin );
}

void player_t::set_abs_angles ( const vec_t &abs_angles ) {
	using set_abs_angles_fn = void ( __thiscall * )( void *, const vec_t & );
	static auto o_set_abs_angles = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) ).as< set_abs_angles_fn > ( );
	o_set_abs_angles ( this, abs_angles );
}