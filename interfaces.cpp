#include "includes.h"

interfaces_t interfaces { };

template < typename t >
__forceinline t create_interface ( const char *module_name, const char *name ) {
	using create_interface_fn = std::uintptr_t *( __cdecl * ) ( const char *, int * );
	const auto create_interface_call = reinterpret_cast< create_interface_fn >( LI_FN ( GetProcAddress )( LI_FN ( GetModuleHandleA )( module_name ), x_ ( "CreateInterface" ) ) );

	return reinterpret_cast< t >( create_interface_call ( name, nullptr ) );
}

bool interfaces_t::init ( ) {
	/* create interfaces. */
	m_engine  = create_interface< c_engine * > ( x_ ( "engine.dll" ), x_ ( "VEngineClient014" ) );
	m_surface = create_interface< c_surface * > ( x_ ( "vguimatsurface.dll" ), x_ ( "VGUI_Surface031" ) );
	m_client  = create_interface< c_client * > ( x_ ( "client.dll" ), x_ ( "VClient018" ) );
	m_entlist = create_interface< c_entlist * > ( x_ ( "client.dll" ), x_ ( "VClientEntityList003" ) );

	/* get addresses. */
	m_input   = pattern::find ( x_ ( "client.dll" ), x_ ( "B9 ? ? ? ? FF 60 60" ) ).add ( 1 ).deref ( ).as< c_input * > ( );
	m_globals = pattern::find ( x_ ( "client.dll" ), x_ ( "A1 ? ? ? ? FF 70 04 68 ? ? ? ? 56 E8 ? ? ? ? 8B 06" ) ).add ( 1 ).deref ( ).deref ( ).as< c_global_vars * > ( );

	return true;
}