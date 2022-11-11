#pragma once

struct inputpred_t {
	void predict ( ucmd_t *ucmd );
	void restore ( ucmd_t *ucmd );

	uintptr_t m_prediction_player;
	uintptr_t m_prediction_seed;
};

extern inputpred_t prediction;