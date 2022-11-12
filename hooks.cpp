#include "includes.h"

hooks_t hooks { };

std::unique_ptr < c_event_handler > event_handler = nullptr;

bool hooks_t::init ( ) {
	m_hwnd = LI_FN ( FindWindowA )( x_ ( "Valve001" ), nullptr );
	
	/* set wndproc hook. */
	m_old_wndproc = reinterpret_cast < WNDPROC > ( SetWindowLongPtrA ( m_hwnd, GWLP_WNDPROC, LONG_PTR ( wnd_proc ) ) );	

	if ( MH_Initialize ( ) != MH_OK )
		throw std::runtime_error ( x_ ( "failed to initialize minhook." ) );

	/* addresses. */
	const auto _create_move = util::get_method < void * > ( interfaces.m_client, 22 );
	const auto _frame_stage_notify = util::get_method < void * > ( interfaces.m_client, 37 );
	const auto _paint = pattern::find ( x_ ( "engine.dll" ), x_ ( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89" ) ).as< void * > ( );
	const auto _paint_traverse = util::get_method < void * > ( interfaces.m_panel, 41 );
	const auto _run_command = util::get_method < void * > ( interfaces.m_prediction, 19 );

	/* create detours. */
	m_create_move_proxy.create ( _create_move, create_move_proxy );
	m_frame_stage_notify.create ( _frame_stage_notify, frame_stage_notify );
	m_paint.create ( _paint, paint );
	m_paint_traverse.create ( _paint_traverse, paint_traverse );
	m_run_command.create ( _run_command, run_command );

	/* create event handler. */
	event_handler = std::make_unique < c_event_handler > ( );

	MH_EnableHook ( MH_ALL_HOOKS );

	return true;
}

void __stdcall hooks_t::create_move ( int seq_num, float input_sample_frame_time, bool is_active, bool &send_packet ) {
	hooks.m_create_move_proxy.get_old_method< decltype ( &create_move_proxy ) > ( )( interfaces.m_client, 0, seq_num, input_sample_frame_time, is_active );

	const auto cmd = interfaces.m_input->get_cmd ( seq_num );
	const auto verified = interfaces.m_input->get_verified_cmd ( seq_num );

	if ( ( !cmd || !cmd->m_command_number ) || !verified || !is_active )
		return;

	g.m_ucmd = cmd;

	prediction.predict ( cmd );
	prediction.restore ( cmd );
}

void __fastcall hooks_t::frame_stage_notify ( void *ecx, void *edx, client_frame_stage_t stage ) {
	if ( stage != frame_start )
		g.m_stage = stage;

	/* store local player. */
	g.m_local = ( interfaces.m_engine->is_connected ( ) || interfaces.m_engine->is_in_game ( ) ) ? interfaces.m_entlist->get< player_t * > ( interfaces.m_engine->get_local_player ( ) ) : nullptr;

	/* call og. */
	hooks.m_frame_stage_notify.get_old_method< decltype ( &frame_stage_notify ) > ( )( ecx, edx, stage );
}

void __fastcall hooks_t::run_command ( void *ecx, void *edx, player_t *player, ucmd_t *ucmd, c_move_helper *move_helper ) {
	hooks.m_run_command.get_old_method < decltype ( &run_command ) > ( )( ecx, edx, player, ucmd, move_helper );

	/* set move helper pointer. */
	interfaces.m_move_helper = move_helper;
}

void __fastcall hooks_t::paint_traverse ( void *ecx, void *edx, unsigned int panel, bool force_repaint, bool allow_force ) {
	const auto panel_hash = fnv1a::hash_const ( interfaces.m_panel->get_name ( panel ) );

	hooks.m_paint_traverse.get_old_method< decltype ( &paint_traverse ) > ( )( ecx, edx, panel, force_repaint, allow_force );

	if ( panel_hash == HASH ( "FocusOverlayPanel" ) ) {
		/* update shit before you draw. */
	}
}

void __fastcall hooks_t::paint ( void *ecx, void *edx, paint_modes_t mode ) {
	const auto engine_vgui = reinterpret_cast< c_engine_vgui * >( ecx );

	hooks.m_paint.get_old_method< decltype ( &paint ) > ( )( ecx, edx, mode );

	if ( engine_vgui->m_static_transition_panel && ( mode & paint_modes_t::paint_uipanels ) ) {
		interfaces.m_surface->paint ( [ & ] {
			visuals.paint ( );
			logs.paint ( );
		} );
	}
}

LRESULT __stdcall hooks_t::wnd_proc ( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param ) {
	return CallWindowProcA ( hooks.m_old_wndproc, hwnd, msg, w_param, l_param );
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

void hooks_t::restore ( ) {
	/* restore window procedure. */
	SetWindowLongPtrA ( m_hwnd, GWLP_WNDPROC, LONG_PTR ( m_old_wndproc ) );

	/* restore hooks. */
	MH_DisableHook ( MH_ALL_HOOKS );
	MH_RemoveHook ( MH_ALL_HOOKS );
	MH_Uninitialize ( );
}