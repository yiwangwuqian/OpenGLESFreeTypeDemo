//
//  esCommonTypes.h
//  OpenGLESDemo
//
//  Created by guohaoyang on 2019/11/3.
//  Copyright © 2019 guohaoyang. All rights reserved.
//

#ifndef esCommonTypes_h
#define esCommonTypes_h

#include <stdio.h>

typedef struct TextureInfo
{
    // Texture handle
    GLuint textureId;
    
    // Texture truly width
    GLint tWidth;
    
    // Texture truly height
    GLint tHeight;
} TextureInfo;

#endif /* esCommonTypes_h */
