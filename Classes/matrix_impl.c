/*
 NEON math library for the iPhone / iPod touch
 
 Copyright (c) 2009 Justin Saunders
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising
 from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must
 not claim that you wrote the original software. If you use this
 software in a product, an acknowledgment in the product documentation
 would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must
 not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source distribution.
 */


#ifdef __thumb__
#error "This file should be compiled in ARM mode only."
// Note in Xcode, right click file, Get Info->Build, Other compiler flags = "-marm"
#endif

//#include "matrix.h"
#include <string.h>

void matvec4_neon(const float* a, const float* b, float* output)
{
#if defined(_ARM_ARCH_7)
    asm volatile
    (
     // Store A & B leaving room for q4-q7, which should be preserved
     "vldmia %1, { q0-q3 } \n\t"
     "vldmia %2, { q8-q11 }\n\t"
     
     // result = first column of B x first row of A
     "vmul.f32 q12, q8, d0[0]\n\t"
     "vmul.f32 q13, q8, d2[0]\n\t"
     "vmul.f32 q14, q8, d4[0]\n\t"
     "vmul.f32 q15, q8, d6[0]\n\t"
     
     // result += second column of B x second row of A
     "vmla.f32 q12, q9, d0[1]\n\t"
     "vmla.f32 q13, q9, d2[1]\n\t"
     "vmla.f32 q14, q9, d4[1]\n\t"
     "vmla.f32 q15, q9, d6[1]\n\t"
     
     // result += third column of B x third row of A
     "vmla.f32 q12, q10, d1[0]\n\t"
     "vmla.f32 q13, q10, d3[0]\n\t"
     "vmla.f32 q14, q10, d5[0]\n\t"
     "vmla.f32 q15, q10, d7[0]\n\t"
     
     // result += last column of B x last row of A
     "vmla.f32 q12, q11, d1[1]\n\t"
     "vmla.f32 q13, q11, d3[1]\n\t"
     "vmla.f32 q14, q11, d5[1]\n\t"
     "vmla.f32 q15, q11, d7[1]\n\t"
     
     // output = result registers
     "vstmia %0, { q12-q15 }"
     : // no output
     : "r" (output), "r" (a), "r" (b)       // input - note *value* of pointer doesn't change
     : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15" //clobber
     );
#else
    float mat[16];
    
    const float *m1 = a, *m2 = b;
    
    mat[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    mat[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    mat[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    mat[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
    
    mat[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    mat[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    mat[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    mat[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
    
    mat[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    mat[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    mat[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    mat[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
    
    mat[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    mat[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    mat[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    mat[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    
    memcpy(output, mat, sizeof(float)*16);
#endif
}