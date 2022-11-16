#pragma once

class entity_t;

class weapon_info_t {
public:
	PAD ( 20 ); //0x0000
	int m_max_clip; //0x0014
	int m_max_clip_alt; //0x0018
	int m_default_clip; //0x001C
	int m_default_clip_alt; //0x0020
	PAD ( 100 ); //0x0024
	char *m_hud_name; //0x0088
	char *m_weapon_name; //0x008C
	PAD ( 56 ); //0x0090
	int m_weapon_type; //0x00C8
	PAD ( 12 ); //0x00CC
	char *m_anim_prefix; //0x00D8
	float m_cycle_time; //0x00DC
	float m_cycle_time_alt; //0x00E0
	PAD ( 8 ); //0x00E4
	int m_slot; //0x00EC
	int m_damage; //0x00F0
	PAD ( 8 ); //0x00F4
	int m_bullets; //0x00FC
	float m_penetration; //0x0100
	PAD ( 8 );  //0x0104
	float m_range; //0x010C
	float m_range_modifier; //0x0110
	float m_throw_velocity; //0x0114
	PAD ( 28 );  //0x0118
	float m_max_speed; //0x0134
	float m_max_speed_alt; //0x0138
	PAD ( 100 );  //0x013C
}; //Size: 0x01A0

class weapon_t : public entity_t {
public:
	NETVAR ( short, item_definition_index, "DT_BaseAttributableItem", "m_iItemDefinitionIndex" );
	weapon_info_t *data ( );
};