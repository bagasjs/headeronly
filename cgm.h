#ifndef CGM_H
#define CGM_H

#define CGM_PI 3.14159265358979323846f
#define CGM_PI_2 2.0f * CGM_PI
#define CGM_HALF_PI 0.5f * CGM_PI
#define CGM_QUARTER_PI 0.25f * CGM_PI
#define CGM_ONE_OVER_PI 1.0f / CGM_PI
#define CGM_ONE_OVER_TWO_PI 1.0f / CGM_PI_2
#define CGM_SQRT_TWO 1.41421356237309504880f
#define CGM_SQRT_THREE 1.73205080756887729352f #define CGM_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define CGM_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define CGM_DEG2RAD_MULTIPLIER CGM_PI / 180.0f
#define CGM_RAD2DEG_MULTIPLIER 180.0f / CGM_PI
#define CGM_SEC_TO_US_MULTIPLIER (1000.0f * 1000.0f)
#define CGM_SEC_TO_MS_MULTIPLIER 1000.0f
#define CGM_MS_TO_SEC_MULTIPLIER 0.001f
#define CGM_INFINITY 1e30f
#define CGM_EPSILON 1.192092896e-07f

#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif
#ifndef BOOL
    #define BOOL char
#endif

typedef union V2f {
    float elements[2];
    struct {
        union { float x, r, s, u; };
        union { float y, g, t, v; };
    };
} V2f;

typedef union V3f {
    float elements[3];
    struct {
        union { float x, r, s, u; };
        union { float y, g, t, v; };
        union { float z, b, p, w; };
    };
} V3f;

typedef union V4f {
    float elements[4];
    struct {
        union { float x, r, s; };
        union { float y, g, t; };
        union { float z, b, p; };
        union { float w, a, q; };
    };
} V4f;

typedef union M4f {
    V4f rows[4];
    float elements[4*4];
} M4f;

V2f v2f(float x, float y);
V2f v2f_add(V2f a, V2f b);
V2f v2f_sub(V2f a, V2f b);
V2f v2f_mul(V2f a, V2f b);
V2f v2f_div(V2f a, V2f b);
float v2f_length(V2f a);
V2f v2f_normalize(V2f a);
float v2f_distance(V2f a, V2f b);
BOOL v2f_cmp(V2f a, V2f b);

V3f v3f(float x, float y, float z);
V3f v3f_add(V3f a, V3f b);
V3f v3f_sub(V3f a, V3f b);
V3f v3f_mul(V3f a, V3f b);
V3f v3f_div(V3f a, V3f b);
float v3f_length(V3f a);
V3f v3f_normalize(V3f a);
float v3f_distance(V3f a, V3f b);
BOOL v3f_cmp(V3f a, V3f b);

V4f v4f(float x, float y, float z, float w);
V4f v4f_add(V4f a, V4f b);
V4f v4f_sub(V4f a, V4f b);
V4f v4f_mul(V4f a, V4f b);
V4f v4f_div(V4f a, V4f b);
float v4f_length(V4f a);
V4f v4f_normalize(V4f a);
float v4f_distance(V4f a, V4f b);
float v4f_dot(V4f a, V4f b);
BOOL v4f_cmp(V4f a, V4f b);

M4f m4f(float a);
M4f m4f_ortho(float left, float right, float bottom, float top, float near, float far);
M4f m4f_perspective(float fovRadians, float aspect_ratio, float near, float far);
M4f m4f_dot(M4f a, M4f b);

M4f m4f_translate(V3f position);
M4f m4f_scale(V3f scale);
M4f m4f_rotate_x(float a_radians);
M4f m4f_rotate_y(float a_radians);
M4f m4f_rotate_z(float a_radians);
M4f m4f_rotate(V3f angles_radians);


#ifdef CGM_EXTENSIONS
void v2f_dump(V2f a);
void v3f_dump(V3f a);
void v4f_dump(V4f a);
void m4f_dump(M4f a);
#endif

#endif // CGM_H

#ifdef CGM_IMPLEMENTATION

#include <math.h> // sqrt fabs

V2f v2f(float x, float y) { return (V2f){ .x = x, .y = y, }; }
V2f v2f_add(V2f a, V2f b) { return (V2f){ .x=a.x+b.x, .y=a.y+b.y, }; }
V2f v2f_sub(V2f a, V2f b) { return (V2f){ .x=a.x-b.x, .y=a.y-b.y, }; }
V2f v2f_mul(V2f a, V2f b) { return (V2f){ .x=a.x*b.x, .y=a.y*b.y, }; }
V2f v2f_div(V2f a, V2f b) { return (V2f){ .x=a.x/b.x, .y=a.y/b.y, }; }
float v2f_length(V2f a) { return (float)sqrt((double)(a.x*a.x + a.y*a.y)); }
V2f v2f_normalize(V2f a) { float l = v2f_length(a); return (V2f){ .x = a.x/l, .y = a.y/l, }; }
float v2f_distance(V2f a, V2f b) { return v2f_length(v2f( a.x-b.x, a.y-b.y )); }
BOOL v2f_cmp(V2f a, V2f b) { return fabs(a.x-b.x) <= CGM_EPSILON && fabs(a.y-b.y) <= CGM_EPSILON; }

