#include "includes.h"

notify_t notify { };

void notify_t::push_log ( const char *msg, color_t col ) {
	interfaces.m_cvar->console_color_printf ( { 202, 153, 255 }, x_ ( "[ hack ] " ) );
	interfaces.m_cvar->console_color_printf ( col, msg );

	m_notify_text.push_front ( { msg, col, interfaces.m_globals->m_curtime, 255 } );
}

#define SPEED_FREQ ( 255 / 0.5f )

void notify_t::paint ( ) {
	if ( m_notify_text.empty ( ) )
		return;

	for ( int i { 0 }; i < m_notify_text.size ( ); i++ ) {
		auto &notify = m_notify_text [ i ];

		if ( interfaces.m_globals->m_curtime - notify.m_time > 5.f )
			notify.m_alpha -= SPEED_FREQ * interfaces.m_globals->m_frametime;

		if ( ( notify.m_alpha < 0.f && ( interfaces.m_globals->m_curtime - notify.m_time ) > 5.f ) || m_notify_text.size ( ) > 11 ) {
			m_notify_text.pop_back ( );
			continue;
		}

		render.string ( fonts [ fonts_t::log_font ].m_data, 5, 6 + i * fonts [ fonts_t::log_font ].m_height, { notify.m_color, notify.m_alpha }, notify.m_message );
	}
}