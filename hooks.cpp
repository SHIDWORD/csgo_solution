#include "includes.h"

hooks_t hooks { };

bool hooks_t::init ( ) {
	if ( MH_Initialize ( ) != MH_OK )
		throw std::runtime_error ( XOR ( "failed to initialize minhook." ) );

	const auto _enginevgui_paint = pattern::find ( XOR ( "engine.dll" ), XOR ( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89" ) ).as< void * > ( );
	const auto _chlclient_create_move = vf < void * > ( interfaces.m_client, 22 );
	const auto _chlclient_frame_stage_notify = vf < void * > ( interfaces.m_client, 37 );
	
	/* create detours. */ {
		m_create_move_proxy.create ( _chlclient_create_move, create_move_proxy );
		m_frame_stage_notify.create ( _chlclient_frame_stage_notify, frame_stage_notify );
		m_paint.create ( _enginevgui_paint, paint );
	}

	MH_EnableHook ( MH_ALL_HOOKS );

	return true;
}

void __fastcall hooks_t::frame_stage_notify ( void *ecx, void *edx, client_frame_stage_t stage ) {
	if ( stage != frame_start )
		g.m_stage = stage;

	/* store local player. */
	g.m_local = ( interfaces.m_engine->is_connected ( ) || interfaces.m_engine->is_in_game ( ) ) ? interfaces.m_entlist->get< entity_t * > ( interfaces.m_engine->get_local_player ( ) ) : nullptr;

	/* call og. */
	hooks.m_frame_stage_notify.get_old_method< decltype ( &frame_stage_notify ) > ( )( ecx, edx, stage );
}

void __fastcall hooks_t::paint ( void *ecx, void *edx, paint_modes_t mode ) {
	const auto engine_vgui = reinterpret_cast< c_engine_vgui * >( ecx );
	
	/* call og. */
	hooks.m_paint.get_old_method< decltype ( &paint ) > ( )( ecx, edx, mode );

	if ( engine_vgui->m_static_transition_panel && ( mode & paint_modes_t::paint_uipanels ) ) {
		interfaces.m_surface->paint ( [ & ] {
			/* render data. */
			render.string ( fonts [ fonts_t::hack_watermark ].m_data, 10, 10, { 255, 255, 255, 255 }, XOR ( "CSGO Base" ) );
		} );
	}
}

void __stdcall hooks_t::create_move ( int seq_num, float input_sample_frame_time, bool is_active, bool &send_packet ) {
	hooks.m_create_move_proxy.get_old_method< decltype ( &create_move_proxy ) > ( )( interfaces.m_client, 0, seq_num, input_sample_frame_time, is_active );

	const auto cmd = interfaces.m_input->get_cmd ( seq_num );
	const auto verified = interfaces.m_input->get_verified_cmd ( seq_num );

	if ( ( !cmd || !cmd->m_command_number ) || !verified || !is_active )
		return;

	g.m_ucmd = cmd;
}

__declspec( naked ) void __fastcall hooks_t::create_move_proxy ( void *ecx, void *edx, int seq_num, float input_sample_frame_time, bool is_active ) {
	__asm {
		push ebp
		mov	ebp, esp;
		push ebx;
		push esp;
		push dword ptr [ is_active ];
		push dword ptr [ input_sample_frame_time ];
		push dword ptr [ seq_num ];
		call create_move
		pop	ebx
		pop	ebp
		retn 0Ch
	}
}