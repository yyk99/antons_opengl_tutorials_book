/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Dr Anton Gerdelan, Trinity College Dublin, Ireland.                          |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| "Hello Triangle". Just the basics.                                           |
| If you're on Apple un-comment the version number code at the beginning. It   |
| will give you the latest, even if you say 3.2!                               |
| This uses the libraries GLEW and GLFW3 to start GL. Download and compile     |
| these first. Linking them might be a pain, but you'll need to master this.   |
\******************************************************************************/

#include <GL/glew.h>    /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */
#include <stdio.h>

int main() {
  GLFWwindow* window = NULL;
  const GLubyte* renderer;
  const GLubyte* version;
  GLuint vao;
  GLuint vbo;

  /* geometry to use. these are 3 xyz points (9 floats total) to make a triangle */
  GLfloat points[] = { 
      0.0f, 0.5f, 0.0f, 
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
  };

  /* these are the strings of code for the shaders
  the vertex shader positions each vertex point */
  const char* vertex_shader =
    "#version 410\n"
    "in vec3 vp;"
    "void main () {"
    "  gl_Position = vec4(vp, 1.0);"
    "}";

  /* the fragment shader colours each fragment (pixel-sized area of the
  triangle) */
  const char* fragment_shader =
    "#version 410\n"
    "out vec4 frag_colour;"
    "void main () {"
    "  frag_colour = vec4(0.5, 0.0, 0.0, 1.0);"
    "}";

  /* GL shader objects for vertex and fragment shader [components] */
  GLuint vert_shader, frag_shader;
  /* GL shader programme object [combined, to link] */
  GLuint shader_programme;

  /* start GL context and O/S window using the GLFW helper library */
  if ( !glfwInit() ) {
    fprintf( stderr, "ERROR: could not start GLFW3\n" );
    return 1;
  }

  /* Version 4.1 Core is a good default that should run on just about everything. Adjust later to suit project requirements. */
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  window = glfwCreateWindow( 640, 480, "Hello Triangle", NULL, NULL );
  if ( !window ) {
    fprintf( stderr, "ERROR: could not open window with GLFW3\n" );
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent( window );

  /* start GLEW extension handler */
  glewExperimental = GL_TRUE;
  glewInit();

  /* get version info */
  renderer = glGetString( GL_RENDERER ); /* get renderer string */
  version  = glGetString( GL_VERSION );  /* version as a string */
  printf( "Renderer: %s\n", renderer );
  printf( "OpenGL version supported %s\n", version );

  /* tell GL to only draw onto a pixel if the shape is closer to the viewer
  than anything already drawn at that pixel */
  glEnable( GL_DEPTH_TEST ); /* enable depth-testing */
  /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
  glDepthFunc( GL_LESS );

  /* a vertex buffer object (VBO) is created here. this stores an array of
  data on the graphics adapter's memory. in our case - the vertex points */
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( GLfloat ), points, GL_STATIC_DRAW );

  /* the vertex array object (VAO) is a little descriptor that defines which
  data from vertex buffer objects should be used as input variables to vertex
  shaders. in our case - use our only VBO, and say 'every three floats is a
  variable' */
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  /* "attribute #0 should be enabled when this vao is bound" */
  glEnableVertexAttribArray( 0 );
  /* this VBO is already bound, but it's a good habit to explicitly specify which
  VBO's data the following vertex attribute pointer refers to */
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  /* "attribute #0 is created from every 3 variables in the above buffer, of type
  float (i.e. make me vec3s)" */
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

  /* here we copy the shader strings into GL shaders, and compile them. we
  then create an executable shader 'program' and attach both of the compiled
      shaders. we link this, which matches the outputs of the vertex shader to
  the inputs of the fragment shader, etc. and it is then ready to use */
  vert_shader = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vert_shader, 1, &vertex_shader, NULL );
  glCompileShader( vert_shader );
  frag_shader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( frag_shader, 1, &fragment_shader, NULL );
  glCompileShader( frag_shader );
  shader_programme = glCreateProgram();
  glAttachShader( shader_programme, frag_shader );
  glAttachShader( shader_programme, vert_shader );
  glLinkProgram( shader_programme );

  /* this loop clears the drawing surface, then draws the geometry described
      by the VAO onto the drawing surface. we 'poll events' to see if the window
  was closed, etc. finally, we 'swap the buffers' which displays our drawing
      surface onto the view area. we use a double-buffering system which means
      that we have a 'currently displayed' surface, and 'currently being drawn'
      surface. hence the 'swap' idea. in a single-buffering system we would see
      stuff being drawn one-after-the-other */
  while ( !glfwWindowShouldClose( window ) ) {
    /* experimenting... */
    glClearColor( 0.6f, 0.6f, 0.6f, 1.0f );
    /* wipe the drawing surface clear */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUseProgram( shader_programme );
    glBindVertexArray( vao );
    /* draw points 0-3 from the currently bound VAO with current in-use shader */
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    /* update other events like input handling */
    glfwPollEvents();
    /* put the stuff we've been drawing onto the display */
    glfwSwapBuffers( window );
  }

  /* close GL context and any other GLFW resources */
  glfwTerminate();
  return 0;
}
