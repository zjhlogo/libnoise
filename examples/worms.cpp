// worms.cpp
//
// This program uses the libnoise library to draw and animate worm-like
// creatures.  On a fast computer (P4/AMD Athlon-class), hundreds of worms
// can be drawn and animated at very high frame rates.
//
// The worms do not interact with each other because that would use way too
// much CPU resources.
//
// See the documentation for the Worm class for information on how
// coherent-noise values are used to draw and animate a worm.
//
// Requires OpenGL 1.1+ and GLUT (a windowing toolkit for OpenGL.)  The GLUT
// webpage is http://www.opengl.org/resources/libraries/glut.html
//
// Keyboard controls:
//
// Esc: Exits the program.
//
// Q: Increase the number of worms by one.
// A: Decrease the number of worms by one.
//
// W: Increase the number of worm segments by one.
// S: Increase the number of worm segments by one.
//
// E: Increase the worms' speed.
// D: Decrease the worms' speed.
//
// R: Increase the lateral (thrashing) speed of the worms.
// F: Decrease the lateral (thrashing) speed of the worms.
//
// T: Increase the worms' thickness.
// G: Decrease the worms' thickness.
//
// Y: Increase the worms' "twistiness".
// H: Decrease the worms' "twistiness".
//
// Copyright (C) 2004, 2005 Jason Bevins
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// (COPYING.txt) for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>
#include <iostream>
#include <fstream>

#include <gl/gl.h>
#include <glut.h>

#include <noise/noise.h>
#include <noise/mathconsts.h>

using namespace std;

using namespace noise;

// Width of the worm texture.
const int g_textureWidth = 16;

// Height of the worm texture.
const int g_textureHeight = 16;

// 16x16 RGB texture for the worms.  Generated by The GIMP.
static char* g_pTextureData =
  "?VE`8U)K13Y:1C];2$%:5DQA>'&!@WB*:UQR9EET9%ES8UAP9UMZ>&%[A6Z$>6-Z"
  "S[6XR[*PIH2\"IXN0HY2>DWR#VL;+TL3,LYZ<QZVNUL\"_L)..NJJXN:>[V<#)R;\"Y"
  "W\\G,Y-/4P9^3SZ^KZ-K>RJN?S:VMY=/4U[NPW<+![-GCV<\"UX,C&Y-7BZ=K>V\\2^"
  "W\\F^XL[!R*.6UK:PYM7/RZB:U[R^V;^POY:)SZ^:Y]#/Y=')SZ^:Y]#/Y=')UKBH"
  "OY*%O9*%K()WPYF+Q)Z+JGUVU[RUO9.\"HW=MN9)\\R)N)MXQVM9!WRIN+QIJ+L8-W"
  "GG9FJGYNIGMMJ'YMK()PHWIKN9J+HX)GGGI?L8YQL8US@653F7EBJGYJJGUIEVU@"
  "GGY:F7E9E'18G'I9G'M:E'1;CW->C')6AVE3E'==F7ED<UQ5@6M4EWE5F7Y6DG92"
  "C'!6AVI5@613AVQ5B6U5@6=6?FA8?F96>V)5@6E6AVY<:UE6:UM1>V90@6E0@6A0"
  "<U]7=E]7<UE6<%M7=F!7<%M5:5=4:5M7;EI6:UM2;EY29%138E53:UA1:5A0<%]2"
  "9%E6<%U:;EE;;EA::UE79E548E%39%15:5957U)29E=58E-69EA6<%Y6<%Y6:5E5"
  "6U)3:UY<=F5A<U]?=F1>:UI:9EA7:5E78E158E-6;EQ89E=69%54:5M6<V189%E6"
  "4$I/7556;F->;F!?;EY>:UI=;EY=<&%=9%588E-59%55<&!;:5A96TY25DY/5D]0"
  "/SQ*03],1D-/2$523$)44$-65DI76TY86TQ674Y674Y49EA::5E;64Y2/SQ(/CQ("
  ",#%'+C!',#))-#-,-S--.3-./#9//#9./#1-0SM02D%013U,/SA*-S-(+BY%,3!&"
  "(R-%&Q]$&A]%'B)'(R1()\"5((B-(*2=(*\"9)+\"E)+2M))21%(\"%#'!Y#(\"%$(B)%"
  "\"A)\"\"A)\"\"1-\"\"A-\"\"A1\"\"A-\"\"Q-\"#!5\"#A5\"#A5\"#Q9\"$AA"
  "\"&QU$%QM#\"Q-\"\"A-\""
  "";

