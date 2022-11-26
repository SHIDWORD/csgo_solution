#pragma once

class c_engine_trace;

struct interfaces_t {
	bool init ( );

	c_engine *m_engine = nullptr;
	c_client *m_client = nullptr;
	c_input *m_input = nullptr;
	c_entlist *m_entlist = nullptr;
	c_surface *m_surface = nullptr;
	c_convar *m_cvar = nullptr;
	c_global_vars *m_globals = nullptr;
	c_localize *m_localize = nullptr;
	c_model_cache *m_mdl_cache = nullptr;
	c_panel *m_panel = nullptr;
	c_game_event_mgr *m_event_mgr = nullptr;
	c_game_type *m_game_type = nullptr;
	c_debug_overlay *m_debug_overlay = nullptr;
	c_move_helper *m_move_helper = nullptr;
	c_prediction *m_prediction = nullptr;
	c_game_movement *m_game_movement = nullptr;
	c_phys *m_phys_props = nullptr;
	c_engine_trace *m_trace = nullptr;
};

extern interfaces_t interfaces;