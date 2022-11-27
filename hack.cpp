#include "includes.h"

hack_t hack { };

void hack_t::on_paint ( ) {
	if ( !interfaces::m_engine->is_in_game ( ) || !interfaces::m_engine->is_connected ( ) )
		return;

	visuals.paint ( );
	notify.paint ( );
}

void hack_t::on_tick ( ucmd_t* ucmd, bool& send_packet ) {
	if ( !interfaces::m_engine->is_in_game ( ) || !interfaces::m_engine->is_connected ( ) )
		return;
}