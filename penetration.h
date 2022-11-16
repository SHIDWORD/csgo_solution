#pragma once

struct fire_data_t {
    float m_penetrate_count;
    float m_trace_length;
    float m_remaining_length;
    int m_hitbox;
    int m_hitgroup;
    float m_damage;
    float m_out_damage;
};

class penetration_t {
public:
    fire_data_t run ( const vec_t src, const vec_t end, player_t *ent );
    bool fire_bullet ( const weapon_info_t *data, vec_t src, vec_t pos, trace_filter_t *filter, fire_data_t &fire_info, player_t *ent = nullptr, bool point = false );
    bool is_breakable_ent ( player_t *ent );
    bool trace_to_exit ( vec_t start, vec_t dir, vec_t &end, trace_t &trace_enter, trace_t &trace_exit, float step_size, float max_distance );
    bool handle_bullet_penetration ( player_t *ent, float &penetration, int &enter_material, bool &hit_grate, trace_t &tr, vec_t &direction, surfacedata_t *surface_data, float penetration_modifier, float damage_modifier, float penetration_power, int &penetration_count, vec_t &src, float distance, float current_distance, float &current_damage );
    float scale_dmg ( int hitgroup, player_t *ent, float armor_ratio, float damage );
    void clip_trace_to_players ( const vec_t &vec_abs_start, const vec_t &vec_abs_end, unsigned int mask, trace_filter_t *filter, trace_t *tr );
};

extern penetration_t penetration;