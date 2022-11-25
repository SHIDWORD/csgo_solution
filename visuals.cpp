#include "includes.h"

visuals_t visuals { };

void visuals_t::paint ( ) {
	if ( !g.m_local || !g.m_local->alive ( ) )
		return;

	auto weapon = g.m_local->weapon ( );

	if ( !weapon )
		return;

	auto data = weapon->data ( );

	if ( !data )
		return;

	for ( int i = 1; i <= interfaces.m_globals->m_max_clients; ++i ) {
		auto player = interfaces.m_entlist->get< player_t * > ( i );

		if ( !player || player->team ( ) == g.m_local->team ( ) || !player->alive ( ) || player->dormant ( ) )
			continue;

		auto pos = player->shoot_pos ( );

		auto data = penetration.run ( g.m_local->shoot_pos ( ), pos, player );

		vec_t screen_from, screen_to;

		render.world_to_screen ( pos, screen_to );
		render.world_to_screen ( g.m_local->shoot_pos ( ), screen_from );

		const auto end = data.m_end;

		vec_t screen_end;
		render.world_to_screen ( end, screen_end );
		render.line ( screen_from.x, screen_from.y, screen_end.x, screen_end.y, { 255, 255, 255 } );

		//trace_t tr;
		//trace_filter_t filter;
		//filter.m_skip = g.m_local;

		//interfaces.m_trace->trace_ray ( ray, 0x4600400B, &filter, &tr );

		//vec_t screen_endpos;
		//render.world_to_screen ( tr.m_endpos, screen_endpos );

		//vec_t screen_origin;
		//render.world_to_screen ( g.m_local->origin( ), screen_origin );

		//render.line ( screen_origin.x, screen_origin.y, screen_endpos.x, screen_endpos.y, { 0, 255, 0 } );
		render.string ( fonts [ fonts_t::debug_font ].m_data, screen_to.x, screen_to.y, { 255, 255, 255, 200 }, tinyformat::format ( "Damage: %.f", data.m_out_damage ).c_str ( ) );
	}
}

bool visuals_t::get_box_bounds ( player_t *ent, box_t &box ) {
	vec_t flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	auto origin = ent->origin ( );

	auto min = ent->mins ( ) + origin;
	auto max = ent->maxs ( ) + origin;

	vec_t points [ ] = {
		vec_t ( min.x, min.y, min.z ),
		vec_t ( min.x, min.y, max.z ),
		vec_t ( min.x, max.y, min.z ),
		vec_t ( min.x, max.y, max.z ),
		vec_t ( max.x, min.y, min.z ),
		vec_t ( max.x, min.y, max.z ),
		vec_t ( max.x, max.y, min.z ),
		vec_t ( max.x, max.y, max.z ),
	};

	if ( !render.world_to_screen ( points [ 3 ], flb )
		|| !render.world_to_screen ( points [ 5 ], brt )
		|| !render.world_to_screen ( points [ 0 ], blb )
		|| !render.world_to_screen ( points [ 4 ], frt )
		|| !render.world_to_screen ( points [ 2 ], frb )
		|| !render.world_to_screen ( points [ 1 ], brb )
		|| !render.world_to_screen ( points [ 6 ], blt )
		|| !render.world_to_screen ( points [ 7 ], flt ) )
		return false;

	vec_t arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for ( auto i = 1; i < 8; i++ ) {
		if ( left > arr [ i ].x )
			left = arr [ i ].x;

		if ( bottom < arr [ i ].y )
			bottom = arr [ i ].y;

		if ( right < arr [ i ].x )
			right = arr [ i ].x;

		if ( top > arr [ i ].y )
			top = arr [ i ].y;
	}

	box.x = static_cast< int >( left );
	box.y = static_cast< int >( top );
	box.w = static_cast< int >( right - left );
	box.h = static_cast< int >( bottom - top );

	return true;
}