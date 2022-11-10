#pragma once

struct log_t {
	const char* m_message;
	color_t m_color;
	float m_time;
	int m_alpha;
};

struct logs_t {
	void push_log ( const char *msg, color_t col );
	void paint ( );

	std::deque < log_t > m_logs;
};

extern logs_t logs;