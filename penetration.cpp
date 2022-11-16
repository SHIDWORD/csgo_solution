#include "includes.h"

penetration_t penetration { };

enum {
	char_tex_antlion = 'A',
	char_tex_bloodyflesh = 'B',
	char_tex_concrete = 'C',
	char_tex_dirt = 'D',
	char_tex_eggshell = 'E',
	char_tex_flesh = 'F',
	char_tex_grate = 'G',
	char_tex_alienflesh = 'H',
	char_tex_clip = 'I',
	char_tex_grass = 'J',
	char_tex_snow = 'K',
	char_tex_plastic = 'L',
	char_tex_metal = 'M',
	char_tex_sand = 'N',
	char_tex_foliage = 'O',
	char_tex_computer = 'P',
	char_tex_asphalt = 'Q',
	char_tex_brick = 'R',
	char_tex_slosh = 'S',
	char_tex_tile = 'T',
	char_tex_cardboard = 'U',
	char_tex_vent = 'V',
	char_tex_wood = 'W',
	char_tex_glass = 'Y',
	char_tex_warpshield = 'Z'
};

fire_data_t penetration_t::run ( const vec_t src, const vec_t end, player_t *ent ) {
	fire_data_t info { };

	if ( !ent )
		return { };

	const auto weapon = g.m_local->weapon ( );

	if ( !weapon )
		return { };

	const auto data = weapon->data ( );

	if ( !data )
		return { };

	trace_filter_t filter { };

	filter.m_skip = g.m_local;
	bool result = fire_bullet ( data, src, end, &filter, info, ent );

	if ( result && info.m_damage < 1.f )
		return { };

	return info;
}

bool penetration_t::fire_bullet ( const weapon_info_t *data, vec_t src, vec_t pos, trace_filter_t *filter, fire_data_t &fire_info, player_t *ent, bool point ) {
	static cvar_t *sv_penetration_type = interfaces.m_cvar->find_var ( HASH ( "sv_penetration_type" ) );

	fire_info.m_penetrate_count = 4;
	fire_info.m_trace_length = 0.0f;

	auto weapon = g.m_local->weapon ( );
	auto angles = math.vec_angle ( pos - src );

	angles.normalize ( );

	if ( angles.is_zero ( ) || !weapon )
		return false;

	const auto data = weapon->data ( );

	if ( !data )
		return false;

	auto dir = math.ang_vector ( angles );

	fire_info.m_damage = data->m_damage;

	float damage_modifier = 0.5f;
	float penetration_power = 0.f;
	float distance = data->m_range;
	float penetration = data->m_penetration;
	float penetration_modifier = 1.f;
	int penetrate_count = fire_info.m_penetrate_count;
	bool hit_grate = false;
	float damage = 0.f;

	if ( sv_penetration_type->get_int ( ) == 1 )
		penetration_power = penetration;

	while ( ( fire_info.m_penetrate_count > 0 ) && ( fire_info.m_damage >= 1.0f ) ) {
		trace_t trace { };
		ray_t ray { };

		fire_info.m_penetrate_count = data->m_range - fire_info.m_trace_length;

		vec_t end = src + dir * fire_info.m_remaining_length;

		ray.init ( src, end );

		interfaces.m_trace->trace_ray ( ray, 0x4600400B, filter, &trace );
		clip_trace_to_players ( src, end + dir * 40.f, 0x4600400B, filter, &trace );

		if ( trace.m_fraction == 1.0f )
			return false;

		if ( ( trace.m_hitgroup <= 7 ) && ( trace.m_hitgroup > 0 ) ) {
			if ( !trace.m_hit_ent || ( trace.m_hit_ent != ent ) )
				break;

			fire_info.m_trace_length += trace.m_fraction * fire_info.m_remaining_length;

			damage *= std::powf ( data->m_range_modifier, fire_info.m_trace_length * 0.002f );

			fire_info.m_damage = scale_dmg ( trace.m_hitgroup, ent, data->m_armor_ratio, damage );

			return true;
		}

		auto surface_data = interfaces.m_phys_props->get_surface_data ( trace.m_surface.m_props );

		int enter_material = surface_data->m_game.m_material;

		damage_modifier = surface_data->m_game.m_damage_modifier;
		penetration_modifier = surface_data->m_game.m_penetration_modifier;
		penetration = data->m_penetration;
		hit_grate = ( trace.m_contents & contents_t::contents_grate ) != 0;

		float damage = fire_info.m_damage;

		if ( handle_bullet_penetration ( ent, penetration, enter_material, hit_grate, trace, dir, surface_data, penetration_modifier, damage_modifier, penetration_power, penetrate_count, src, distance, fire_info.m_trace_length, damage ) )
			break;
	}

	return true;
}

