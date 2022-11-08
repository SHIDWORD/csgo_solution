#pragma once

struct globals_t {
	/* global variables. */
	HMODULE m_module;
	ucmd_t *m_ucmd = nullptr;
	entity_t *m_local = nullptr;
	client_frame_stage_t m_stage;
}; 

inline globals_t g { };