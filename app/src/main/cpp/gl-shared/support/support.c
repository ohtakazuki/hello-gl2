//
//  support.c
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/01/27.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#include    "support.h"

/**
 * ファイルのフルパス -> ファイル名に変換する
 */
char* util_getFileName(char* __file__) {
    return strrchr(__file__, '/') + 1;
}

