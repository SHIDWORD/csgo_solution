#include "includes.h"

logs_t logs { };

void logs_t::push_log ( const char *msg, color_t col ) {
	interfaces.m_cvar->console_color_printf ( { 255, 0, 0 }, x_ ( "[ hypnotic ] " ) );
	interfaces.m_cvar->console_color_printf ( col, msg );

	m_logs.push_front ( { msg, col, interfaces.m_globals->m_curtime, 255 } );
}

void logs_t::paint ( ) {
	if ( m_logs.empty ( ) )
		return;

	for ( int i { 0 }; i < m_logs.size ( ); i++ ) {
		auto &log = m_logs [ i ];

		if ( interfaces.m_globals->m_curtime - log.m_time > 5.f )
			log.m_alpha -= ( 255 / 0.5f ) * interfaces.m_globals->m_frametime;

		if ( log.m_alpha < 0.f && ( interfaces.m_globals->m_curtime - log.m_time > 5.f ) || m_logs.size ( ) > 9 ) {
			m_logs.pop_back ( );
			continue;
		}

		render.string ( fonts [ fonts_t::log_font ].m_data, 5, 6 + i * fonts [ fonts_t::log_font ].m_height, { log.m_color, log.m_alpha }, log.m_message );
	}
}