// This macro, provided by The GIMP, extracts the pixel values from the
// texture defined above.
#define HEADER_PIXEL(data,pixel) { \
  pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
  pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
  pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
  data += 4; \
}

// Default worm lateral speed.
const double WORM_LATERAL_SPEED = (2.0 / 8192.0);

// Default length of a worm segment, in screen units.
const double WORM_SEGMENT_LENGTH = (1.0 / 64.0);

// Default segment count for each worm.
const int WORM_SEGMENT_COUNT = 112;

// Default worm speed.
const double WORM_SPEED = (3.0 / 2048.0);

// Default worm thickness.
const double WORM_THICKNESS = (4.0 / 256.0);

// Default "twistiness" of the worms.
const double WORM_TWISTINESS = (4.0 / 256.0);

// Default worm lateral speed.
double g_wormLateralSpeed = WORM_LATERAL_SPEED;

// Default length of a worm segment, in screen units.
double g_wormSegmentLength = WORM_SEGMENT_LENGTH;

// Default segment count for each worm.
int g_wormSegmentCount = WORM_SEGMENT_COUNT;

// Default worm speed.
double g_wormSpeed = WORM_SPEED;

// Default worm thickness.
double g_wormThickness = WORM_THICKNESS;

// Default "twistiness" of the worms.
double g_wormTwistiness = WORM_TWISTINESS;

// Number of worms rendered on the screen.
int g_curWormCount = 32;

// This template function clamps the specified value to the specified bounds.
template<class T>
void Clamp (T& value, T lowerBound, T upperBound)
{
  if (value < lowerBound) {
    value = lowerBound;
  } else if (value > upperBound) {
    value = upperBound;
  }
}

// A standard 2-component vector.
class Vector2
{

  public:

    double x, y;

};

// A standard 3-component vector.
class Vector3
{

  public:

    double x, y, z;

};

// Worm class
//
// This class uses three-dimensional coherent noise to draw and animate a
// single worm-like creature.
//
// A worm is made up of a chain of "segments".  The first segment is known as
// the "head" segment.  This class draws the worm starting from the head
// segment.
//
// A worm object contains a three-octave Perlin-noise module, which is used to
// specify the angles of each worm segment, in radians.  Three octaves is a
// good compromise between realism and speed.
//
// The coherent-noise values that specify the segment angles are generated by
// the input values that occur along a horizontal line in "noise space".  By
// shifting the (x, y, z) coordinates of the input value by a small constant
// amount each frame, the worm will slightly move.
//
// To draw the worm, call the Draw() method.
//
// To move the worm, call the Update() method.  That method moves the position
// of the head segment a small distance in the direction opposite its angle.
// Also, the x coordinate of the input value is shifted in a negative
// direction, which propagates the previous coherent-noise values over to
// subsequent segments.  This will produce a "slithering" effect.
//
// Each worm should have its own seed value because worms with the same seed
// value will act identically.
class Worm
{

  public:

    Worm ()
    {
      // The coordinates of the input value for the head segment must not
      // start at an integer boundary (for example, (0, 0, 0)).  At integer
      // boundaries, the coherent-noise values are always zero (blame gradient
      // noise for that), which would cause the worm to unrealistically
      // straighten those boundaries.
      m_headNoisePos.x =    7.0 / 2048.0;
      m_headNoisePos.y = 1163.0 / 2048.0;
      m_headNoisePos.z =  409.0 / 2048.0;

      // Set up us the Perlin-noise module.
      m_noise.SetSeed (0);
      m_noise.SetFrequency (1.0);
      m_noise.SetLacunarity (2.375);
      m_noise.SetOctaveCount (3);
      m_noise.SetPersistence (0.5);
      m_noise.SetNoiseQuality (noise::QUALITY_STD);

      // Set the worm parameters with their default values.
      m_headScreenPos.x = 0.0;
      m_headScreenPos.y = 0.0;
      m_lateralSpeed = WORM_LATERAL_SPEED;
      m_segmentCount = WORM_SEGMENT_COUNT;
      m_segmentLength = WORM_SEGMENT_LENGTH;
      m_speed = WORM_SPEED;
      m_thickness = WORM_THICKNESS;
      m_twistiness = WORM_TWISTINESS;
    }

    // Draw the worm using OpenGL.
    void Draw () const;

