// LINUX / Mac OSX or UNIX ONLY
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// person position in the environment
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 6.0;
float rad =0;
const float DEFAULT_SPEED   = 0.5f;
//=========================================================//
//=========================================================//

GLvoid  DrawNormalObjects(GLfloat rotation);

//=========================================================//
//=========================================================//
// quadric objects
void init_data(void);
void setup_sceneEffects(void);
GLUquadricObj *g_normalObject     = NULL;
GLUquadricObj *g_wireframeObject  = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanUP_data(void);

const int   WORLD_SIZE = 100;
//=========================================================//
//=========================================================//
//=========================================================//
//=========================================================//
typedef struct
{
    const char * const name;
    void (*solid) (void);
    void (*wire)  (void);
} entry;

//=========================================================//
//=========================================================//
static void text_onScreen (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    //void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0,viewport[2],0,viewport[3],-1,1);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//=========================================================//
//=========================================================//
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( ar > .5 )
        glFrustum( -ar, ar, -1.0, 1.0, 2.0, 100.0 );
    else
        glFrustum( -1.0, 1.0, -1/ar, 1/ar, 2.0, 100.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

//=========================================================//
//=========================================================//
static int slices = 16;
static int stacks = 16;
static double irad = .25;
static double orad = 1.0;
/*
 * This macro is only intended to be used on arrays, of course.
 */
#define NUMBEROF(x) ((sizeof(x))/(sizeof(x[0])))
//=========================================================================//
/*
 * These one-liners draw particular objects, fetching appropriate
 * information from the above globals.  They are just thin wrappers
 * for the OpenGLUT objects.
 */
static void drawSolidTetrahedron(void)
    { glutSolidTetrahedron (); }
static void drawWireTetrahedron(void)
    { glutWireTetrahedron (); }
static void drawSolidCube(void)
    { glutSolidCube(1); }
static void drawWireCube(void)
    { glutWireCube(1); }
static void drawSolidOctahedron(void)
    { glutSolidOctahedron (); }
static void drawWireOctahedron(void)
    { glutWireOctahedron (); }
static void drawSolidDodecahedron(void)
    { glutSolidDodecahedron (); }
static void drawWireDodecahedron(void)
    { glutWireDodecahedron (); }
/*
static void drawSolidRhombicDodecahedron(void)
    { glutSolidRhombicDodecahedron (); }
static void drawWireRhombicDodecahedron(void)
    { glutWireRhombicDodecahedron (); }
*/
static void drawSolidIcosahedron(void)
    { glutSolidIcosahedron (); }
static void drawWireIcosahedron(void)
    { glutWireIcosahedron (); }
/*
static void drawSolidSierpinskiSponge(void)
    { glutSolidSierpinskiSponge (depth, offset, 1); }
static void drawWireSierpinskiSponge(void)
    { glutWireSierpinskiSponge (depth, offset, 1); }
*/
static void drawSolidTeapot(void)
    { glutSolidTeapot(1); }
static void drawWireTeapot(void)
    { glutWireTeapot(1); }
static void drawSolidTorus(void)
    { glutSolidTorus(irad,orad,slices,stacks); }
static void drawWireTorus(void)
    { glutWireTorus (irad,orad,slices,stacks); }
static void drawSolidSphere(void)
    { glutSolidSphere(1,slices,stacks); }
static void drawWireSphere(void)
    { glutWireSphere(1,slices,stacks); }
static void drawSolidCone(void)
    { glutSolidCone(1,1,slices,stacks); }
static void drawWireCone(void)
    { glutWireCone(1,1,slices,stacks); }
/*
static void drawSolidCylinder(void)
    { glutSolidCylinder(1,1,slices,stacks); }
static void drawWireCylinder(void)
    { glutWireCylinder(1,1,slices,stacks); }
*/
//=========================================================================//
float rotateBase_degrees=0;
float rotatePointer_degrees=0;
#define ENTRY(e) {#e, drawSolid##e, drawWire##e}
static const entry table [] =
{
    ENTRY (Tetrahedron),
    ENTRY (Cube),
    ENTRY (Octahedron),
    ENTRY (Dodecahedron),
    //ENTRY (RhombicDodecahedron),
    ENTRY (Icosahedron),
    //ENTRY (SierpinskiSponge),
    ENTRY (Teapot),
    ENTRY (Torus),
    ENTRY (Sphere),
    ENTRY (Cone)//,
    //ENTRY (Cylinder)
};
#undef ENTRY

//=========================================================//
//=========================================================//
GLvoid DrawNormalObjects(GLfloat rotation)
{
  // make sure the random color values we get are the same every time
  srand(200);

  // save the existing color properties
  glPushAttrib(GL_CURRENT_BIT);

    //clock3D();

  //THIS IS IT
  // a red disk with a hole in it
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(1,0,0);
    glTranslatef(0, 6, -7);
    glRotatef(rotation * 6.0f, 0.0, 0.0, 1.0);
    gluDisk(g_normalObject, 1.5f, 4.0f, 6, 1);
  glPopMatrix();

  //green
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,1,0);
    glTranslatef(0, 5, -6);
    glRotatef(rotation * -5.0f, 0.0, 0.0, 1.0);
    gluDisk(g_normalObject, 1.0f, 3.0f, 6, 1);
  glPopMatrix();

  //blue
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,0,1);
    glTranslatef(0, 5, -5);
    glRotatef(rotation * 3.0f, 0.0, 0.0, 1.0);
    gluDisk(g_normalObject, 0.5f, 2.0f, 6, 1);
  glPopMatrix();

  // windwheel stem
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(1,1,1);
    glTranslatef(0, 0, -8);
    glRotatef(-90, 1.0, 0.0, rotateBase_degrees);
    gluCylinder(g_flatshadedObject, 0.5, 0.5, 6.0, 32, 4);
  glPopMatrix();


  // restore the previous color values
  glPopAttrib();
} // end DrawNormalObjects()
//=========================================================//
//=========================================================//

