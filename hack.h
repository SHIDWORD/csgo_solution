#pragma once

struct hack_t {
	void on_paint ( );
	void on_tick ( ucmd_t *ucmd, bool &send_packet );
};

extern hack_t hack;