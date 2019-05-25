/*
 * Projectiv Texture Mapping Library
 *
 * maintained by <hjwang1@163.com>
 *
 * Originally based on:
 * Copyright (C) 2018 Hongjun Wang.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 */

#include <iostream> 

#include <SOIL.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h> 
#include <GL/glut.h>
#include <GL/freeglut.h> 


using namespace std;

GLdouble objTeapotSize = 5.0;
GLuint programHandle;
GLuint vShader, fShader;

//读入着色器字符流  
char *textFileRead(const char *fn)
{
  FILE *fp;
  char *content = NULL;
  int count = 0;
  if (fn != NULL)
  {
    fp = fopen(fn, "rt");
    if (fp != NULL)
    {
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);
      if (count > 0)
      {
	content = (char *)malloc(sizeof(char) * (count + 1));
	count = fread(content, sizeof(char), count, fp);
	content[count] = '\0';
      }
      fclose(fp);
    }
  }
  return content;
}

void initShader(const char *VShaderFile, const char *FShaderFile)
{
  //1、查看显卡、GLSL和OpenGL的信息    
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
  cout << "显卡供应商   : " << vendor << endl;
  cout << "显卡型号     : " << renderer << endl;
  cout << "OpenGL版本   : " << version << endl;
  cout << "GLSL版本     : " << glslVersion << endl;
  //2、编译着色器    
  //创建着色器对象：顶点着色器  
  vShader = glCreateShader(GL_VERTEX_SHADER);
  //错误检测    
  if (0 == vShader)
  {
    cerr << "ERROR : Create vertex shader failed" << endl;
    exit(1);
  }
  //把着色器源代码和着色器对象相关联    
  const GLchar *vShaderCode = textFileRead(VShaderFile);
  const GLchar *vCodeArray[1] = { vShaderCode };

  //将字符数组绑定到对应的着色器对象上  
  glShaderSource(vShader, 1, vCodeArray, NULL);

  //编译着色器对象    
  glCompileShader(vShader);

  //检查编译是否成功    
  GLint compileResult;
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
  if (GL_FALSE == compileResult)
  {
    GLint logLen;
    //得到编译日志长度    
    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0)
    {
      char *log = (char *)malloc(logLen);
      GLsizei written;
      //得到日志信息并输出    
      glGetShaderInfoLog(vShader, logLen, &written, log);
      cerr << "vertex shader compile log : " << endl;
      cerr << log << endl;
      free(log);//释放空间    
    }
  }

  //创建着色器对象：片断着色器    
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  //错误检测    
  if (0 == fShader)
  {
    cerr << "ERROR : Create fragment shader failed" << endl;
    exit(1);
  }

  //把着色器源代码和着色器对象相关联    
  const GLchar *fShaderCode = textFileRead(FShaderFile);
  const GLchar *fCodeArray[1] = { fShaderCode };
  glShaderSource(fShader, 1, fCodeArray, NULL);

  //编译着色器对象    
  glCompileShader(fShader);

  //检查编译是否成功    
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
  if (GL_FALSE == compileResult)
  {
    GLint logLen;
    //得到编译日志长度    
    glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0)
    {
      char *log = (char *)malloc(logLen);
      GLsizei written;
      //得到日志信息并输出    
      glGetShaderInfoLog(fShader, logLen, &written, log);
      cerr << "fragment shader compile log : " << endl;
      cerr << log << endl;
      free(log);//释放空间    
    }
  }
  //3、链接着色器对象    
  //创建着色器程序    
  programHandle = glCreateProgram();
  if (!programHandle)
  {
    cerr << "ERROR : create program failed" << endl;
    exit(1);
  }
  //将着色器程序链接到所创建的程序中    
  glAttachShader(programHandle, vShader);
  glAttachShader(programHandle, fShader);
  //将这些对象链接成一个可执行程序    
  glLinkProgram(programHandle);
  //查询链接的结果    
  GLint linkStatus;
  glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
  if (GL_FALSE == linkStatus)
  {
    cerr << "ERROR : link shader program failed" << endl;
    GLint logLen;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
	    &logLen);
    if (logLen > 0)
    {
      char *log = (char *)malloc(logLen);
      GLsizei written;
      glGetProgramInfoLog(programHandle, logLen, &written, log);
      cerr << "Program log : " << endl;
      cerr << log << endl;
    }
  }
}

#define PI 3.1415926535 
//du是视点和x轴的夹角
static int sita=90, OriX=-1, OriY=-1, phi=90;  
//r是视点绕y轴的半径，h是视点高度即在y轴上的坐标 
static float r=10.5;
//弧度和角度转换参数
static float c=PI/180.0;
static float times = 1;
static float xdis = 0;
static float ydis = 0;

