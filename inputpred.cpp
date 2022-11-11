#include "includes.h"

inputpred_t prediction { };

void inputpred_t::predict ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd )
		return;

	if ( !m_prediction_player || !m_prediction_seed ) {
	}

	g.m_local->current_cmd ( ) = ucmd;
	g.m_local->last_cmd ( ) = ucmd;

	*reinterpret_cast< int * >( m_prediction_seed ) = ucmd ? ucmd->m_random_seed : -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = g.m_local;

	interfaces.m_globals->m_curtime = g.m_local->tick_base ( ) * interfaces.m_globals->m_interval_per_tick;
	interfaces.m_globals->m_frametime = interfaces.m_prediction->m_engine_paused ? 0 : interfaces.m_globals->m_interval_per_tick;

	ucmd->m_buttons |= g.m_local->button_forced ( );
	ucmd->m_buttons &= ~g.m_local->button_disabled ( );
	
	interfaces.m_game_movement->start_track_prediction_errors ( g.m_local );

	if ( ucmd->m_weaponselect != 0 ) {
		weapon_t *weapon = interfaces.m_entlist->get_by_handle < weapon_t * > ( g.m_local->weapon_handle ( ) );

		if ( weapon )
			g.m_local->select_item ( weapon->GetName ( ), ucmd->m_weaponsubtype );
	}

	const auto buttons_changed = ucmd->m_buttons ^ g.m_local->button_last ( );

	g.m_local->buttons ( ) = g.m_local->button_last ( );
	g.m_local->button_last ( ) = ucmd->m_buttons;
	g.m_local->button_pressed ( ) = ucmd->m_buttons & buttons_changed;
	g.m_local->button_released ( ) = buttons_changed & ~ucmd->m_buttons;

	interfaces.m_prediction->check_moving_ground ( g.m_local, interfaces.m_globals->m_frametime );
	interfaces.m_prediction->set_local_view_angles ( ucmd->m_ang );

	if ( g.m_local->physics_run_think ( 0 ) )
		( *( g.m_local->m_vftable + 1272 ) )( player );

	const auto think_tick = g.m_local->think_tick ( );

	if ( think_tick > 0 && think_tick <= g.m_local->tick_base ( ) ) {
		g.m_local->think_tick ( ) = -1;
		static auto set_next_think = pattern::find ( x_ ( "clig.m_local.dll" ), x_ ( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6 C1 E8 16 24 01 74 18" ) ).as< void ( __thiscall * )( void *, int ) > ( );
		set_next_think ( g.m_local, 0 );
		g.m_local->think ( );
	}

	interfaces.m_prediction->setup_move ( g.m_local, ucmd, interfaces.m_move_helper, move_data );
	interfaces.m_game_movement->process_movement ( g.m_local, move_data );
	interfaces.m_prediction->finish_move ( g.m_local, ucmd, move_data );
	interfaces.m_move_helper->process_impacts ( );
	
	g.m_local->post_think ( );
	interfaces.m_game_movement->finish_track_prediction_errors ( g.m_local );
}

void inputpred_t::restore ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd )
		return;

	g.m_local->current_cmd ( ) = 0;

	*reinterpret_cast< int * >( m_prediction_seed ) = -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = 0;

	if ( !interfaces.m_prediction->m_engine_paused && interfaces.m_globals->m_frametime > 0.0f )
			++g.m_local->tick_base ( );
}