//=========================================================//
//=========================================================//
static void display(void)
{
    update_camera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);

    // position the light
    GLfloat pos[4] = { 5.0, 5.0, 5.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // rotation is used for animation
    static GLfloat rotation = 0.0;
    // it's increased by one every frame
    rotation += 1.0;
    // and ranges between 0 and 360
    if (rotation > 360.0)
    rotation = 0.0;
    // draw all of our objects in their normal position
    DrawNormalObjects(rotation);

    //glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);

    glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{
	//rotation, absolutely bbbbbroken rn

	switch (key)
	   {
	      case 'l':
	          rotateBase_degrees++;
	          break;
	      case 'L':
	          rotateBase_degrees++;
	          break;
	      case 'r':
	    	  rotateBase_degrees--;
	    	  break;
	      case 'R':
	    	  rotateBase_degrees--;
	    	  break;
	      case '1':
	    	  // wheelSpeed = 10
	    	  break;
	      case '2':
	    	  // wheelSpeed = 50
	    	  break;
	      case '3':
	    	  // wheelSpeed = 200
	    	  break;
	      default:
	          break;
	   }


    glutPostRedisplay();
}
//=========================================================//
//=========================================================//

//=========================================================//
static void idle(void)
{
    glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void update_camera()
{
// don't allow the player to wander past the "edge of the world"
    if (g_playerPos[0] < -(WORLD_SIZE-50))
    g_playerPos[0] = -(WORLD_SIZE-50);
    if (g_playerPos[0] > (WORLD_SIZE-50))
    g_playerPos[0] = (WORLD_SIZE-50);
    if (g_playerPos[2] < -(WORLD_SIZE-50))
    g_playerPos[2] = -(WORLD_SIZE-50);
    if (g_playerPos[2] > (WORLD_SIZE-50))
    g_playerPos[2] = (WORLD_SIZE-50);

  // calculate the player's angle of rotation in radians
    float rad =  float(3.14159 * g_viewAngle / 180.0f);
    // use the players view angle to correctly set up the view matrix
    g_lookAt[0] = float(g_playerPos[0] + 100*cos(rad));
    g_lookAt[2] = float(g_playerPos[2] + 100*sin(rad));

    rad = float (3.13149 * g_elevationAngle / 180.0f);

    g_lookAt[1] = float (g_playerPos[1] + 100 * sin(rad));

    // clear the modelview matrix
    glLoadIdentity();

    // setup the view matrix
    gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
              g_lookAt[0],    g_lookAt[1],    g_lookAt[2],
              0.0,            1.0,            0.0);
}
//=========================================================//
//=========================================================//
void setup_sceneEffects(void)
{
  // enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // enable using glColor to change material properties
  // we'll use the default glColorMaterial setting (ambient and diffuse)
  glEnable(GL_COLOR_MATERIAL);

  // set the default blending function
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // set up the fog parameters for reflections
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, -100.0);
  glFogf(GL_FOG_END, 100.0);

  // enable line anti-aliasing and make the lines slightly bigger than default
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.5f);
}
//=========================================================//
//=========================================================//
void init_data(void)
{
  setup_sceneEffects();
  // create a normal quadric (uses default settings)
  g_normalObject = gluNewQuadric();

  g_wireframeObject = gluNewQuadric();
  gluQuadricDrawStyle(g_wireframeObject, GLU_LINE);

  // create an object that uses flat shading
  g_flatshadedObject = gluNewQuadric();
  gluQuadricNormals(g_flatshadedObject, GLU_FLAT);

}
//=========================================================//
//=========================================================//
void cleanUP_data(void)
{  // delete all quadratic objects
  if (g_normalObject)
    gluDeleteQuadric(g_normalObject);
  if (g_wireframeObject)
    gluDeleteQuadric(g_wireframeObject);
  if (g_flatshadedObject)
    gluDeleteQuadric(g_flatshadedObject);
}
//=========================================================//
//=========================================================//
int main(int argc, char *argv[])
{
    glutInitWindowSize(640,480);
    glutInitWindowPosition(40,40);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    init_data();

    glutCreateWindow("Wind Wheel");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    // environment background color
    glClearColor(0.9,0.9,0.7,1);//(1,1,1,1);
    // deepth efect to objects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // light and material in the environment
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    cleanUP_data();

    return EXIT_SUCCESS;
}
//=========================================================//
//=========================================================//
