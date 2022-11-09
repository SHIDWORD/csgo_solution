#pragma once

struct box_t {
	int x, y, w, h;
};

struct visuals_t {
	bool get_box_bounds ( player_t *ent, box_t &box );
	void paint ( );
};

extern visuals_t visuals;