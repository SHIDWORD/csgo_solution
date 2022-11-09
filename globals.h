#pragma once

struct globals_t {
	client_frame_stage_t m_stage;
	HMODULE m_module;
	player_t *m_local = nullptr;
	ucmd_t *m_ucmd= nullptr;
}; inline globals_t g { };