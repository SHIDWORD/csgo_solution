#pragma once

class ucmd_t {
public:
	virtual       ~ucmd_t ( ) { };
	int           m_command_number;
	int           m_tick_count;
	vec_t         m_ang;
	vec_t         m_aimdir;
	float         m_fmove;
	float         m_smove;
	float         m_umove;
	int           m_buttons;
	unsigned char m_impulse;
	int           m_weaponselect;
	int           m_weaponsubtype;
	int           m_random_seed;
	short         m_mousedx;
	short         m_mousedy;
	bool          m_hasbeenpredicted;
	vec_t		  m_head_angles;
	vec_t		  m_head_offset;

	uint32_t get_checksum ( ) {
		uint32_t crc;
		crc32::init ( crc );

		crc32::process ( crc, m_command_number );
		crc32::process ( crc, m_tick_count );
		crc32::process ( crc, m_ang );
		crc32::process ( crc, m_aimdir );
		crc32::process ( crc, m_fmove );
		crc32::process ( crc, m_smove );
		crc32::process ( crc, m_umove );
		crc32::process ( crc, m_buttons );
		crc32::process ( crc, m_impulse );
		crc32::process ( crc, m_weaponselect );
		crc32::process ( crc, m_weaponsubtype );
		crc32::process ( crc, m_random_seed );
		crc32::process ( crc, m_mousedx );
		crc32::process ( crc, m_mousedy );

		crc32::final ( crc );

		return crc;
	}
};

class verified_ucmd_t {
public:
	ucmd_t        m_cmd;
	unsigned long m_crc;
};