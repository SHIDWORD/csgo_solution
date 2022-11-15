#pragma once

struct weapon_info_t;

class c_weapon_system {
	enum indices_t : std::uintptr_t {
		_get_weapon_data = 2,
	};
public:
	weapon_info_t *weapon_data ( short idx ) {
		using weapon_data_fn = weapon_info_t * ( __thiscall * ) ( void *, short );
		return util::get_method< weapon_data_fn > ( this, indices_t::_get_weapon_data )( this, idx );
	}
};