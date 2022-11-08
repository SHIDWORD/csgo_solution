#include "includes.h"

interfaces_t interfaces { };

template < typename t > 
__forceinline t create_interface ( const char* module_name, const char* name ) {
	using create_interface_fn = std::uintptr_t * ( __cdecl* ) ( const char*, int* );
	const auto create_interface_call = reinterpret_cast< create_interface_fn >( LI_FN ( GetProcAddress )( LI_FN ( GetModuleHandleA )( module_name ), XOR ( "CreateInterface" ) ) );
	
	return reinterpret_cast< t >( create_interface_call ( name, nullptr ) );
}

bool interfaces_t::init ( ) {
	/* create interfaces. */
	m_engine  = create_interface< c_engine * > ( XOR ( "engine.dll" ), XOR ( "VEngineClient014" ) );
	m_surface = create_interface < c_surface * > ( XOR ( "vguimatsurface.dll" ), XOR ( "VGUI_Surface031" ) );
	m_client  = create_interface< c_client * > ( XOR ( "client.dll" ), XOR ( "VClient018" ) );
	m_entlist = create_interface < c_entlist * > ( XOR ( "client.dll" ), XOR ( "VClientEntityList003" ) );

	/* get addresses. */
	m_input = pattern::find ( XOR ( "client.dll" ), XOR ( "B9 ? ? ? ? FF 60 60" ) ).add ( 1 ).deref ( ).as< c_input * > ( );

	return true;
}