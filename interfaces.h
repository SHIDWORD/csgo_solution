#pragma once

struct interfaces_t {
	bool init ( );

	/* interface list. */
	c_engine *m_engine = nullptr;
	c_client *m_client = nullptr;
	c_input *m_input = nullptr;
	c_entlist *m_entlist = nullptr;
	c_surface *m_surface = nullptr;
	c_convar *m_cvar = nullptr;
	c_global_vars *m_globals = nullptr;
	c_localize *m_localize = nullptr;
};

extern interfaces_t interfaces;