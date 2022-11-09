#include "includes.h"

visuals_t visuals { };

void visuals_t::paint ( ) {
	for ( int i = 1; i <= interfaces.m_globals->m_max_clients; i++ ) {
		auto pl = interfaces.m_entlist->get< player_t* > ( i );

		if ( !pl || pl->dormant ( ) || !pl->is_alive ( ) || pl->team ( ) == g.m_local->team ( ) )
			continue;
        
        box_t box;
		if ( !get_box_bounds ( pl, box ) )
			continue;

		player_info_t info;

		if ( !interfaces.m_engine->get_player_info ( i, &info ) )
			continue;

		/* render box. */
        render.outlined_rect ( box.x - 1, box.y - 1, box.w + 2, box.h + 2, { 0, 0, 0, 200 } );
		render.outlined_rect ( box.x, box.y, box.w, box.h, { 255, 255, 255, 200 } );
        render.outlined_rect ( box.x + 1, box.y + 1, box.w - 2, box.h - 2, { 0, 0, 0, 200 } );

		auto dim = render.get_text_size ( fonts [ fonts_t::default_font ].m_data, info.m_name );

        /* render name. */
		render.string ( fonts [ fonts_t::default_font ].m_data, box.x + ( box.w / 2 ) - ( dim.x / 2 ), box.y - fonts [ fonts_t::default_font ].m_height - 1, { 255, 255, 255, 200 }, info.m_name );
	}
}

bool visuals_t::get_box_bounds ( player_t *ent, box_t &box ) {
    vec_t flb, brt, blb, frt, frb, brb, blt, flt;
    float left, top, right, bottom;

    auto abs_origin = ent->abs_origin ( );

    auto min = ent->mins ( ) + abs_origin;
    auto max = ent->maxs ( ) + abs_origin;

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

    /* state the box bounds. */
    box.x = static_cast< int >( left );
    box.y = static_cast< int >( top );
    box.w = static_cast< int >( right - left );
    box.h = static_cast< int >( bottom - top );
}
