#include "includes.h"

inputpred_t prediction { };

void post_think ( player_t* ent ) {
	interfaces.m_mdl_cache->begin_lock ( );

	static auto post_think_vphysics = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ).as< bool ( __thiscall * )( player_t * ) > ( );
	static auto simulate_player_simulated_entities = pattern::find ( x_ ( "client.dll" ), x_ ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) ).as< void ( __thiscall * )( void * ) > ( );

	if ( ent->alive ( ) ) {
		util::get_method< void ( __thiscall * )( void * ) > ( ent, 340 ) ( ent );

		if ( ent->flags ( ) & flags_t::on_ground )
			ent->fall_vel ( ) = 0.f;

		if ( ent->sequence ( ) == -1 )
			ent->set_sequence ( 0 );

		util::get_method< void ( __thiscall * )( void * ) > ( ent, 220 ) ( ent );
		post_think_vphysics ( ent );
	}

	simulate_player_simulated_entities ( ent );
	interfaces.m_mdl_cache->end_lock ( );
}

void inputpred_t::predict ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd || !interfaces.m_move_helper )
		return;

	g.m_local->current_cmd ( ) = ucmd;
	g.m_local->last_cmd ( ) = *ucmd;

	if ( !m_prediction_player || !m_prediction_seed ) {
		m_prediction_seed = pattern::find ( x_ ( "client.dll" ), x_ ( "8B 47 40 A3" ) ).add ( 4 ).deref ( ).as< uintptr_t > ( );
		m_prediction_player = pattern::find ( x_ ( "client.dll" ), x_ ( "0F 5B C0 89 35" ) ).add ( 5 ).deref ( ).as< uintptr_t > ( );
	}

	*reinterpret_cast< int * >( m_prediction_seed ) = ucmd ? ucmd->m_random_seed : -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = g.m_local;

	m_curtime = interfaces.m_globals->m_curtime;
	m_frametime = interfaces.m_globals->m_frametime;

	interfaces.m_globals->m_curtime = g.m_local->tick_base ( ) * interfaces.m_globals->m_interval_per_tick;
	interfaces.m_globals->m_frametime = interfaces.m_prediction->m_engine_paused ? 0 : interfaces.m_globals->m_interval_per_tick;

	m_is_first_time_predicted = interfaces.m_prediction->m_is_first_time_predicted;
	m_in_prediction = interfaces.m_prediction->m_in_prediction;

	interfaces.m_prediction->m_is_first_time_predicted = false;
	interfaces.m_prediction->m_in_prediction = true;

	ucmd->m_buttons |= g.m_local->button_forced ( );
	ucmd->m_buttons &= ~g.m_local->button_disabled ( );

	interfaces.m_move_helper->set_host ( g.m_local );
	interfaces.m_game_movement->start_track_prediction_errors ( g.m_local );

	if ( ucmd->m_weaponselect != 0 ) {
		weapon_t *weapon = g.m_local->weapon ( );

		if ( weapon ) {
			//auto data = interfaces.m_weapon_system->weapon_data ( weapon->item_definition_index ( ) );

			//if ( data )
				//g.m_local->select_item ( data->m_weapon_name, ucmd->m_weaponsubtype );
		}

		//if ( weapon )
			//g.m_local->select_item ( weapon->GetName ( ), ucmd->m_weaponsubtype );
	}

	const auto buttons_changed = ucmd->m_buttons ^ g.m_local->button_last ( );

	g.m_local->buttons ( ) = g.m_local->button_last ( );
	g.m_local->button_last ( ) = ucmd->m_buttons;
	g.m_local->button_pressed ( ) = ucmd->m_buttons & buttons_changed;
	g.m_local->button_released ( ) = buttons_changed & ~ucmd->m_buttons;

	interfaces.m_prediction->check_moving_ground ( g.m_local, interfaces.m_globals->m_frametime );
	interfaces.m_prediction->set_local_view_angles ( ucmd->m_ang );

	if ( g.m_local->physics_run_think ( 0 ) )
		g.m_local->pre_think ( );

	const auto think_tick = g.m_local->think_tick ( );

	if ( think_tick > 0 && think_tick <= g.m_local->tick_base ( ) ) {
		g.m_local->think_tick ( ) = -1;
		static auto set_next_think = pattern::find ( x_ ( "client.dll" ), x_ ( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6 C1 E8 16 24 01 74 18" ) ).as< void ( __thiscall * )( void *, int ) > ( );
		set_next_think ( g.m_local, 0 );
		g.m_local->think ( );
	}

	interfaces.m_prediction->setup_move ( g.m_local, ucmd, interfaces.m_move_helper, &data );
	interfaces.m_game_movement->process_movement ( g.m_local, &data );

	interfaces.m_prediction->finish_move ( g.m_local, ucmd, &data );
	interfaces.m_move_helper->process_impacts ( );

	post_think ( g.m_local );

	interfaces.m_prediction->m_is_first_time_predicted = m_is_first_time_predicted;
	interfaces.m_prediction->m_in_prediction = m_in_prediction;
}

void inputpred_t::restore ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd || !interfaces.m_move_helper )
		return;

	interfaces.m_game_movement->finish_track_prediction_errors ( g.m_local );
	interfaces.m_move_helper->set_host ( nullptr );

	g.m_local->current_cmd ( ) = nullptr;

	*reinterpret_cast< int * >( m_prediction_seed ) = -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = nullptr;

	interfaces.m_game_movement->reset ( );

	interfaces.m_globals->m_curtime = m_curtime;
	interfaces.m_globals->m_frametime = m_frametime;
}