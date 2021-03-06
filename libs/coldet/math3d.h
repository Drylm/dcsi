/*   ColDet - C++ 3D Collision Detection Library
 *   Copyright (C) 2000   Amir Geva
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 *
 * Any comments, questions and bug reports send to:
 *   photon@photoneffect.com
 *
 * Or visit the home page: http://photoneffect.com/coldet/
 */

#pragma once

#include <math.h>
#include <kos.h>

#define MATH_PI 3.1415227

struct Vector3D;
struct Matrix3;
struct Matrix3D;
struct Plane; 


inline float flabs(float f) { return (f>=0.0f?f:-f); }
const float epsilon=1e-8f;
inline bool IsZero(float f) { return flabs(f)<epsilon; }

Vector3D operator*(float scalar, const Vector3D& v);
/** Dot product. */
float    operator*(const Vector3D& v1, const Vector3D& v2);
Vector3D operator+(const Vector3D& v1, const Vector3D& v2);
Vector3D operator-(const Vector3D& v1, const Vector3D& v2);
Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2);
Matrix3D operator*(const Matrix3D& m1, const Matrix3D& m2);
Matrix3D operator*(float scalar, const Matrix3D& m);

struct Vector3D
{
  float x,y,z;
  static const Vector3D Zero;

  Vector3D() {}
  Vector3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
  Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z) {}

  Vector3D& operator+=(const Vector3D& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
  Vector3D& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }
  Vector3D& operator/=(float s) { return *this *= (1.0f/s); }
  bool      operator==(const Vector3D& v) { return x==v.x && y==v.y && z==v.z; }

  Vector3D operator-       () const { return Vector3D(-x,-y,-z); }
  float    SquareMagnitude () const { return x*x+y*y+z*z; }
  float    Magnitude       () const { return (float)sqrt(SquareMagnitude()); }
  Vector3D Normalized      () const { return (1.0f/Magnitude())*(*this); }
  float    operator[] (int i) const { return ((float*)&x)[i]; }
  float&   operator[] (int i)       { return ((float*)&x)[i]; }
};

#define _11 sclr.s11
#define _12 sclr.s12
#define _13 sclr.s13
#define _14 sclr.s14
#define _21 sclr.s21
#define _22 sclr.s22
#define _23 sclr.s23
#define _24 sclr.s24
#define _31 sclr.s31
#define _32 sclr.s32
#define _33 sclr.s33
#define _34 sclr.s34
#define _41 sclr.s41
#define _42 sclr.s42
#define _43 sclr.s43
#define _44 sclr.s44

/** 3x3 matrix */
struct Matrix3
{
  union {
    struct { float s11,s12,s13,
                   s21,s22,s23,
                   s31,s32,s33; } sclr;
    float m[3][3];
  };
  static const Matrix3 Identity;

  Vector3D& baseRow(int i) { return *((Vector3D*)m[i]); }
  float  operator() (int i, int j) const { return m[i][j]; }
  float& operator() (int i, int j)       { return m[i][j]; }
};

/** 4x4 matrix, used for transformations. */
struct Matrix3D
{
  union {
    struct { float s11,s12,s13,s14,
                   s21,s22,s23,s24,
                   s31,s32,s33,s34,
                   s41,s42,s43,s44; } sclr;
    float m[4][4];
  };
  static const Matrix3D Identity;

  Matrix3D() {}

  Matrix3D(float f11, float f12, float f13, float f14,
           float f21, float f22, float f23, float f24,
           float f31, float f32, float f33, float f34,
           float f41, float f42, float f43, float f44)
  {
    _11=f11; _12=f12; _13=f13; _14=f14;
    _21=f21; _22=f22; _23=f23; _24=f24;
    _31=f31; _32=f32; _33=f33; _34=f34;
    _41=f41; _42=f42; _43=f43; _44=f44;
  }

  Matrix3D& operator*= (const Matrix3D& m)
  {
    return *this = *this * m;
  }

  friend Matrix3D PitchMatrix3D(const float theta);
  friend Matrix3D YawMatrix3D(const float theta);
  friend Matrix3D RollMatrix3D(const float theta);
  void rotate(const Vector3D& v);

  Matrix3D Inverse() const;
  Matrix3D Adjoint() const;
  float Determinant() const;

  float  operator() (int i, int j) const { return m[i][j]; }
  float& operator() (int i, int j)       { return m[i][j]; }
};

/** 3D Plane.  Used in conjunction with triangles. */
struct Plane
{
  Vector3D normal;
  float    d;

  Plane(const Vector3D& a, const Vector3D& b, const Vector3D& c)
  {
    normal = CrossProduct(b - a, c - a).Normalized();
    d = -normal * a;
  }

