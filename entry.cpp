#include "includes.h"

void __stdcall cheat_thread ( ) {
	/* wait for serverbrowser.dll to finish loading. */
	while ( !LI_FN ( GetModuleHandleA )( XOR ( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 100 ) );

	/* init our hack. */
	interfaces.init ( );
	render.init ( );
	hooks.init ( );
}

int __stdcall DllMain ( HMODULE mod, unsigned long reason, void* reserved ) {
	g.m_module = mod;

	if ( reason == DLL_PROCESS_ATTACH ) {
		const auto handle = CreateThread ( nullptr, 0, LPTHREAD_START_ROUTINE ( cheat_thread ), mod, 0, nullptr );

		if ( !handle )
			return 0;

		CloseHandle ( handle );
	}
	
	return 1;
}