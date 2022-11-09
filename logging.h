#pragma once

struct logs_t {
	void push_log ( const std::string& msg, color_t col );
	void paint ( );
};

extern logs_t logs;