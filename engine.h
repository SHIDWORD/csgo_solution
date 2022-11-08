#pragma once

enum client_frame_stage_t {
	frame_undefined = -1,
	frame_start,
	frame_net_update_start,
	frame_net_update_postdataupdate_start,
	frame_net_update_postdataupdate_end,
	frame_net_update_end,
	frame_render_start,
	frame_render_end,
	frame_net_full_frame_update_on_remove
};

struct player_info_t {
	std::uint64_t m_version;

	union {
		std::uint64_t m_xuid;

		struct {
			std::uint32_t m_xuid_low;
			std::uint32_t m_xuid_high;
		};
	};

	char			m_name [ 128 ];
	int				m_user_id;
	char			m_steam_id [ 33 ];
	std::uint32_t	m_friends_id;
	char			m_friends_name [ 128 ];
	bool			m_fake_player;
	bool			m_is_hltv;
	std::uint32_t	m_custom_files [ 4 ];
	std::uint8_t	m_files_downloaded;
};

class c_engine {
	enum indices_t : std::uintptr_t {
		_get_screen_size = 5,
		_get_player_info = 8,
		_get_local_player = 12,
		_get_view_angles = 18,
		_set_view_angles = 19,
		_get_max_clients = 20,
		_is_in_game = 26,
		_is_connected = 27,
		_world_to_screen_matrix = 37,
		_get_bsp_tree_query = 43,
		_execute_client_cmd = 108,
		_client_cmd_unrestricted = 114,
	};
public:
	void get_screen_size ( int &width, int &height ) {
		using get_screen_size_fn = void ( __thiscall * )( void *, int &, int & );
		vf< get_screen_size_fn > ( this, indices_t::_get_screen_size )( this, width, height );
	}

	void get_player_info ( int index, player_info_t *info ) {
		using get_player_info_fn = void ( __thiscall * )( void*, int, player_info_t * );
		vf< get_player_info_fn > ( this, indices_t::_get_player_info )( this, index, info );
	}

	int get_local_player ( ) {
		using get_local_player_fn = int ( __thiscall * )( void * );
		return vf< get_local_player_fn > ( this, indices_t::_get_local_player )( this );
	}

	void get_view_angles ( const vec_t &angles ) {
		using get_view_angles_fn = void ( __thiscall * )( void *, const vec_t & );
		vf< get_view_angles_fn > ( this, indices_t::_get_view_angles )( this, angles );
	}

	void set_view_angles ( const vec_t &angles ) {
		using set_view_angles_fn = void ( __thiscall * )( void *, const vec_t & );
		vf< set_view_angles_fn > ( this, indices_t::_set_view_angles )( this, angles );
	}

	int get_max_clients ( ) {
		using get_max_clients_fn = int ( __thiscall * )( void * );
		return vf< get_max_clients_fn > ( this, indices_t::_get_max_clients )( this );
	}

	bool is_in_game ( ) {
		using is_in_game_fn = bool ( __thiscall * )( void * );
		return vf< is_in_game_fn > ( this, indices_t::_is_in_game )( this );
	}

	bool is_connected ( ) {
		using is_connected_fn = bool ( __thiscall * )( void * );
		return vf< is_connected_fn > ( this, indices_t::_is_connected )( this );
	}

	void* get_bsp_tree_query ( ) {
		using get_bsp_tree_query_fn = void *( __thiscall * )( void * );
		return vf< get_bsp_tree_query_fn > ( this, indices_t::_get_bsp_tree_query )( this );
	}
};