bool penetration_t::is_breakable_ent ( player_t *ent ) {
	static auto _is_breakable = pattern::find ( x_ ( "client.dll" ), x_ ( "E8 ? ? ? ? 84 C0 75 A1" ) ).rel32 ( );

	static auto m_takedamage_offset = _is_breakable.add ( 38 ).deref ( ).as< uint32_t > ( );

	if ( !ent || !ent->idx ( ) )
		return false;

	auto &takedmg = *reinterpret_cast< uint8_t * > ( reinterpret_cast< uintptr_t >( ent ) + m_takedamage_offset );

	const auto backup_takedmg = takedmg;
	takedmg = 2;

	const auto ret = _is_breakable.as< bool ( __thiscall * )( entity_t * ) > ( ) ( ent );

	takedmg = backup_takedmg;

	return ret;
}

bool penetration_t::trace_to_exit ( vec_t start, vec_t dir, vec_t &end, trace_t &trace_enter, trace_t &trace_exit, float step_size, float max_distance ) {
	float dist = 0.f;
	vec_t last = start;
	int start_contents = 0;

	while ( dist <= max_distance ) {
		dist += step_size;

		end = start + ( dir * dist );

		vec_t trace_end = end - ( dir * step_size );

		if ( start_contents == 0 )
			start_contents = interfaces.m_trace->get_point_contents ( end, 0x4600400B );

		int current_contents = interfaces.m_trace->get_point_contents ( end, 0x4600400B );

		if ( ( current_contents & 0x600400B ) == 0 || ( ( current_contents & 0x40000000 ) && start_contents != current_contents ) ) {
			ray_t ray_world { };

			ray_world.init ( end, trace_end );
			interfaces.m_trace->trace_ray ( ray_world, 0x600400B, 0, &trace_exit );

			if ( trace_exit.m_startsolid == true && ( trace_exit.m_surface.m_flags & surf_t::surf_hitbox ) ) {
				ray_t ray { };
				trace_filter_t filter { };

				filter.m_skip = trace_exit.m_hit_ent;
				ray.init ( end, start );

				interfaces.m_trace->trace_ray ( ray, 0x600400B, &filter, &trace_enter );

				if ( trace_exit.did_hit ( ) && trace_exit.m_startsolid == false ) {
					end = trace_exit.m_endpos;

					return true;
				}
			}
			else if ( trace_exit.did_hit ( ) && trace_exit.m_startsolid == false ) {
				bool start_is_no_draw = !!( trace_enter.m_surface.m_flags & surf_t::surf_nodraw );
				bool exit_is_no_draw = !!( trace_exit.m_surface.m_flags & surf_t::surf_nodraw );

				if ( exit_is_no_draw && is_breakable_ent ( trace_exit.m_hit_ent ) && is_breakable_ent ( trace_enter.m_hit_ent ) ) {
					end = trace_exit.m_endpos;
					return true;
				}
				else if ( exit_is_no_draw == false || ( start_is_no_draw && exit_is_no_draw ) ) {
					vec_t normal = trace_exit.m_plane.m_normal;
					float dot_val = dir.dot ( normal );

					if ( dot_val <= 1.0f ) {
						end = end - ( dir * ( step_size * trace_exit.m_fraction ) );

						return true;
					}
				}
			}
			else if ( trace_enter.did_hit_non_world_ent ( ) && is_breakable_ent ( trace_enter.m_hit_ent ) ) {
				trace_exit = trace_enter;
				trace_exit.m_endpos = start + ( dir * 1.0f );

				return true;
			}
		}
	}

	return false;
}

