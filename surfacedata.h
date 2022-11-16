#pragma once

struct surfacedata_t;

struct surfacephysicsparams_t {
    PAD ( 20 );
};

struct surfaceaudioparams_t {
    PAD ( 36 );
};

struct surfacesoundnames_t {
    PAD ( 24 );
};

struct surfacegameprops_t {
    PAD ( 8 );
    float m_penetration_modifier;
    float m_damage_modifier;
    unsigned short m_material;
    unsigned char m_climbable;
    PAD ( 4 );
};

struct surfacedata_t {
    surfacephysicsparams_t m_physics;	  
    surfaceaudioparams_t m_audio;
    surfacesoundnames_t	m_sounds;
    surfacegameprops_t m_game;
    PAD ( 48 );
};

class c_phys {
public:
    virtual int	parse_surface_data ( const char *file_name, const char *text_file ) = 0;
    virtual int	surface_prop_count ( void ) const = 0;
    virtual int	get_surface_index ( const char *surface_prop_name ) const = 0;
    virtual void get_physics_properties ( int surface_data_index, float *density, float *thickness, float *friction, float *elasticity ) const = 0;
    virtual surfacedata_t *get_surface_data ( int surface_data_index ) = 0;
    virtual const char *get_string ( unsigned short surface_data_index ) const = 0;
    virtual const char *get_prop_name ( int surface_data_index ) const = 0;
};