void onKey(unsigned char key, int x, int y) {
  if(key == 'a') {
    xdis -= 0.2;
  } else if(key == 'd') {
    xdis += 0.2;
  } else if(key == 's') {
    ydis -= 0.2;
  } else if(key == 'w') {
    ydis += 0.2;
  }
}

void menu(int selection)
{
  if (selection == 666) {
    exit(0);
  }
  onKey(selection, 0, 0);
}

void Mouse(int button, int state, int x, int y)
{
  /*
   * 记录鼠标按下位置
   */
  if(state == GLUT_DOWN) {
    OriX = x,OriY = y;
  }
  if(state == GLUT_UP && button == 3) {
    r += 0.5;
  }
  if(state == GLUT_UP && button == 4) {
    r -= 0.5;
  }
  if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
    sita = 90;
    OriX = -1;
    OriY = -1;
    phi = 90;
    times = 1;
    xdis = 0;
    ydis = 0;
    r = 10.5;
  }
}

void onMouseMove(int x,int y)   //处理鼠标拖动
{
  sita += x - OriX;
  phi += (y-OriY);
  if(phi < 5) phi = 5;
  if(phi > 175) phi = 175;
  OriX = x;
  OriY = y;
}

#define WindowWidth  400
#define WindowHeight 400
#define WindowTitle  "OpenGL projectiv texture mesh"

 
//定义两个纹理对象编号
GLuint texGround;
GLuint texWall;

static GLfloat angle = 0.0f;   //旋转角度
static GLfloat parg = 18.0f;
static GLfloat narg = -18.0f;
 

GLuint soil_load_texture(const char* file_name)
{
  GLuint texture_ID = 0;
  
  int width, height;
  unsigned char* image = SOIL_load_image(file_name, &width, &height, 0, SOIL_LOAD_RGB);
  glGenTextures(1, &texture_ID);
  glBindTexture(GL_TEXTURE_2D, texture_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);  
  //glBindTexture(GL_TEXTURE_2D, 0);
  
  return texture_ID;
}

void drawCameraLine(glm::vec3 projPos, glm::vec3 projAt, glm::vec3 projUp, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
  glm::vec3 forward = projAt - projPos;
  glm::vec3 forw = glm::normalize(forward);
  glm::vec3 left = glm::normalize(glm::cross(projUp, forward));
  glm::vec3 uupp = glm::cross(forw, left);
  GLfloat tanfovy = tan(fovy*0.0087222);
  GLfloat tanfovx = tanfovy*aspect;
  glm::vec3 nlu = projPos + forw*zNear + left*tanfovx*zNear + uupp*tanfovy*zNear;
  glm::vec3 nl_u = projPos + forw*zNear + left*tanfovx*zNear - uupp*tanfovy*zNear;
  glm::vec3 n_l_u = projPos + forw*zNear - left*tanfovx*zNear - uupp*tanfovy*zNear;
  glm::vec3 n_lu = projPos + forw*zNear - left*tanfovx*zNear + uupp*tanfovy*zNear;
  
  glm::vec3 flu = projPos + forw*zFar + left*tanfovx*zFar + uupp*tanfovy*zFar;
  glm::vec3 fl_u = projPos + forw*zFar + left*tanfovx*zFar - uupp*tanfovy*zFar;
  glm::vec3 f_l_u = projPos + forw*zFar - left*tanfovx*zFar - uupp*tanfovy*zFar;
  glm::vec3 f_lu = projPos + forw*zFar - left*tanfovx*zFar + uupp*tanfovy*zFar;
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_LOOP);
  glVertex3fv(glm::value_ptr( nlu ));
  glVertex3fv(glm::value_ptr( flu));
  glVertex3fv(glm::value_ptr( fl_u));
  glVertex3fv(glm::value_ptr( nl_u));
  glVertex3fv(glm::value_ptr( n_l_u));
  glVertex3fv(glm::value_ptr( f_l_u));
  glVertex3fv(glm::value_ptr( f_lu));
  glVertex3fv(glm::value_ptr( n_lu));
  glEnd();
}
 