    // Returns the taper amount for a specified segment.  A taper value of 0.0
    // indicates full tapering, while a taper value of 1.0 indicates no
    // tapering.  Taper values are at a minimum at both ends of the worm and
    // are at a maximum at the middle of the worm.
    double GetTaperAmount (int segment) const
    {
      double curSegment = (double)segment;
      double segmentCount = (double)m_segmentCount;
      double halfSegmentCount = segmentCount / 2.0;
      double baseTaperAmount
        = 1.0 - fabs ((curSegment / halfSegmentCount) - 1.0);
      return sqrt (baseTaperAmount); // sqrt better defines the tapering.
    }

    // Sets the position of the worm's head segment in screen space.
    void SetHeadScreenPos (const Vector2& pos)
    {
      m_headScreenPos = pos;
    }

    // Sets the worm's lateral speed.  This is the amount the worm "thrashes
    // around" between frames.  Higher values increases the thrashing amount.
    void SetLateralSpeed (double lateralSpeed)
    {
      m_lateralSpeed = lateralSpeed;
    }

    // Sets the seed of the Perlin-noise module.
    void SetSeed (int seed)
    {
      m_noise.SetSeed (seed);
    }

    // Sets the number of segments that make up the worm.
    void SetSegmentCount (double segmentCount)
    {
      m_segmentCount = segmentCount;
    }

    // Sets the length of a worm segment, in screen units.
    void SetSegmentLength (double segmentLength)
    {
      m_segmentLength = segmentLength;
    }

    // Sets the worm's speed.  Higher values increase the speed.
    void SetSpeed (double speed)
    {
      m_speed = speed;
    }

    // Sets the worm's thickness, in screen units.
    void SetThickness (double thickness)
    {
      m_thickness = thickness;
    }

    // Defines the "twistiness" of the worms.  Higher values produce more
    // contorted worms.
    void SetTwistiness (double twistiness)
    {
      m_twistiness = twistiness;
    }

    // Updates the worm's segment positions.  This must be called after each
    // frame.
    void Update ();

  private:

    // Coordinates of the input value that generates the Perlin noise in
    // "noise space".  This is used to specify the angles of the worm's
    // segments.
    Vector3 m_headNoisePos;

    // Position of the worm's head segment, in screen space.
    Vector2 m_headScreenPos;

    // Worm's lateral speed.
    double m_lateralSpeed;

    // Noise module used to draw the worm.
    module::Perlin m_noise;

    // Number of segments that make up the worm.
    int m_segmentCount;

    // Length of a worm segment.
    double m_segmentLength;

    // Worm speed.
    double m_speed;

    // Worm thickness.
    double m_thickness;

    // "Twistiness" of the worm.
    double m_twistiness;

};

void Worm::Draw () const
{
  // The worm is drawn using a triangle strip.
  glBegin (GL_TRIANGLE_STRIP);

  // Position of the current segment being drawn, in screen space.
  Vector2 curSegmentScreenPos = m_headScreenPos;

  // The width of the worm's body at the current segment being drawn.
  Vector2 offsetPos;

  // Coordinates of the input value, in "noise space", that specifies the
  // current segment's angle.
  Vector3 curNoisePos;

  // The vector that is perpindicular to the center of the segment; used to
  // determine the position of the edges of the worm's body.
  Vector2 curNormalPos;

  for (int curSegment = 0; curSegment < m_segmentCount; curSegment++) {

    // Get the Perlin-noise value for this segment based on the segment
    // number.  This value is interpreted as an angle, in radians.
    curNoisePos.x = m_headNoisePos.x + (curSegment * m_twistiness);
    curNoisePos.y = m_headNoisePos.y;
    curNoisePos.z = m_headNoisePos.z;
    double noiseValue = m_noise.getValue (
      curNoisePos.x,
      curNoisePos.y,
      curNoisePos.z);

    // Determine the width of the worm's body at this segment.
    double taperAmount = GetTaperAmount (curSegment) * m_thickness;

    // Determine the offset of this segment from the previous segment by
    // converting the angle from the Perlin-noise module to an (x, y)
    // coordinate.
    offsetPos.x = cos (noiseValue * 2.0 * noise::PI);
    offsetPos.y = sin (noiseValue * 2.0 * noise::PI);

    // Determine the coordinates of each corner of the segment.
    curNormalPos.x = (-offsetPos.y) * taperAmount;
    curNormalPos.y = ( offsetPos.x) * taperAmount;
    offsetPos.x *= m_segmentLength;
    offsetPos.y *= m_segmentLength;
    GLdouble x0 = (GLdouble)(curSegmentScreenPos.x + curNormalPos.x);
    GLdouble y0 = (GLdouble)(curSegmentScreenPos.y + curNormalPos.y);
    GLdouble x1 = (GLdouble)(curSegmentScreenPos.x - curNormalPos.x);
    GLdouble y1 = (GLdouble)(curSegmentScreenPos.y - curNormalPos.y);

    // Draw the segment using OpenGL.
    glTexCoord2f ((GLfloat)curSegment, 0.0f);
    glVertex2d (x0, y0);
    glTexCoord2f ((GLfloat)curSegment, 1.0f);
    glVertex2d (x1, y1);

    // Prepare the next segment.
    ++curSegment;
    curSegmentScreenPos.x += offsetPos.x;
    curSegmentScreenPos.y += offsetPos.y;
  }

  // Finish drawing the worm.
  glEnd ();
}

