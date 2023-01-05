#ifndef types_h
#define types_h

#define uint unsigned int
#define uchar unsigned char
#define null NULL
#define bool int
#define true 1
#define false 0

#define Type(tp, a) i##tp, 
enum idType { types };
#undef Type

#define Type(tp, ...) typedef struct sd##tp *tp; struct sd##tp { uint type; __VA_ARGS__ } st##tp; 
types
#undef Type

// compiler stuff
#ifdef _MSC_VER
  #define inln __forceinline // use __forceinline (VC++ specific)
#else
  #define inln inline // use standard inline
#endif

// operator macros
inln void* newType(uint sztp, uint itp) { uint* r = malloc(sztp); *r = itp; return r; }
#define new(tp) ((tp)newType(sizeof(st##tp), i##tp))
#define is(var,tp) (/*var && (*/*(uint*)var == i##tp)
#define as(var,tp) ((tp)var)





#endif 
