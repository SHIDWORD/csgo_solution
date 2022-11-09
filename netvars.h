#pragma once

struct netvar_data_t {
	hash32_t m_table_name;
	hash32_t m_prop_name;
	int		 m_offset;
};

inline std::unordered_map < hash32_t, std::unordered_map < hash32_t, netvar_data_t > > netvar_data { };

struct netvars_t {
	bool init ( );
	void store_table ( c_recv_table *table );
	int get_offset ( hash32_t table, hash32_t prop );
};

extern netvars_t netvars;

#define NETVAR( t, func, table, prop ) \
__forceinline t& func( ) { \
	auto offset = netvar_data [ fnv1a::hash_const ( table ) ][ fnv1a::hash_const ( prop ) ].m_offset; \
	return *( t* )( std::uintptr_t( this ) + offset ); \
}

#define OFFSET( t, func, offset ) \
__forceinline t& func( ) { \
	return *( t* )( std::uintptr_t( this ) + offset ); \
}