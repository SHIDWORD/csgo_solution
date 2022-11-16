#pragma once

class player_t;
class weapon_t;

struct animstate_t {
	const int *m_layer_order_preset;
	bool m_first_run_since_init;
	bool m_first_foot_plant_since_init;
	int	m_last_procedural_foot_plant_update_frame;
	float m_eye_position_smooth_lerp;
	float m_strafe_change_weight_smooth_fall_off;
	PAD ( 60 );
	int m_cached_model_idx;
	float m_step_height_left;
	float m_step_height_right;
	weapon_t *m_weapon_last_bone_setup;
	player_t *m_player;
	weapon_t *m_weapon;
	weapon_t *m_weapon_last;
	float m_last_update_time;
	int	m_last_update_frame;
	float m_last_update_increment;
	float m_eye_yaw;
	float m_eye_pitch;
	float m_abs_yaw;
	float m_abs_yaw_last;
	float m_move_yaw;
	float m_move_yaw_ideal;
	float m_move_yaw_current_to_ideal;
	float m_time_to_align_lower_body;
	float m_primary_cycle;
	float m_move_weight;
	float m_move_weight_smoothed;
	float m_duck_amount;
	float m_duck_additional;
	float m_recrouch_weight;
	vec_t m_origin;
	vec_t m_origin_last;
	vec_t m_vel;
	vec_t m_vel_normalized;
	vec_t m_vel_normalized_non_zero;
	float m_vel_length_2d;
	float m_vel_length_z;
	float m_run_speed_normalized;
	float m_walk_speed_normalized;
	float m_crouch_speed_normalized;
	float m_duration_moving;
	float m_duration_still;
	bool m_on_ground;
	bool m_landing;
	float m_jump_to_fall;
	float m_duration_in_air;
	float m_left_ground_height;
	float m_hit_ground_weight;
	float m_walk_to_run_transition;
	PAD ( 4 );
	float m_in_air_smooth_value;
	bool m_on_ladder;
	float m_ladder_weight;
	float m_ladder_speed;
	bool m_walk_to_run_transition_state;
	bool m_defuse_started;
	bool m_plant_anim_started;
	bool m_twitch_anim_started;
	bool m_adjust_started;
	PAD ( 14 );
	float m_next_twitch_time;
	float m_time_of_last_known_injury;
	float m_last_velocity_test_time;
	vec_t m_last_vel;
	vec_t m_target_accel;
	vec_t m_accel;
	float m_accel_weight;
	PAD ( 12 );
	float m_strafe_change_weight;
	float m_strafe_change_target_weight;
	float m_strafe_change_cycle;
	int	 m_strafe_sequence;
	bool m_strafe_changing;
	float m_duration_strafing;
	float m_foot_lerp;
	bool m_feet_crossed;
	bool m_player_is_accelerating;
	PAD ( 372 );
	float m_camera_smooth_height;
	bool m_smooth_height_valid;
	PAD ( 16 );
	float m_aim_yaw_min;
	float m_aim_yaw_max;
	int	m_state_version;
};

enum class life_states_t : uint8_t {
	life_alive = 0,
	life_dying,
	life_dead,
	life_reborn,
	life_discard
};

enum flags_t : int {
	on_ground = ( 1 << 0 ),
	ducking = ( 1 << 1 ),
	water_jump = ( 1 << 2 ),
	on_train = ( 1 << 3 ),
	in_rain = ( 1 << 4 ),
	frozen = ( 1 << 5 ),
	at_controls = ( 1 << 6 ),
	client = ( 1 << 7 ),
	fake_client = ( 1 << 8 ),
};

class entity_t {
public:
	__forceinline const vec_t &origin ( ) {
		using abs_origin_fn = const vec_t &( __thiscall * )( void * );
		return util::get_method < abs_origin_fn > ( this, 10 )( this );
	}

	__forceinline const vec_t &angles ( ) {
		using abs_angles_fn = const vec_t &( __thiscall * )( void * );
		return util::get_method < abs_angles_fn > ( this, 11 )( this );
	}

	OFFSET ( void *, renderable, 0x4 );
	OFFSET ( void *, networkable, 0x8 );
	OFFSET ( int, idx, 0x64 );
	OFFSET ( bool, dormant, 0xED );
	OFFSET ( float, fall_vel, 0xC09 );
	OFFSET ( int, think_tick, 0xFC );
	OFFSET ( ucmd_t *, current_cmd, 0x3348 );
	OFFSET ( ucmd_t , last_cmd, 0x3298 );
	OFFSET ( int, buttons, 0x31FC );
	NETVAR ( int, team, "DT_BaseEntity", "m_iTeamNum" );
	NETVAR ( int, effects, "DT_BaseEntity", "m_fEffects" );
};

class player_t : public entity_t {
public:
	__forceinline bool alive ( ) {
		return ( life_state ( ) == life_states_t::life_alive ) && health ( ) > 0;
	}

	OFFSET ( int, button_pressed, 0x3200 );
	OFFSET ( int, button_last, 0x3208 );
	OFFSET ( int, button_released, 0x3204 );
	OFFSET ( int, button_disabled, 0x3340 );
	OFFSET ( int, sequence, 0xA30 );
	OFFSET ( animstate_t *, animstate, 0x9960 );
	NETVAR ( int, health, "DT_BasePlayer", "m_iHealth" );
	NETVAR ( life_states_t, life_state, "DT_BasePlayer", "m_lifeState" );
	NETVAR ( int, tick_base, "DT_BasePlayer", "m_TickBase" );
	NETVAR ( uint32_t, handle, "DT_BasePlayer", "m_hGroundEntity" );
	NETVAR ( int, flags, "DT_BasePlayer", "m_fFlags" );
	NETVAR_ADD ( vec_t, mins, "DT_BaseEntity", "m_Collision", 0x8 );
	NETVAR_ADD ( vec_t, maxs, "DT_BaseEntity", "m_Collision", 0x14 );
	NETVAR ( uint32_t, weapon_handle, "DT_BaseCombatCharacter", "m_hActiveWeapon" );
	NETVAR ( int, armor, "DT_CSPlayer", "m_ArmorValue" );
	NETVAR ( bool, helmet, "DT_CSPlayer", "m_bHasHelmet" );
	NETVAR ( bool, heavy_armor, "DT_CSPlayer", "m_bHasHeavyArmor" );
	OFFSET ( bool, use_new_animstate, 0x9B14 );
	OFFSET ( int, button_forced, 0x3344 );

	void set_abs_origin ( const vec_t &abs_origin );
	void set_abs_angles ( const vec_t &abs_angles );
	int lookup_bone ( const char *name );
	void select_item ( const char *name, int sub_type );
	void modify_eye_position ( animstate_t *state, vec_t *input_eye_pos );
	bool class_name ( const char *name );
	weapon_t *weapon ( );
	vec_t shoot_pos ( );
	void pre_think ( );
	bool is_player ( );
	void think ( );
	void set_sequence ( int sequence );
	bool physics_run_think ( int think_method );
	void bone_pos ( int bone, vec_t &out, vec_t &q );
};
