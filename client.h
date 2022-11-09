#pragma once

class c_client_class;
class c_recv_table;
class c_recv_prop;
class c_recv_proxy_data;
class c_variant;

using create_client_class_fn = void *( * )( int, int );
using create_event_fn = void *( * )( );

using recv_var_proxy_fn = void ( * )( const c_recv_proxy_data *, void *, void * );
using array_length_recv_proxy_fn = void ( * )( void *, int, int );
using data_table_recv_var_proxy_fn = void ( * )( const c_recv_prop *, void **, void *, int );

enum send_prop_type_t {
	dpt_int = 0,
	dpt_float,
	dpt_vector,
	dpt_vectorxy,
	dpt_string,
	dpt_array,
	dpt_datatable,
	dpt_int64,
	dpt_numsendproptypes
};

class c_variant {
public:
	union {
		float     floating_point;
		long      integer;
		char *string;
		void *data;
		float     vector [ 3 ];
		long long int64;
	};

	send_prop_type_t m_type;
};

class c_recv_proxy_data {
public:
	const c_recv_prop *m_recv_prop;
	c_variant          m_value;
	int				   m_element;
	int				   m_object_id;
};

class c_recv_prop {
public:
	char *m_var_name;
	send_prop_type_t               m_recv_type;
	int                          m_flags;
	int                          m_string_buffer_size;
	bool                         m_inside_array;
	const void *m_extra_data;
	c_recv_prop *m_array_prop;
	array_length_recv_proxy_fn   m_array_length_proxy;
	recv_var_proxy_fn            m_proxy_fn;
	data_table_recv_var_proxy_fn m_data_table_proxy_fn;
	c_recv_table *m_data_table;
	int                          m_offset;
	int                          m_element_stride;
	int                          m_num_elements;
	const char *m_parent_array_prop_name;
};

class c_client_class {
public:
	create_client_class_fn m_create_fn;
	create_event_fn        m_create_event_fn;
	char *m_network_name;
	c_recv_table *m_recv_table;
	c_client_class *m_next;
	int	                   m_class_id;
	const char *m_map_class_name;
};

class c_recv_table {
public:
	c_recv_prop *m_props;
	int	         m_num_props;
	void *m_decoder;
	char *m_net_table_name;
	bool         m_initialized;
	bool         m_in_main_list;
};

class c_client {
	enum indices_t : std::uintptr_t {
		_create_move = 22,
		_frame_stage_notify = 37,
		_get_all_classes = 8
	};
public:
	__forceinline c_client_class *get_all_classes ( ) {
		using get_all_classes_fn = c_client_class * ( __thiscall * )( void * );
		return util::get_method< get_all_classes_fn > ( this, indices_t::_get_all_classes )( this );
	}
};