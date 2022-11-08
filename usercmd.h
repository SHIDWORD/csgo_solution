#pragma once

class ucmd_t {
public:
    virtual       ~ucmd_t ( ) { };
    int           m_command_number;
    int           m_tick_count;
    vec_t         m_ang;
    vec_t         m_aimdir;
    float         m_forwardmove;
    float         m_sidemove;
    float         m_upmove;
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
};

class verified_ucmd_t {
public:
	ucmd_t        m_cmd;
	unsigned long m_crc;
};