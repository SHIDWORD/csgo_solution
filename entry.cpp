#include "includes.h"

void __stdcall cheat_thread ( ) {
	/* wait for serverbrowser.dll to finish loading. */
	while ( !LI_FN ( GetModuleHandleA )( x_ ( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 400 ) );

	/* init our hack. */
	interfaces.init ( );
	netvars.init ( );
	render.init ( );
	hooks.init ( );

	/* unload our hack. */
	while ( !LI_FN ( GetAsyncKeyState )( VK_END ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 200 ) );

	hooks.restore ( );
	LI_FN ( FreeLibraryAndExitThread )( g.m_module, 0 );
}

int __stdcall DllMain ( HMODULE mod, unsigned long reason, void *reserved ) {
	g.m_module = mod;

	if ( reason == DLL_PROCESS_ATTACH ) {
		const auto handle = CreateThread ( nullptr, 0, LPTHREAD_START_ROUTINE ( cheat_thread ), mod, 0, nullptr );

		if ( !handle )
			return 0;

		CloseHandle ( handle );
	}

	return 1;
}