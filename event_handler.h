#pragma once

class c_event_handler : public c_event_listener {
public:
	virtual void fire_game_event ( event_t *event );
	
	void init ( );
	void remove ( );
	int get_event_debug_id ( );
};

extern c_event_handler event_handler;