void display(void)
{
  // 清除屏幕
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  // 设置视角
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, 1, 1, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(-4, 7,7, 0, 0, 0, 0, 0, 1);
  //gluLookAt(0, 0, 15, 0, -4, 0, 0, 1, 0);
  gluLookAt(r*sin(c*phi)*cos(c*sita), r*cos(c*phi), r*sin(c*phi)*sin(c*sita), 0.0,-4.0,0.0, 0.0,1.0,0.0);

  //glRotatef(angle, 0.0f, 1.0f, 0.0f);
  //glScalef(times, times, times);
  //glTranslatef(xdis, ydis, 0);
  /*
    * 随后几行的注释与否关系到纹理贴图渲染与纹理投影映射的区别
  /*/
  glUseProgram(programHandle);
  glm::vec3 projPos = glm::vec3(0.0f,0.0f,5.0f);
  glm::vec3 projAt = glm::vec3(0.0f,-4.0f,0.0f);
  glm::vec3 projUp = glm::vec3(0.0f,1.0f,0.0f);
  glm::mat4 projView = glm::lookAt(projPos, projAt, projUp);
  glm::mat4 projProj = glm::perspective(55.0f, 1.0f, 1.0f, 100.0f);
  glm::mat4 projTrans = glm::translate(glm::mat4( 1.0f ), glm::vec3(0.5f));
  glm::mat4 projScaleTrans = glm::scale(projTrans, glm::vec3(0.5f));
  glm::mat4 projMVP = projScaleTrans * projProj * projView;
  glUniformMatrix4fv(glGetUniformLocation(programHandle, "ProjectorMatrix"), 1, GL_FALSE, glm::value_ptr( projMVP ));//*/
  glm::vec3 mkd = glm::vec3(0.9f, 0.5f, 0.3f);
  glm::vec3 mka = glm::vec3(0.9f, 0.5f, 0.3f);
  glm::vec3 mks = glm::vec3(0.8f, 0.8f, 0.8f);
  glm::vec3 linten = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec4 lpos = glm::vec4(0.0f,-4.0f,0.0f, 1.0f);
  glUniform3fv(glGetUniformLocation(programHandle, "Material.Kd"), 1, glm::value_ptr( mkd ));
  glUniform3fv(glGetUniformLocation(programHandle, "Material.Ka"), 1, glm::value_ptr( mka ));
  glUniform3fv(glGetUniformLocation(programHandle, "Material.Ks"), 1, glm::value_ptr( mks ));
  glUniform1f(glGetUniformLocation(programHandle, "Material.Shininess"), 100.0f);
  glUniform4fv(glGetUniformLocation(programHandle, "Light.Position"), 1, glm::value_ptr( lpos ));
  glUniform3fv(glGetUniformLocation(programHandle, "Light.Intensity"), 1, glm::value_ptr( linten ));

  //drawCameraLine(projPos, projAt, projUp, 55.0f, 1.0f, 1.0f, 30.0f);
  glBindTexture(GL_TEXTURE_2D, texGround);
  //*
  glBegin(GL_POLYGON);
    glVertex3f(narg, narg, parg);
    glVertex3f(narg, narg, narg);
    glVertex3f(parg, narg, narg);
    glVertex3f(parg, narg, parg);
  glEnd();//*/
  //*
  glBegin(GL_POLYGON);
    glVertex3f(narg, parg, narg);
    glVertex3f(narg, narg, narg);
    glVertex3f(parg, narg, narg);
    glVertex3f(parg, parg, narg);
  glEnd();//*/
  //glutSolidSphere(6, 64, 64);
  //glutSolidTeapot(objTeapotSize);

  glutSwapBuffers();  
}
 
void myIdle(void)  
{     
  angle += 1.8f;    
  if( angle >= 360.0f ) {
    angle = 0.0f;
  }
  display();  
}  

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
}
 
int main(int argc, char* argv[])
{
  // GLUT初始化
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutCreateWindow(WindowTitle);	
  glEnable(GL_DEPTH_TEST);    
  glEnable(GL_TEXTURE_2D);
  
  //初始化glew扩展库    
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
  }
  initShader("../data/TexVerShader.txt", "../data/TexFragmentShader.txt");
  //texGround = load_texture("../data/sample.bmp");  //加载纹理
  //texWall = load_texture("../data/sample.bmp");
  texGround = soil_load_texture("../data/lena.png");//sample.bmp//
  //texWall = soil_load_texture("../data/chessboard.bmp");
  glutDisplayFunc(&display);
  glutReshapeFunc(reshape);
  glutIdleFunc(&myIdle);  
  glutMouseFunc(Mouse);
  glutMotionFunc(onMouseMove);
  glutKeyboardFunc(onKey);
  glutCreateMenu(menu);
  glutAddMenuEntry("Up moving", 'w');
  glutAddMenuEntry("Down moving", 's');
  glutAddMenuEntry("Quit", 666);
  glutAttachMenu(GLUT_MIDDLE_BUTTON);
  
  glutMainLoop(); 
  return 0;
}