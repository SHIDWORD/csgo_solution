#pragma once

struct inputpred_t {
	void predict ( ucmd_t *ucmd, const std::function<void ( )> &fn );
	void begin ( ucmd_t *ucmd );
	void restore ( ucmd_t *ucmd );

	uintptr_t m_prediction_player;
	uintptr_t m_prediction_seed;
	bool m_in_prediction;
	bool m_is_first_time_predicted;
	float m_curtime;
	float m_frametime;
	c_move_data data;
};

extern inputpred_t prediction;