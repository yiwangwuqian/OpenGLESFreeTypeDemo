// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Simple_Texture2D.c
//
//    This is a simple example that draws a quad with a 2D
//    texture image. The purpose of this example is to demonstrate
//    the basics of 2D texturing
//
#include <stdlib.h>
#include "esUtil.h"
#include "esCommonTypes.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Sampler location
   GLint samplerLoc;
    
   TextureInfo *tInfo;

} UserData;

TextureInfo * CreateSimpleTexture2D(GLint width,GLint height)
{
    return textureFrom("OpenGL_ES_with_FreeType_So_Good", width, height);
}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   char vShaderStr[] =
      "#version 300 es                            \n"
      "layout(location = 0) in vec4 a_position;   \n"
      "layout(location = 1) in vec2 a_texCoord;   \n"
      "out vec2 v_texCoord;                       \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "   v_texCoord = a_texCoord;                \n"
      "}                                          \n";

   char fShaderStr[] =
      "#version 300 es                                     \n"
      "precision mediump float;                            \n"
      "in vec2 v_texCoord;                                 \n"
      "layout(location = 0) out vec4 outColor;             \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  outColor = texture( s_texture, v_texCoord );      \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

   // Load the texture
//   userData->tInfo = CreateSimpleTexture2D ();

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
    if (userData->tInfo != NULL){
        return;
    }
    userData->tInfo = CreateSimpleTexture2D(esContext->width, esContext->height);
    TextureInfo *tInfo = userData->tInfo;
    GLfloat hScale=0;
    GLfloat vScale=0;
    if (tInfo->tWidth <= esContext->width && tInfo->tHeight <= esContext->height){
        hScale = tInfo->tWidth/ (GLfloat)esContext->width;
        vScale = tInfo->tHeight/ (GLfloat)esContext->height;
    }else if (tInfo->tWidth >= esContext->width && tInfo->tHeight >= esContext->height){
        if (tInfo->tHeight >= tInfo->tWidth){
            vScale = 1;
            hScale = tInfo->tWidth/ (GLfloat)tInfo->tHeight;
        }else{
            hScale = 1;
            vScale = tInfo->tHeight/ (GLfloat)tInfo->tWidth;
        }
    }else{
        if (tInfo->tWidth >= esContext->width){
            hScale = 1;
            vScale = tInfo->tHeight/ (GLfloat)tInfo->tWidth;
        }else if(tInfo->tHeight >= esContext->height){
            vScale = 1;
            hScale = tInfo->tWidth/ (GLfloat)tInfo->tHeight;
        }
    }
   GLfloat vVertices[] = { -hScale,  vScale, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                           -hScale, -vScale, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            hScale, -vScale, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            hScale,  vScale, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT,
                           GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( 1, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->tInfo->textureId );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->tInfo->textureId );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

    float width=0;
    float height = 0;
    esWindowSize(&width, &height);
   esCreateWindow ( esContext, "Simple Texture 2D", width, height, ES_WINDOW_RGB );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterDrawFunc ( esContext, Draw );
   esRegisterShutdownFunc ( esContext, ShutDown );

   return GL_TRUE;
}
