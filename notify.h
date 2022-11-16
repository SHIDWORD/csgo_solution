#pragma once

struct notify_text_t {
	const char *m_message;
	color_t m_color;
	float m_time;
	int m_alpha;
};

struct notify_t {
	void push_log ( const char *msg, color_t col );
	void paint ( );

	std::deque < notify_text_t > m_notify_text;
};

extern notify_t notify;