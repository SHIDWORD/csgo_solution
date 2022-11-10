#include "includes.h"

void player_t::modify_eye_position ( animstate_t *state, vec_t *input_eye_pos ) {
	const auto e = state->m_player;

	if ( !e )
		return;

	const auto ground_ent = interfaces.m_entlist->get_by_handle< player_t * > ( e->handle ( ) );

	if ( !state->m_landing || state->m_duck_amount == 0.0f || ground_ent )
		return;

	interfaces.m_mdl_cache->begin_lock ( );

	const auto head_bone = e->lookup_bone ( x_ ( "head_0" ) );

	if ( head_bone != -1 ) {
		vec_t head_pos, temp;
		e->bone_pos ( head_bone, head_pos, temp );

		float v6 = head_pos.z + 1.7f;

		if ( input_eye_pos->z > v6 ) {
			float v8 = 0.0f;
			float v9 = ( float ) ( fabs ( input_eye_pos->z - v6 ) - 4.0f ) * 0.16666667f;

			if ( v9 >= 0.0 )
				v8 = fminf ( v9, 1.0f );

			input_eye_pos->z = ( ( v6 - input_eye_pos->z ) * ( ( ( v8 * v8 ) * 3.0f ) - ( ( ( v8 * v8 ) * 2.0f ) * v8 ) ) ) + input_eye_pos->z;
		}
	}

	interfaces.m_mdl_cache->end_lock ( );
}

vec_t player_t::shoot_pos ( ) {
	vec_t pos;
	util::get_method < void ( __thiscall * )( void *, const vec_t & ) > ( this, 169 )( this, pos );

	if ( use_new_animstate ( ) ) {
		auto state = animstate ( );

		if ( state )
			modify_eye_position ( state, &pos );
	}

	return pos;
}

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

void player_t::bone_pos ( int bone, vec_t &out, vec_t &q ) {
	using bone_pos_fn = void ( __thiscall * )( void *, int, vec_t &, vec_t & );
	static auto o_bone_pos = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 83 E4 F8 56 8B F1 57 83" ) ).as< bone_pos_fn > ( );
	o_bone_pos ( this, bone, out, q );
}

int player_t::lookup_bone ( const char *name ) {
	using lookup_bone_fn = int ( __thiscall * )( void *, const char * );
	static auto o_lookup_bone = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 53 56 8B F1 57 83 ? ? ? ? ? ? 75" ) ).as< lookup_bone_fn > ( );
	return o_lookup_bone ( this, name );
}