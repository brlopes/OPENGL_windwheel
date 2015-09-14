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
void move_camera(int specialKey,char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
float rad =0;
const float DEFAULT_SPEED   = 0.5f;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();

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
static void text_onScreen(int row, int col, const char *fmt, ...);
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
GLvoid DrawGround()
{
  // enable blending for anti-aliased lines
  glEnable(GL_BLEND);

  // set the color to a bright blue
  glColor3f(0.5f, 0.7f, 1.0f);

  // draw the lines
  glBegin(GL_LINES);
    for (int x = -WORLD_SIZE; x < WORLD_SIZE; x += 6)
    {
      glVertex3i(x, 0, -WORLD_SIZE);
      glVertex3i(x, 0, WORLD_SIZE);
    }

    for (int z = -WORLD_SIZE; z < WORLD_SIZE; z += 6)
    {
      glVertex3i(-WORLD_SIZE, 0, z);
      glVertex3i(WORLD_SIZE, 0, z);
    }
  glEnd();

  // turn blending off
  glDisable(GL_BLEND);
} // end DrawGround()
//=========================================================//
//=========================================================//
static void shapesPrintf (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    void *font = GLUT_BITMAP_9_BY_15;
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
/*
        glRasterPos2i(
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+2) + viewport[3]
        );
        glutBitmapString (font, (unsigned char *) buf);
*/
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//=========================================================//
static int function_index;
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
GLvoid clock3D(){

    int ViewPosX=0,ViewPosZ=-8;

    //glEnable(GL_LIGHTING);

    glColor3d(0,0,1);

   glPushMatrix();
   // draw blue basis
       //--------------------------------------------//
       glTranslatef (ViewPosX, 1, ViewPosZ);// Set Position Figure

       glRotatef ((GLfloat) rotateBase_degrees, 0.0, 0.0, 1.0); // rotate

       glTranslatef (0.0, -0.3, 0.0);//define central base of rotation

       glPushMatrix();
            table[1].solid ();
       glPopMatrix();
       //............................................//
       glPushMatrix();
       // draw red pointer
            glRotatef(rotatePointer_degrees,0,0,1);
            glTranslatef(0,1.5,0.2);

            glColor3d(1,0,0);

            glPushMatrix();
                glScalef(0.3,2,0.3);
                table[1].solid ();
            glPopMatrix();

        glPopMatrix();
       //............................................//
       //............................................//
       glPushMatrix();
       // draw yellow pointer
            glRotatef(rotatePointer_degrees*-1,0,0,1);
            glTranslatef(0,1.5,-0.2);

            glColor3d(1,1,0);

            glPushMatrix();
                glScalef(0.3,2,0.3);
                table[1].solid ();
            glPopMatrix();

        glPopMatrix();
       //............................................//

       //--------------------------------------------//
    // close the basis
    glPopMatrix();

    //glDisable(GL_LIGHTING);
   // glColor3d(0.1,0.1,0.4);

    shapesPrintf (1, 3, "Prof. Adriano Cavalcanti / Biomechanics / TUT");
    shapesPrintf (2, 3, " Press A: rotateBase ++  // B: rotateBase --");
    shapesPrintf (3, 3, " Press C: rotatePointer ++  // D: rotatePointer --");
}
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
    glRotatef(-90, 1.0, 0.0, 0.0);
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

    text_onScreen(0, 3, "Prof. Adriano Cavalcanti / A385 Computer Graphics / UAA");
    text_onScreen(2, 3, "- look up or down: A / Z");
    text_onScreen(3, 3, "- look right/left: arrows ->/<-");
    text_onScreen(4, 3, "- walk forward/backward: arrows UP/Down");

    DrawGround();

    glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{ int number=-1;

    move_camera(number,key);

    glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void move_camera(int specialKEY,char normalKEY)
{

    // clock input
    switch (specialKEY)
    {
    case GLUT_KEY_PAGE_UP:    ++function_index; break;
    case GLUT_KEY_PAGE_DOWN:  --function_index; break;
    case GLUT_KEY_UP:         orad *= 2;        break;
    case GLUT_KEY_DOWN:       orad /= 2;        break;

    case GLUT_KEY_RIGHT:      irad *= 2;        break;
    case GLUT_KEY_LEFT:       irad /= 2;        break;

    default:
        break;
    }
    if (0 > function_index)
        function_index = NUMBEROF (table) - 1;
    if (NUMBEROF (table) <= ( unsigned )function_index)
        function_index = 0;
    // keyboard :: normal keys
    switch(normalKEY)
    {
		case 'E':
		   rotateBase_degrees++;
		   break;
		case 'e':
		   rotateBase_degrees++;
		   break;
		case 'B':
		   rotateBase_degrees--;
		   break;
		case 'b':
		   rotateBase_degrees--;
		   break;
		case 'C':
		   rotatePointer_degrees++;
		   break;
		case 'c':
		   rotatePointer_degrees++;
		   break;
		case 'D':
		   rotatePointer_degrees--;
		   break;
		case 'd':
		   rotatePointer_degrees--;
		   break;
		default:
		{   break;
		}
	}
    // end of clock input


    // keyboard :: normal keys
    switch(normalKEY)
    {   // looking up
        case 'A':
        case 'a':    g_elevationAngle += 2.0; break;
       // looking down
        case 'Z':
        case 'z':g_elevationAngle -= 2.0;  break;

        default:
        {    break;
        }
    }

    // special :: special keys
    switch(specialKEY)
    {   // camera setup
        // check if it is moving the view to look left
        case GLUT_KEY_LEFT:
        {
            g_viewAngle -= 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
            break;
        }
        // check if it is moving the view to look right
        case GLUT_KEY_RIGHT:
        {
             g_viewAngle += 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
            break;
        }
        // pressing keys Up/Down, update coordinates "x" and "z"
        // based on speed and angle of view.
        case GLUT_KEY_UP:
        {
            g_playerPos[2] += sin(rad) * DEFAULT_SPEED;
            g_playerPos[0] += cos(rad) * DEFAULT_SPEED;
            break;
        }
        case GLUT_KEY_DOWN:
        {
            g_playerPos[2] -= sin(rad) * DEFAULT_SPEED;
            g_playerPos[0] -= cos(rad) * DEFAULT_SPEED;
            break;
        }
        default:
        {   break;
        }
    }
} // End move_camera()
//=========================================================//
//=========================================================//
static void special(int key, int x, int y)
{   char letter=' ';

    move_camera(key,letter);

    glutPostRedisplay();
}

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

    glutCreateWindow("A385 Computer Graphics");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
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
//======================================================,===//