  float Classify(const Vector3D& v)
  {
    return v * normal + d;
  }
};

inline Vector3D operator* (float scalar, const Vector3D& v)
{
  return Vector3D(scalar*v.x,scalar*v.y,scalar*v.z);
}

inline Vector3D operator+ (const Vector3D& v1, const Vector3D& v2)
{
  return Vector3D(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

inline Vector3D operator- (const Vector3D& v1, const Vector3D& v2)
{
  return Vector3D(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}

inline float operator* (const Vector3D& v1, const Vector3D& v2)
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline Vector3D CrossProduct(const Vector3D& v1, const Vector3D& v2)
{
  return Vector3D(v1.y*v2.z-v2.y*v1.z,
                  v1.z*v2.x-v2.z*v1.x,
                  v1.x*v2.y-v2.x*v1.y);
}

inline Vector3D Transform(const Vector3D& v, const Matrix3D& m)
{
  return Vector3D(v.x*m._11 + v.y*m._21 + v.z*m._31 + m._41,
                  v.x*m._12 + v.y*m._22 + v.z*m._32 + m._42,
                  v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43);
}

inline Vector3D rotateVector(const Vector3D& v, const Matrix3D& m)
{
  return Vector3D(v.x*m._11 + v.y*m._21 + v.z*m._31,
                  v.x*m._12 + v.y*m._22 + v.z*m._32,
                  v.x*m._13 + v.y*m._23 + v.z*m._33);
}

inline Matrix3D operator*(float scalar, const Matrix3D& m)
{
  return Matrix3D(scalar*m(0,0),scalar*m(0,1),scalar*m(0,2),scalar*m(0,3),
                  scalar*m(1,0),scalar*m(1,1),scalar*m(1,2),scalar*m(1,3),
                  scalar*m(2,0),scalar*m(2,1),scalar*m(2,2),scalar*m(2,3),
                  scalar*m(3,0),scalar*m(3,1),scalar*m(3,2),scalar*m(3,3));
}

inline Matrix3D operator*(const Matrix3D& m1, const Matrix3D& m2)
{
  return Matrix3D(
    m1._11*m2._11 + m1._12*m2._21 + m1._13*m2._31 + m1._14*m2._41,
    m1._11*m2._12 + m1._12*m2._22 + m1._13*m2._32 + m1._14*m2._42,
    m1._11*m2._13 + m1._12*m2._23 + m1._13*m2._33 + m1._14*m2._43,
    m1._11*m2._14 + m1._12*m2._24 + m1._13*m2._34 + m1._14*m2._44,
    m1._21*m2._11 + m1._22*m2._21 + m1._23*m2._31 + m1._24*m2._41,
    m1._21*m2._12 + m1._22*m2._22 + m1._23*m2._32 + m1._24*m2._42,
    m1._21*m2._13 + m1._22*m2._23 + m1._23*m2._33 + m1._24*m2._43,
    m1._21*m2._14 + m1._22*m2._24 + m1._23*m2._34 + m1._24*m2._44,
    m1._31*m2._11 + m1._32*m2._21 + m1._33*m2._31 + m1._34*m2._41,
    m1._31*m2._12 + m1._32*m2._22 + m1._33*m2._32 + m1._34*m2._42,
    m1._31*m2._13 + m1._32*m2._23 + m1._33*m2._33 + m1._34*m2._43,
    m1._31*m2._14 + m1._32*m2._24 + m1._33*m2._34 + m1._34*m2._44,
    m1._41*m2._11 + m1._42*m2._21 + m1._43*m2._31 + m1._44*m2._41,
    m1._41*m2._12 + m1._42*m2._22 + m1._43*m2._32 + m1._44*m2._42,
    m1._41*m2._13 + m1._42*m2._23 + m1._43*m2._33 + m1._44*m2._43,
    m1._41*m2._14 + m1._42*m2._24 + m1._43*m2._34 + m1._44*m2._44);
}

inline Matrix3D
PitchMatrix3D(const float c, const float s)
{
   return Matrix3D(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f,    c,   -s, 0.0f,
                   0.0f,    s,    c, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix3D
YawMatrix3D(const float c, const float s)
{
   return Matrix3D(   c, 0.0f,    s, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                     -s, 0.0f,    c, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix3D
PitchMatrix3D(const float theta)
{
   return PitchMatrix3D((float) cos(theta), (float) sin(theta));
}


inline Matrix3D
YawMatrix3D(const float theta)
{
   return YawMatrix3D((float) cos(theta), (float) sin(theta));
}


inline Matrix3D
RollMatrix3D(const float c, const float s)
{
   return Matrix3D(c,   -s,    0.0f, 0.0f,
                   s,    c,    0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}


inline Matrix3D
RollMatrix3D(const float theta)
{
   return RollMatrix3D((float) cos(theta), (float) sin(theta));
}

inline void
Matrix3D::rotate(const Vector3D& v)
{
   if (v.x!=0.0f) *this = PitchMatrix3D(v.x) * (*this);
   if (v.y!=0.0f) *this = YawMatrix3D  (v.y) * (*this);
   if (v.z!=0.0f) *this = RollMatrix3D (v.z) * (*this);
}

inline Matrix3D
TranslateMatrix3D(const Vector3D& v)
{
  return Matrix3D(1.0f,0.0f,0.0f,0.0f,
                  0.0f,1.0f,0.0f,0.0f,
                  0.0f,0.0f,1.0f,0.0f,
                   v.x, v.y, v.z,1.0f);
}


inline Matrix3D
ScaleMatrix3D(const Vector3D& v)
{
   return Matrix3D( v.x,0.0f,0.0f,0.0f,
                   0.0f, v.y,0.0f,0.0f,
                   0.0f,0.0f, v.z,0.0f,
                   0.0f,0.0f,0.0f,1.0f);
}


inline Matrix3D
ScaleMatrix3D(const float s)
{
   return ScaleMatrix3D(Vector3D(s,s,s));
}


template<class T>
inline T Max(T a, T b)
{
  return (a>b ? a : b);
}

template<class T>
inline T Min(T a, T b)
{
  return (a<b ? a : b);
}


// This returns the absolute value of "num"
static float absolute(float num) {
  if(num < 0)
    return (0 - num);
  return num;
}



// This returns the normal of a polygon
//the polygon is an array of Vector3D, each Vector3D being each vertice of the polygon
static Vector3D getNormalOfPoly (Vector3D polygon [])
{

  // Get 2 vectors from the polygon (2 sides)
  Vector3D side1 (polygon[2] - polygon[0]);
  Vector3D side2 (polygon[1] - polygon[0]);

  Vector3D vecNorm =  CrossProduct(side1, side2);       // Take the cross product of our 2 sides (ie the plan the polygon is on) to get a perpendicular vector

  //we normalize it, cos its magnitude has no meaning and is useless
  return vecNorm.Normalized();
}


//returns the distance between 2 points :
static float getDistance (const Vector3D& vPoint1, const Vector3D& vPoint2)
{
  // Distance = sqrt(  (P2.x - P1.x)^2 + (P2.y - P1.y)^2 + (P2.z - P1.z)^2 )
  return (float) sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
		       (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
		       (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );
}

//returns the angle between 2 angles, in radians
static double angleBetweenVectors(const Vector3D& Vector1, const Vector3D& Vector2)
{                           
  // the Dot Product of returns the cosine of the angle between 2 vectors.
  // that is assuming they are unit vectors (normalize vectors).
  // So, if we don't have a unit vector, then instead of just saying  arcCos(DotProduct(A, B))
  // We need to divide the dot product by the magnitude of the 2 vectors multiplied by each other.
  // Here is the equation:   arc cosine of (V . W / || V || * || W || )
  // the || V || means the magnitude of V.  This then cancels out the magnitudes dot product magnitudes.
  // But basically, if you have normalize vectors already, you can forget about the magnitude part.
  
  // Get the dot product of the vectors
  float dotProduct = Vector1 * Vector2;               

  // Get the product of both of the vectors magnitudes
  float vectorsMagnitude = Vector1.Magnitude() * Vector2.Magnitude() ;
  
  // Get the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
  // (IE.   PI/2 radians = 90 degrees      PI radians = 180 degrees    2*PI radians = 360 degrees)
  // To convert radians to degrees use this equation:   radians * (PI / 180)
  // TO convert degrees to radians use this equation:   degrees * (180 / PI)
  double angle = acos( dotProduct / vectorsMagnitude );
  
  // Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate.
  // acos() thinks it's funny when it returns -1.#IND0000000.  If we don't do this check,
  // our collision results will sometimes say we are colliding when we aren't. Usually
  // this value is found when the dot product and the magnitude are the same value.
  // We want to return 0 when this happens.
    if(isnan(angle))
        return 0;
    
    // Return the angle in radians
    return( angle );
}


/* here we can handle angles greater than pi.
   we must know the plan's vector, ie x and z, in order to know if the angle is greater or less than 2pi.
   the angle returned is the angle between z and vec
   better if all vectors are already normalized.
*/
static double angleBetweenVectorsWithReference(const Vector3D& vectorZ, const Vector3D& vectorX, const Vector3D& vec)
{                           
  // the Dot Product of returns the cosine of the angle between 2 vectors.
  // that is assuming they are unit vectors (normalize vectors).
  // So, if we don't have a unit vector, then instead of just saying  arcCos(DotProduct(A, B))
  // We need to divide the dot product by the magnitude of the 2 vectors multiplied by each other.
  // Here is the equation:   arc cosine of (V . W / || V || * || W || )
  // the || V || means the magnitude of V.  This then cancels out the magnitudes dot product magnitudes.
  // But basically, if you have normalize vectors already, you can forget about the magnitude part.
  
  // Get the dot product of the vectors
  float dotProduct = vectorZ * vec;

  // Get the product of both of the vectors magnitudes
  float vectorsMagnitude = vectorZ.Magnitude() * vec.Magnitude() ;
  
  // Get the arc cosine of the (dotProduct / vectorsMagnitude) which is the angle in RADIANS.
  // (IE.   PI/2 radians = 90 degrees      PI radians = 180 degrees    2*PI radians = 360 degrees)
  // To convert radians to degrees use this equation:   radians * (PI / 180)
  // TO convert degrees to radians use this equation:   degrees * (180 / PI)
  double angle = acos( dotProduct / vectorsMagnitude );
  
  // Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate.
  // acos() thinks it's funny when it returns -1.#IND0000000.  If we don't do this check,
  // our collision results will sometimes say we are colliding when we aren't. Usually
  // this value is found when the dot product and the magnitude are the same value.
  // We want to return 0 when this happens.
    if(isnan(angle))
        return 0;
    
    //if vec and x axis are not at the same side of z axis
    if ((vec.x * vectorX.x) < 0)
      angle = (2 * MATH_PI) - angle;

    // Return the angle in radians
    return( angle );
}


// returns the distance between a plane and the origin
// needs the normal of the plan and a point on the plane
static float planeDistanceFromOrigin (const Vector3D& normal, const Vector3D& point)
{   
  // Use the plane equation to find the distance (Ax + By + Cz + D = 0)  We want to find D.
  // So, we come up with D = -(Ax + By + Cz)
  return (- ((normal.x * point.x) + (normal.y * point.y) + (normal.z * point.z)));
}


//returns the distance between a point and a plane
static float getDistanceFromPlane (Vector3D point, Vector3D normal, float distancePlaneFromOrigin)
{
  // Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)
  
  return ((normal.x * point.x)  +                             // Ax +
	  (normal.y * point.y)  +                             // Bx +
	  (normal.z * point.z)) + distancePlaneFromOrigin;    // Cz + D 
}


// This functions checks the point to make sure it is inside of the polygon
bool static isInsidePoly (Vector3D poly [], int verticeCount, const Vector3D& pt) {

    const float MATCH_FACTOR = 0.9999;     // Used to cover up the error in floating point
    float angle = 0.0f;                     // Initialize the angle
    Vector3D vA, vB;                       // Create temp vectors
    
    // what we do is create triangles within the polygon
    // from the intersection point.  It then adds up the inner angle of each of those triangles.
    // If the angles together add up to 360 degrees (or 2 * PI in radians) then we are inside.
    // If the angle is under that value, we must be outside of polygon.

    for (int i = 0; i < verticeCount; i++)
    {   
        vA = (poly[i]) - pt;

        vB = (poly[(i + 1) % verticeCount]) - pt;
                                                
        angle += angleBetweenVectors(vA, vB);
    }

    // Now that we have the total angles added up, we need to check if they add up to 360 degrees.
    // Since we are using the dot product, we are working in radians, so we check if the angles
    // equals 2*PI. We use a MATCH_FACTOR
    // in conjunction with our desired degree.  This is because of the inaccuracy when working
    // with floating point numbers.  It usually won't always be perfectly 2 * PI, so we need
    // to use a little twiddling.
                                                
    if(angle >= (MATCH_FACTOR * (2.0 * MATH_PI)) )   // If the angle is greater than 2 PI, (360 degrees)
      return true;                            // The point is inside of the polygon
        
    return false;
}



//returns the opposite vector
static Vector3D opposite (const Vector3D& vec)
{
  return Vector3D (-(vec.x), -(vec.y), -(vec.z));
}


//convert radians to degrees
static float radToDeg (float rad) {
  return (180 * rad / MATH_PI);
}

#undef _11
#undef _12
#undef _13
#undef _14
#undef _21
#undef _22
#undef _23
#undef _24
#undef _31
#undef _32
#undef _33
#undef _34
#undef _41
#undef _42
#undef _43
#undef _44