bool penetration_t::handle_bullet_penetration ( player_t *ent, float &penetration, int &enter_material, bool &hit_grate, trace_t &tr, vec_t &direction, surfacedata_t *surface_data, float penetration_modifier, float damage_modifier, float penetration_power, int &penetration_count, vec_t &src, float distance, float current_distance, float &current_damage ) {
	static cvar_t *sv_penetration_type = interfaces.m_cvar->find_var ( HASH ( "sv_penetration_type" ) );
	static cvar_t *ff_damage_reduction_bullets = interfaces.m_cvar->find_var ( HASH ( "ff_damage_reduction_bullets" ) );
	static cvar_t *ff_damage_bullet_penetration = interfaces.m_cvar->find_var ( HASH ( "ff_damage_bullet_penetration" ) );

	bool is_no_draw = !!( tr.m_surface.m_flags & surf_t::surf_nodraw );
	bool failed_penetrate = false;

	if ( penetration_count == 0 && !hit_grate && !is_no_draw
		&& enter_material != char_tex_glass && enter_material != char_tex_grate )
		failed_penetrate = true;

	if ( penetration <= 0 || penetration_count <= 0 )
		failed_penetrate = true;

	vec_t penetration_end;
	trace_t exit_trace;

	if ( !trace_to_exit ( tr.m_endpos, direction, penetration_end, tr, exit_trace, 4, 90 ) ) {
		if ( ( interfaces.m_trace->get_point_contents ( tr.m_endpos, mask_t::mask_shot_hull ) & mask_t::mask_shot_hull ) == 0 ) {
			failed_penetrate = true;
		}
	}

	if ( failed_penetrate == true )
		return true;

	surfacedata_t *exit_surface_data = interfaces.m_phys_props->get_surface_data ( exit_trace.m_surface.m_props );
	int exit_material = exit_surface_data->m_game.m_material;

	if ( sv_penetration_type->get_int ( ) == 1 ) {
		float damage_lost_percentage = 0.16;

		if ( hit_grate || is_no_draw || enter_material == char_tex_glass || enter_material == char_tex_grate ) {
			if ( enter_material == char_tex_glass || enter_material == char_tex_grate ) {
				penetration_modifier = 3.0f;
				damage_lost_percentage = 0.05;
			}
			else
				penetration_modifier = 1.0f;

			damage_modifier = 0.99f;
		}
		else if ( enter_material == char_tex_flesh && ff_damage_reduction_bullets->get_float ( ) == 0 && tr.m_hit_ent && tr.m_hit_ent->is_player ( ) && tr.m_hit_ent->team ( ) == ent->team ( ) ) {
			if ( ff_damage_bullet_penetration->get_float ( ) == 0 ) {
				penetration_modifier = 0;
				return true;
			}

			penetration_modifier = ff_damage_bullet_penetration->get_float ( );
			damage_modifier = ff_damage_bullet_penetration->get_float ( );
		}
		else {
			float exit_penetration_modifier = exit_surface_data->m_game.m_penetration_modifier;
			float exit_damage_modifier = exit_surface_data->m_game.m_damage_modifier;

			penetration_modifier = ( penetration_modifier + exit_penetration_modifier ) / 2;
			damage_modifier = ( damage_modifier + exit_damage_modifier ) / 2;
		}

		if ( enter_material == exit_material ) {
			if ( exit_material == char_tex_wood || exit_material == char_tex_cardboard ) {
				penetration_modifier = 3;
			}
			else if ( exit_material == char_tex_plastic ) {
				penetration_modifier = 2;
			}
		}

		float trace_distance = ( exit_trace.m_endpos - tr.m_endpos ).length ( );
		float penetration_mod = std::fmaxf ( 0, ( 1 / penetration_modifier ) );
		float percent_damage_chunk = current_damage * damage_lost_percentage;
		float penetration_weapon_modifier = percent_damage_chunk + std::fmaxf ( 0, ( 3 / penetration_power ) * 1.25 ) * ( penetration_mod * 3.0 );
		float lost_damage_object = ( ( penetration_mod * ( trace_distance * trace_distance ) ) / 24 );
		float total_lost_damage = penetration_weapon_modifier + lost_damage_object;

		current_damage -= std::fmaxf ( 0, total_lost_damage );

		if ( current_damage < 1 )
			return true;

		current_distance += trace_distance;
		src = exit_trace.m_endpos;
		distance = ( distance - current_distance ) * 0.5;
		penetration_count--;

		return false;
	}
	else {
		if ( hit_grate || is_no_draw ) {
			penetration_modifier = 1.0f;
			damage_modifier = 0.99f;
		}
		else {
			float exit_penetration_modifier = exit_surface_data->m_game.m_penetration_modifier;
			float exit_damage_modifier = exit_surface_data->m_game.m_damage_modifier;

			if ( exit_penetration_modifier < penetration_modifier ) {
				penetration_modifier = exit_penetration_modifier;
			}
			if ( exit_damage_modifier < damage_modifier ) {
				damage_modifier = exit_damage_modifier;
			}
		}

		if ( enter_material == exit_material ) {
			if ( exit_material == char_tex_wood ||
				exit_material == char_tex_metal ) {
				penetration_modifier *= 2;
			}
		}

		float trace_distance = ( exit_trace.m_endpos - tr.m_endpos ).length ( );

		if ( trace_distance > ( penetration_power * penetration_modifier ) )
			return true;

		current_damage *= damage_modifier;

		penetration_power -= trace_distance / penetration_modifier;
		current_distance += trace_distance;

		src = exit_trace.m_endpos;
		distance = ( distance - current_distance ) * 0.5;
		penetration_count--;

		return false;
	}
}