V3f v3f(float x, float y, float z) { return (V3f){ .x=x, .y=y, .z=z }; }
V3f v3f_add(V3f a, V3f b) { return (V3f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z}; }
V3f v3f_sub(V3f a, V3f b) { return (V3f){ .x=a.x-b.x, .y=a.y-b.y, .z=a.z-b.z}; }
V3f v3f_mul(V3f a, V3f b) { return (V3f){ .x=a.x*b.x, .y=a.y*b.y, .z=a.z*b.z}; }
V3f v3f_div(V3f a, V3f b) { return (V3f){ .x=a.x/b.x, .y=a.y/b.y, .z=a.z/b.z}; }
float v3f_length(V3f a) { return (float)sqrt((double)(a.x*a.x + a.y*a.y + a.z*a.z)); }
V3f v3f_normalize(V3f a) { float l = v3f_length(a); return (V3f){ .x = a.x/l, .y = a.y/l, .z=a.z/l }; }
float v3f_distance(V3f a, V3f b) { return v3f_length(v3f( a.x-b.x, a.y-b.y, a.z-b.z )); }
BOOL v3f_cmp(V3f a, V3f b) { return fabs(a.x-b.x) <= CGM_EPSILON 
    && fabs(a.y-b.y) <= CGM_EPSILON
    && fabs(a.z-b.z) <= CGM_EPSILON; }

V4f v4f(float x, float y, float z, float w) { return (V4f){ .x=x, .y=y, .z=z, .w=w }; }
V4f v4f_add(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
V4f v4f_sub(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
V4f v4f_mul(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
V4f v4f_div(V4f a, V4f b) { return (V4f){ .x=a.x+b.x, .y=a.y+b.y, .z=a.z+b.z, .w=a.w+b.w }; }
float v4f_length(V4f a) { return (float)sqrt((double)(a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w)); }
V4f v4f_normalize(V4f a) { float l = v4f_length(a); return (V4f){ .x = a.x/l, .y = a.y/l, .z=a.z/l, .w=a.w/l }; }
float v4f_distance(V4f a, V4f b) { return v4f_length(v4f( a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w )); }
float v4f_dot(V4f a, V4f b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }
BOOL v4f_cmp(V4f a, V4f b) { return fabs(a.x-b.x) <= CGM_EPSILON 
    && fabs(a.y-b.y) <= CGM_EPSILON
    && fabs(a.w-b.w) <= CGM_EPSILON
    && fabs(a.z-b.z) <= CGM_EPSILON; }

M4f m4f(float a) 
{
    M4f res = {0};
    res.elements[0] = a;
    res.elements[5] = a;
    res.elements[10] = a;
    res.elements[15] = a;
    return res;
}


M4f m4f_ortho(float left, float right, float bottom, float top, float near, float far) 
{
    M4f res = m4f(1.0f);
    float lr = 1.0f / (left - right);
    float bt = 1.0f / (bottom - top);
    float nf = 1.0f / (near - far);

    res.elements[0] = -2.0f * lr;
    res.elements[5] = -2.0f * bt;
    res.elements[10] = 2.0f * nf;

    res.elements[12] = (left + right) * lr;
    res.elements[13] = (top + bottom) * bt;
    res.elements[14] = (near + far) * nf;

    return res;
}

M4f m4f_perspective(float fov, float aspect_ratio, float near, float far) 
{
    float half_tan_fov = tan(fov*0.5f);
    M4f res = {0};
    res.elements[0] = 1.0f / (aspect_ratio * half_tan_fov);
    res.elements[5] = 1.0f / half_tan_fov;
    res.elements[10] = -((far + near) / (far - near));
    res.elements[11] = -1.0f;
    res.elements[14] = -((2.0f * far * near) / (far - near));
    return res;
}

M4f m4f_dot(M4f a, M4f b) 
{
    M4f res = m4f(0.0f);

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            for(int k = 0; k < 4; ++k) {
                res.elements[i * 4 + j] += a.elements[i * 4 + k] * b.elements[k * 4 + j];
            }
        }
    }

    return res;
}

#ifdef CGM_EXTENSIONS
#include <stdio.h>
void v2f_dump(V2f a)
{
    printf("[ %f, %f ]\n", a.x, a.y);
}

void v3f_dump(V3f a)
{
    printf("[ %f, %f, %f ]\n", a.x, a.y, a.z);
}

void v4f_dump(V4f a)
{
    printf("[ %f, %f, %f, %f ]\n", a.x, a.y, a.z, a.w);
}

void m4f_dump(M4f m)
{
    printf("[\n");
    for(int i = 0; i < 4; ++i) {
        V4f a = m.rows[i];
        printf("\t%f, %f, %f, %f,\n", a.x, a.y, a.z, a.w);
    }
    printf("]\n");
}
#endif

#endif // CGM_IMPLEMENTATION