void Worm::Update ()
{
  // The angle of the head segment is used to determine the direction the worm
  // moves.  The worm moves in the opposite direction.
  double noiseValue = m_noise.getValue (
    m_headNoisePos.x,
    m_headNoisePos.y,
    m_headNoisePos.z);
  m_headScreenPos.x -= (cos (noiseValue * 2.0 * noise::PI) * m_speed);
  m_headScreenPos.y -= (sin (noiseValue * 2.0 * noise::PI) * m_speed);

  // Slightly update the coordinates of the input value, in "noise space".
  // This causes the worm's shape to be slightly different in the next frame.
  // The x coordinate of the input value is shifted in a negative direction,
  // which propagates the previous Perlin-noise values over to subsequent
  // segments.  This produces a "slithering" effect.
  m_headNoisePos.x -= m_speed * 2.0;
  m_headNoisePos.y += m_lateralSpeed;
  m_headNoisePos.z += m_lateralSpeed;

  // Make sure the worm's head is within the window, otherwise the worm may
  // escape.  Horrible, horrible freedom!
  Clamp (m_headScreenPos.x, -1.0, 1.0);
  Clamp (m_headScreenPos.y, -1.0, 1.0);
}

// Array used to store a bunch of worms.
const int MAX_WORM_COUNT = 1024;
Worm g_wormArray[MAX_WORM_COUNT];

// This function is called by GLUT when the program is idle.
void IdleGLUT ()
{
  glutPostRedisplay ();
}

// This function is called by GLUT when the program window is resized.
void ReshapeGLUT (int w, int h)
{
  glViewport (0, 0, (GLsizei)w, (GLsizei)h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  // No need for perspective since the worms are rendered in 2D.
  double aspect = (double)h / (double)w;
  glOrtho (-1.0f, 1.0f, -1.0f * aspect, 1.0f * aspect, 0.0f, 1.0f);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

// This function is called by GLUT when a key is pressed.
void KeyboardGLUT (unsigned char key, int x, int y)
{
  switch (key) {
    case 'Q':
    case 'q':
      ++g_curWormCount;
      break;
    case 'A':
    case 'a':
      --g_curWormCount;
      break;
    case 'W':
    case 'w':
      ++g_wormSegmentCount;
      break;
    case 'S':
    case 's':
      --g_wormSegmentCount;
      break;
    case 'E':
    case 'e':
      g_wormSpeed += (1.0 / 2048.0);
      break;
    case 'D':
    case 'd':
      g_wormSpeed -= (1.0 / 2048.0);
      break;
    case 'R':
    case 'r':
      g_wormLateralSpeed += (1.0 / 8192.0);
      break;
    case 'F':
    case 'f':
      g_wormLateralSpeed -= (1.0 / 8192.0);
      break;
    case 'T':
    case 't':
      g_wormThickness += (1.0 / 256.0);
      break;
    case 'G':
    case 'g':
      g_wormThickness -= (1.0 / 256.0);
      break;
    case 'Y':
    case 'y':
      g_wormTwistiness += (1.0 / 256.0);
      break;
    case 'H':
    case 'h':
      g_wormTwistiness -= (1.0 / 256.0);
      break;
    case 27:
      exit (0);
  }

  // Make sure the worm parameters are legal.  If they are not, set them to
  // the nearest legal value.
  Clamp (g_curWormCount, 1, 1024);
  Clamp (g_wormSegmentCount, 1, 256);
  Clamp (g_wormSpeed, (1.0 / 2048.0), (1024.0 / 2048.0));
  Clamp (g_wormLateralSpeed, (1.0 / 8192.0), (64.0 / 8192.0));
  Clamp (g_wormThickness, (1.0 / 256.0), (16.0 / 256.0));
  Clamp (g_wormTwistiness, (1.0 / 256.0), (16.0 / 256.0));

  // Set the parameters for each worm.
  for (int i = 0; i < g_curWormCount; i++) {
    g_wormArray[i].SetSegmentCount (g_wormSegmentCount);
    g_wormArray[i].SetSpeed (g_wormSpeed);
    g_wormArray[i].SetLateralSpeed (g_wormLateralSpeed);
    g_wormArray[i].SetThickness (g_wormThickness);
    g_wormArray[i].SetTwistiness (g_wormTwistiness);
  }
}

// This function is called by GLUT when the user moves the mouse or presses
// any of its buttons.
void MouseGLUT (int button, int state, int x, int y)
{
  static int xMouseCenter = 0;
  static int yMouseCenter = 0;

  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        break;
      case GLUT_RIGHT_BUTTON:
        break;
      default:
        break;
    }
  } else if (state == GLUT_UP) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        break;
      case GLUT_RIGHT_BUTTON:
        break;
      default:
        break;
    }
  }
}

