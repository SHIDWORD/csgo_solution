#include "includes.h"

netvars_t netvars { };

bool netvars_t::init ( ) {
	auto client_class = interfaces::m_client->get_all_classes ( );

	if ( !client_class )
		return false;

	for ( ; client_class; client_class = client_class->m_next )
		store_table ( client_class->m_recv_table );

	return true;
}

void netvars_t::store_table ( c_recv_table *table ) {
	for ( auto i = 0; i < table->m_num_props; i++ ) {
		auto prop = &table->m_props [ i ];

		if ( !prop || isdigit ( prop->m_var_name [ 0 ] ) )
			continue;

		if ( prop->m_recv_type == dpt_datatable && prop->m_data_table && prop->m_data_table->m_num_props > 0 )
			store_table ( prop->m_data_table );

		const auto table_hash = fnv1a::hash_const ( table->m_net_table_name );
		const auto prop_hash = fnv1a::hash_const ( prop->m_var_name );

		/* store data. */
		netvar_data [ table_hash ][ prop_hash ].m_offset = prop->m_offset;
		netvar_data [ table_hash ][ prop_hash ].m_table_name = table_hash;
		netvar_data [ table_hash ][ prop_hash ].m_prop_name = prop_hash;
	}
}

int netvars_t::get_offset ( hash32_t table, hash32_t prop ) {
	return netvar_data [ table ][ prop ].m_offset;
}