float penetration_t::scale_dmg ( int hitgroup, player_t *ent, float armor_ratio, float damage ) {
	int armor = ent->armor ( );

	/* https://www.onetap.com/members/llama.1/ */
	float calc_ratio, heavy_ratio, bonus_ratio;
	float old_damage, calc_damage, new_damage;
	float result;

	result = hitgroup - 1;
	
	switch ( hitgroup ) {
	case hitgroup_head:
		damage *= 4.0f;
		break;
	case hitgroup_stomach:
		damage *= 1.25f;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		damage *= 0.75f;
		break;
	default:
		break;
	}
	
	if ( armor > 0 ) {
		switch ( hitgroup ) {
		case hitgroup_generic:
		case hitgroup_chest:
		case hitgroup_stomach:
		case hitgroup_leftarm:
		case hitgroup_rightarm:
		case 8:
			goto LABEL_15;
		case hitgroup_head:
			result = ent->helmet ( );

			if ( !result )
				return result;
		LABEL_15:
			result = ent->heavy_armor ( );

			if ( result && hitgroup == 1 ) {
				calc_ratio = armor_ratio * 0.5f;
				damage = damage + damage;
			LABEL_22:
				old_damage = damage;
				bonus_ratio = 0.33f;
				heavy_ratio = 0.33f;
				calc_damage = ( float ) ( calc_ratio * 0.5f ) * damage;
				new_damage = calc_damage * 0.85f;
				goto LABEL_24;
			}
			heavy_ratio = 0.5;
			bonus_ratio = 1.0;
			calc_ratio = armor_ratio * 0.5;
			if ( result )
				goto LABEL_22;
			old_damage = damage;
			calc_damage = calc_ratio * damage;
			new_damage = calc_damage;
		LABEL_24:
			if ( ( float ) ( ( float ) ( old_damage - calc_damage ) * ( float ) ( heavy_ratio * bonus_ratio ) ) > ( float ) armor )
				new_damage = old_damage - ( float ) ( ( float ) armor / heavy_ratio );
			damage = new_damage;
			break;
		default:
			return result;
		}
	}

	return std::floor ( damage );
}

void penetration_t::clip_trace_to_players ( const vec_t &start, const vec_t &end, unsigned int mask, trace_filter_t *filter, trace_t *tr ) {
	trace_t player_trace;
	ray_t ray;

	float smallest_fraction = tr->m_fraction;
	const float max_range = 60.0f;

	ray.init ( start, end );

	for ( int i = 1; i < interfaces.m_globals->m_max_clients; i++ ) {
		auto player = interfaces.m_entlist->get< player_t * > ( i );

		if ( !player || !player->alive ( ) || player->dormant ( ) )
			continue;

		vec_t world_space_center = player->origin ( ) + ( ( player->mins ( ) + player->maxs ( ) ) * 0.5f );

		if ( filter && filter->should_hit_entity ( player, mask ) == false )
			continue;

		float range = math.distance_to_ray ( world_space_center, start, end );

		if ( range < 0.0f || range > max_range )
			continue;

		interfaces.m_trace->clip_ray_to_entity ( ray, mask | contents_t::contents_hitbox, player, &player_trace );

		if ( player_trace.m_fraction < smallest_fraction ) {
			*tr = player_trace;
			smallest_fraction = player_trace.m_fraction;
		}
	}
}