// This function is called by GLUT when the window needs to be redrawn.
void DisplayGLUT ()
{
  glClear (GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix ();
  glLoadIdentity ();

  // Draw all the worms.
  for (int i = 0; i < g_curWormCount; i++) {
    g_wormArray[i].Draw ();
    g_wormArray[i].Update ();
  }

  // Show the frame count in the console window.
  static int frame = 0;
  frame++;
  if ((frame % 10) == 0) {
    std::cout << frame << std::endl;
  }

  glPopMatrix ();
  glFlush ();
  glutSwapBuffers ();
}

int main (int argc, char** argv)
{
  for (int i = 0; i < MAX_WORM_COUNT; i++) {
    Vector2 pos;
    pos.x = noise::ValueNoise3D (i + 1000, i + 2000, i + 3000);
    pos.y = noise::ValueNoise3D (i + 1001, i + 2001, i + 3001);
    g_wormArray[i].SetSeed (i);
    g_wormArray[i].SetHeadScreenPos (pos);
  }

  // Set up us the GLUT (OpenGL windowing toolkit).
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (640, 640);
  glutInitWindowPosition (0, 0);
  glutCreateWindow ("libnoise demonstration - Perlin worms");

  // Set up us the GLUT function callbacks.
  glutDisplayFunc (DisplayGLUT);
  glutReshapeFunc (ReshapeGLUT);
  glutMouseFunc (MouseGLUT);
  glutKeyboardFunc (KeyboardGLUT);
  glutIdleFunc (IdleGLUT);

  // Set the OpenGL texture mapping and blending parameters for this program.
  glClearColor (0.1f, 0.15f, 0.3f, 1.0f);
  glShadeModel (GL_SMOOTH);
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_CULL_FACE);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_TEXTURE_2D);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Build the texture map to use for the worms.
  unsigned char pTexture[g_textureWidth * g_textureHeight * 4];
  unsigned char* pSource = (unsigned char*)g_pTextureData;
  for (int y = 0; y < g_textureHeight; y++) {
    unsigned char* pDest = &(pTexture[g_textureWidth * 4 * y]);
    for (int x = 0; x < g_textureWidth; x++) {
      HEADER_PIXEL (pSource, pDest); // Decode the pixel value.
      // Set the value of the alpha channel for this pixel.  The top and
      // bottom rows are somewhat transparent.  Combined with mipmapping,
      // it's almost like getting antialiasing for free.
      if (y == 0 || y == g_textureHeight - 1) {
        pDest[3] = 0x40;
      } else {
        pDest[3] = 0xff;
      }
      pDest += 4;
    }
  }

  // Upload the texture to the video card and build mipmaps.
  GLuint textureID = gluBuild2DMipmaps (GL_TEXTURE_2D,
    4, // bits per channel
    (GLint)g_textureWidth, (GLint)g_textureHeight, // dimensions
    GL_RGBA, // texture format
    GL_UNSIGNED_BYTE,
    pTexture);
  glBindTexture (GL_TEXTURE_2D, textureID);

  // Go farkers go!
  glutMainLoop ();

  return 0;
}
