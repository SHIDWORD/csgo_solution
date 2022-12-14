#include "includes.h"

template < typename t >
__forceinline t create_interface ( const char *module_name, const char *name ) {
	using create_interface_fn = std::uintptr_t *( __cdecl * ) ( const char *, int * );
	const auto create_interface_call = reinterpret_cast< create_interface_fn >( LI_FN ( GetProcAddress )( LI_FN ( GetModuleHandleA )( module_name ), x_ ( "CreateInterface" ) ) );

	return reinterpret_cast< t >( create_interface_call ( name, nullptr ) );
}

bool interfaces::init ( ) {
	m_engine = create_interface< c_engine * > ( x_ ( "engine.dll" ), x_ ( "VEngineClient014" ) );
	m_surface = create_interface< c_surface * > ( x_ ( "vguimatsurface.dll" ), x_ ( "VGUI_Surface031" ) );
	m_localize = create_interface< c_localize * > ( x_ ( "localize.dll" ), x_ ( "Localize_001" ) );
	m_mdl_cache = create_interface< c_model_cache * > ( x_ ( "datacache.dll" ), x_ ( "MDLCache004" ) );
	m_prediction = create_interface< c_prediction * > ( x_ ( "client.dll" ), x_ ( "VClientPrediction001" ) );
	m_game_movement = create_interface< c_game_movement * > ( x_ ( "client.dll" ), x_ ( "GameMovement001" ) );
	m_client = create_interface< c_client * > ( x_ ( "client.dll" ), x_ ( "VClient018" ) );
	m_trace = create_interface< c_engine_trace * > ( x_ ( "engine.dll" ), x_ ( "EngineTraceClient004" ) );
	m_entlist = create_interface< c_entlist * > ( x_ ( "client.dll" ), x_ ( "VClientEntityList003" ) );
	m_cvar = create_interface< c_convar * > ( x_ ( "vstdlib.dll" ), x_ ( "VEngineCvar007" ) );
	m_event_mgr = create_interface< c_game_event_mgr * > ( x_ ( "engine.dll" ), x_ ( "GAMEEVENTSMANAGER002" ) );
	m_game_type = create_interface< c_game_type * > ( x_ ( "matchmaking.dll" ), x_ ( "VENGINE_GAMETYPES_VERSION002" ) );
	m_panel = create_interface< c_panel * > ( x_ ( "vgui2.dll" ), x_ ( "VGUI_Panel009" ) );
	m_phys_props = create_interface< c_phys * > ( x_ ( "vphysics.dll" ), x_ ( "VPhysicsSurfaceProps001" ) );
	m_input = pattern::find ( x_ ( "client.dll" ), x_ ( "B9 ? ? ? ? FF 60 60" ) ).add ( 1 ).deref ( ).as< c_input * > ( );
	m_globals = pattern::find ( x_ ( "client.dll" ), x_ ( "A1 ? ? ? ? FF 70 04 68 ? ? ? ? 56 E8 ? ? ? ? 8B 06" ) ).add ( 1 ).deref ( ).deref ( ).as< c_global_vars * > ( );
	m_debug_overlay = create_interface< c_debug_overlay * > ( x_ ( "engine.dll" ), x_ ( "VDebugOverlay004" ) );

	return true;
}