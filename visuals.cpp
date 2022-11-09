#include "includes.h"

visuals_t visuals { };

void visuals_t::paint ( ) {
	for ( int i = 1; i <= interfaces.m_globals->m_max_clients; i++ ) {
		auto pl = interfaces.m_entlist->get< player_t* > ( i );

		if ( !pl || pl->dormant ( ) || !pl->is_alive ( ) || pl->team ( ) == g.m_local->team ( ) )
			continue;

		vec_t origin, screen_origin;
		origin = pl->abs_origin ( );

		if ( !render.world_to_screen ( origin, screen_origin ) )
			continue;

		render.filled_rect ( screen_origin.x - 2, screen_origin.y - 2, 4, 4, { 255, 255, 255 } );
	}
}