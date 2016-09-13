#version 130

#define MASK 1

#define _NO_ACTION 0
#define _ROTATE 1
#define _ROTATE_CENTER 2
#define _TRANSFORM 4
#define _SCALE 8

in  vec3 vPosition;
in  vec3 vColor;

uniform float rot;
uniform float x_trans;
uniform float y_trans;
uniform float x_trans_cent;
uniform float y_trans_cent;
uniform float scale;
uniform float hscale; // horizontal scale
uniform float vscale; // vertical scale
uniform mat3 rot_mat;

uniform int action;	// action = 0: no action
			//	  = 1: rotate
			//	  = 2: rotate around center
			//	  = 4: scale
			// 	  = 8: transform
			// 	  = 16: use rot_mat

out vec4 color;

void
main()
{
    mat3 m, trans_m, rot_m, scale_m, fix_scale_m;
    mat3 centralize, restore;
    
    centralize[0][0] = 1.0f; centralize[0][1] = 0.0f; centralize[0][2] = -x_trans_cent;
    centralize[1][0] = 0.0f; centralize[1][1] = 1.0f; centralize[1][2] = -y_trans_cent;
    centralize[2][0] = 0.0f; centralize[2][1] = 0.0f; centralize[2][2] = 1.0f	      ;
    
    restore[0][0] = 1.0f; restore[0][1] = 0.0f; restore[0][2] = x_trans_cent;
    restore[1][0] = 0.0f; restore[1][1] = 1.0f; restore[1][2] = y_trans_cent;
    restore[2][0] = 0.0f; restore[2][1] = 0.0f; restore[2][2] = 1.0f	    ;
    
    m[0][0] = 1.0f  ; m[0][1] = 0.0f  ; m[0][2] = 0.0f;
    m[1][0] = 0.0f  ; m[1][1] = 1.0f  ; m[1][2] = 0.0f;
    m[2][0] = 0.0f  ; m[2][1] = 0.0f  ; m[2][2] = 1.0f;
    
    fix_scale_m[0][0] = hscale; fix_scale_m[0][1] = 0.0f  ; fix_scale_m[0][2] = 0.0f;
    fix_scale_m[1][0] = 0.0f  ; fix_scale_m[1][1] = vscale; fix_scale_m[1][2] = 0.0f;
    fix_scale_m[2][0] = 0.0f  ; fix_scale_m[2][1] = 0.0f  ; fix_scale_m[2][2] = 1.0f;
    
    trans_m[0][0] = 1.0f; trans_m[0][1] = 0.0f; trans_m[0][2] = x_trans;
    trans_m[1][0] = 0.0f; trans_m[1][1] = 1.0f; trans_m[1][2] = y_trans;
    trans_m[2][0] = 0.0f; trans_m[2][1] = 0.0f; trans_m[2][2] = 1.0f   ;
    
    rot_m[0][0] = cos(rot); rot_m[0][1] = -sin(rot); rot_m[0][2] = 0.0f;
    rot_m[1][0] = sin(rot); rot_m[1][1] = cos(rot) ; rot_m[1][2] = 0.0f;
    rot_m[2][0] = 0.0f    ; rot_m[2][1] = 0.0f       ; rot_m[2][2] = 1.0f;
    
    scale_m[0][0] = scale; scale_m[0][1] = 0.0f ; scale_m[0][2] = 0.0f;
    scale_m[1][0] = 0.0f ; scale_m[1][1] = scale; scale_m[1][2] = 0.0f;
    scale_m[2][0] = 0.0f ; scale_m[2][1] = 0.0f ; scale_m[2][2] = 1.0f;
    
    //m = scale_m * rot_m * trans_m * m;
    //m = trans_m * m;
    
    vec3 v = vec3(vPosition);
    float tmp = v.z;
    v.z = 1.0f;
    if ( ((action >> 4) & MASK) == 1 )
        v = rot_mat * v;
    if ( (action & MASK) == 1 )
        m = rot_m;
    else if ( ((action >> 1) & MASK) == 1 )
        m = restore * rot_m * centralize;
    if ( ((action >> 3) & MASK) == 1 )
        m = trans_m * m;
    if ( ((action >> 2) & MASK) == 1 )
        m = scale_m * m;
    
    v = v * m;
    v.z = tmp;
    gl_Position = vec4(fix_scale_m * v,1.0);
    
    color = vec4( vColor, 1.0 );
}
