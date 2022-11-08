#pragma once

struct interfaces_t {	
	bool init ( );

	/* interface list. */
	c_engine *m_engine;
	c_client *m_client;
	c_input *m_input;
	c_entlist *m_entlist;
	c_surface *m_surface;
};

extern interfaces_t interfaces;