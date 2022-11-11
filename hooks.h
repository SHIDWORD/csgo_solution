#pragma once

struct detour_t {
	void *m_original { };

	__forceinline bool create ( void *target, void *detour ) {
		/* TODO: debugging features. */
		if ( MH_CreateHook ( target, detour, &m_original ) != MH_OK )
			return false;

		return true;
	}

	template < typename t >
	__forceinline t get_old_method ( ) {
		return reinterpret_cast < t > ( m_original );
	}
};

struct hooks_t {
	bool init ( );
	void restore ( );

	/* hooks. */
	static void __stdcall create_move ( int seq_num, float input_sample_frame_time, bool is_active, bool &send_packet );
	static void __fastcall create_move_proxy ( void *ecx, void *edx, int seq_num, float input_sample_frame_time, bool is_active );
	static void __fastcall frame_stage_notify ( void *ecx, void *edx, client_frame_stage_t stage );
	static void __fastcall paint ( void *ecx, void *edx, paint_modes_t mode );
	static LRESULT __stdcall wnd_proc ( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	static void __fastcall paint_traverse ( void *ecx, void *edx, unsigned int panel, bool force_repaint, bool allow_force );
	static void __fastcall run_command ( player_t *player, ucmd_t *ucmd, c_move_helper *move_helper );;
	
	/* detours. */
	detour_t m_create_move_proxy;
	detour_t m_frame_stage_notify;
	detour_t m_paint;
	detour_t m_paint_traverse;
	detour_t m_run_command;

	/* other. */
	HWND m_hwnd;
	WNDPROC m_old_wndproc;
};

extern hooks_t hooks;