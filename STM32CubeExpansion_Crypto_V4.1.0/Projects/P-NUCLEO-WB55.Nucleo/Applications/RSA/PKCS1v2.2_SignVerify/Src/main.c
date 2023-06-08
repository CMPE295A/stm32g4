/**
  ******************************************************************************
  * @file    RSA/PKCS1v2.2_SignVerify/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a short description of how to use the
  *          STM32 Cryptographic Library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "cmox_crypto.h"

/* Global variables ----------------------------------------------------------*/
/* RSA context */
cmox_rsa_handle_t Rsa_Ctx;
/* RSA key */
cmox_rsa_key_t Rsa_Key;

/* RSA working buffer */
uint8_t Working_Buffer[7000];

__IO TestStatus glob_status = FAILED;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from pkcs-1v2-1d2-vec/pss-vect.txt
  * # Example 10: A 2048-bit RSA key pair
# -----------------------------------


# Public key
# ----------

# Modulus:
a5 dd 86 7a c4 cb 02 f9 0b 94 57 d4 8c 14 a7 70
ef 99 1c 56 c3 9c 0e c6 5f d1 1a fa 89 37 ce a5
7b 9b e7 ac 73 b4 5c 00 17 61 5b 82 d6 22 e3 18
75 3b 60 27 c0 fd 15 7b e1 2f 80 90 fe e2 a7 ad
cd 0e ef 75 9f 88 ba 49 97 c7 a4 2d 58 c9 aa 12
cb 99 ae 00 1f e5 21 c1 3b b5 43 14 45 a8 d5 ae
4f 5e 4c 7e 94 8a c2 27 d3 60 40 71 f2 0e 57 7e
90 5f be b1 5d fa f0 6d 1d e5 ae 62 53 d6 3a 6a
21 20 b3 1a 5d a5 da bc 95 50 60 0e 20 f2 7d 37
39 e2 62 79 25 fe a3 cc 50 9f 21 df f0 4e 6e ea
45 49 c5 40 d6 80 9f f9 30 7e ed e9 1f ff 58 73
3d 83 85 a2 37 d6 d3 70 5a 33 e3 91 90 09 92 07
0d f7 ad f1 35 7c f7 e3 70 0c e3 66 7d e8 3f 17
b8 df 17 78 db 38 1d ce 09 cb 4a d0 58 a5 11 00
1a 73 81 98 ee 27 cf 55 a1 3b 75 45 39 90 65 82
ec 8b 17 4b d5 8d 5d 1f 3d 76 7c 61 37 21 ae 05

# Exponent:
01 00 01

# Private key
# -----------

# Modulus:
a5 dd 86 7a c4 cb 02 f9 0b 94 57 d4 8c 14 a7 70
ef 99 1c 56 c3 9c 0e c6 5f d1 1a fa 89 37 ce a5
7b 9b e7 ac 73 b4 5c 00 17 61 5b 82 d6 22 e3 18
75 3b 60 27 c0 fd 15 7b e1 2f 80 90 fe e2 a7 ad
cd 0e ef 75 9f 88 ba 49 97 c7 a4 2d 58 c9 aa 12
cb 99 ae 00 1f e5 21 c1 3b b5 43 14 45 a8 d5 ae
4f 5e 4c 7e 94 8a c2 27 d3 60 40 71 f2 0e 57 7e
90 5f be b1 5d fa f0 6d 1d e5 ae 62 53 d6 3a 6a
21 20 b3 1a 5d a5 da bc 95 50 60 0e 20 f2 7d 37
39 e2 62 79 25 fe a3 cc 50 9f 21 df f0 4e 6e ea
45 49 c5 40 d6 80 9f f9 30 7e ed e9 1f ff 58 73
3d 83 85 a2 37 d6 d3 70 5a 33 e3 91 90 09 92 07
0d f7 ad f1 35 7c f7 e3 70 0c e3 66 7d e8 3f 17
b8 df 17 78 db 38 1d ce 09 cb 4a d0 58 a5 11 00
1a 73 81 98 ee 27 cf 55 a1 3b 75 45 39 90 65 82
ec 8b 17 4b d5 8d 5d 1f 3d 76 7c 61 37 21 ae 05

# Public exponent:
01 00 01

# Exponent:
2d 2f f5 67 b3 fe 74 e0 61 91 b7 fd ed 6d e1 12
29 0c 67 06 92 43 0d 59 69 18 40 47 da 23 4c 96
93 de ed 16 73 ed 42 95 39 c9 69 d3 72 c0 4d 6b
47 e0 f5 b8 ce e0 84 3e 5c 22 83 5d bd 3b 05 a0
99 79 84 ae 60 58 b1 1b c4 90 7c bf 67 ed 84 fa
9a e2 52 df b0 d0 cd 49 e6 18 e3 5d fd fe 59 bc
a3 dd d6 6c 33 ce bb c7 7a d4 41 aa 69 5e 13 e3
24 b5 18 f0 1c 60 f5 a8 5c 99 4a d1 79 f2 a6 b5
fb e9 34 02 b1 17 67 be 01 bf 07 34 44 d6 ba 1d
d2 bc a5 bd 07 4d 4a 5f ae 35 31 ad 13 03 d8 4b
30 d8 97 31 8c bb ba 04 e0 3c 2e 66 de 6d 91 f8
2f 96 ea 1d 4b b5 4a 5a ae 10 2d 59 46 57 f5 c9
78 95 53 51 2b 29 6d ea 29 d8 02 31 96 35 7e 3e
3a 6e 95 8f 39 e3 c2 34 40 38 ea 60 4b 31 ed c6
f0 f7 ff 6e 71 81 a5 7c 92 82 6a 26 8f 86 76 8e
96 f8 78 56 2f c7 1d 85 d6 9e 44 86 12 f7 04 8f

# Prime 1:
cf d5 02 83 fe ee b9 7f 6f 08 d7 3c bc 7b 38 36
f8 2b bc d4 99 47 9f 5e 6f 76 fd fc b8 b3 8c 4f
71 dc 9e 88 bd 6a 6f 76 37 1a fd 65 d2 af 18 62
b3 2a fb 34 a9 5f 71 b8 b1 32 04 3f fe be 3a 95
2b af 75 92 44 81 48 c0 3f 9c 69 b1 d6 8e 4c e5
cf 32 c8 6b af 46 fe d3 01 ca 1a b4 03 06 9b 32
f4 56 b9 1f 71 89 8a b0 81 cd 8c 42 52 ef 52 71
91 5c 97 94 b8 f2 95 85 1d a7 51 0f 99 cb 73 eb

# Prime 2:
cc 4e 90 d2 a1 b3 a0 65 d3 b2 d1 f5 a8 fc e3 1b
54 44 75 66 4e ab 56 1d 29 71 b9 9f b7 be f8 44
e8 ec 1f 36 0b 8c 2a c8 35 96 92 97 1e a6 a3 8f
72 3f cc 21 1f 5d bc b1 77 a0 fd ac 51 64 a1 d4
ff 7f bb 4e 82 99 86 35 3c b9 83 65 9a 14 8c dd
42 0c 7d 31 ba 38 22 ea 90 a3 2b e4 6c 03 0e 8c
17 e1 fa 0a d3 78 59 e0 6b 0a a6 fa 3b 21 6d 9c
be 6c 0e 22 33 97 69 c0 a6 15 91 3e 5d a7 19 cf

# Prime exponent 1:
1c 2d 1f c3 2f 6b c4 00 4f d8 5d fd e0 fb bf 9a
4c 38 f9 c7 c4 e4 1d ea 1a a8 82 34 a2 01 cd 92
f3 b7 da 52 65 83 a9 8a d8 5b b3 60 fb 98 3b 71
1e 23 44 9d 56 1d 17 78 d7 a5 15 48 6b cb f4 7b
46 c9 e9 e1 a3 a1 f7 70 00 ef be b0 9a 8a fe 47
e5 b8 57 cd a9 9c b1 6d 7f ff 9b 71 2e 3b d6 0c
a9 6d 9c 79 73 d6 16 d4 69 34 a9 c0 50 28 1c 00
43 99 ce ff 1d b7 dd a7 87 66 a8 a9 b9 cb 08 73

# Prime exponent 2:
cb 3b 3c 04 ca a5 8c 60 be 7d 9b 2d eb b3 e3 96
43 f4 f5 73 97 be 08 23 6a 1e 9e af aa 70 65 36
e7 1c 3a cf e0 1c c6 51 f2 3c 9e 05 85 8f ee 13
bb 6a 8a fc 47 df 4e dc 9a 4b a3 0b ce cb 73 d0
15 78 52 32 7e e7 89 01 5c 2e 8d ee 7b 9f 05 a0
f3 1a c9 4e b6 17 31 64 74 0c 5c 95 14 7c d5 f3
b5 ae 2c b4 a8 37 87 f0 1d 8a b3 1f 27 c2 d0 ee
a2 dd 8a 11 ab 90 6a ba 20 7c 43 c6 ee 12 53 31

# Coefficient:
12 f6 b2 cf 13 74 a7 36 fa d0 56 16 05 0f 96 ab
4b 61 d1 17 7c 7f 9d 52 5a 29 f3 d1 80 e7 76 67
e9 9d 99 ab f0 52 5d 07 58 66 0f 37 52 65 5b 0f
25 b8 df 84 31 d9 a8 ff 77 c1 6c 12 a0 a5 12 2a
9f 0b f7 cf d5 a2 66 a3 5c 15 9f 99 12 08 b9 03
16 ff 44 4f 3e 0b 6b d0 e9 3b 8a 7a 24 48 e9 57
e3 dd a6 cf cf 22 66 b1 06 01 3a c4 68 08 d3 b3
88 7b 3b 00 34 4b aa c9 53 0b 4c e7 08 fc 32 b6

# PSS Example 10.2

# -----------------

# Message to be signed:
dd 67 0a 01 46 58 68 ad c9 3f 26 13 19 57 a5 0c
52 fb 77 7c db aa 30 89 2c 9e 12 36 11 64 ec 13
97 9d 43 04 81 18 e4 44 5d b8 7b ee 58 dd 98 7b
34 25 d0 20 71 d8 db ae 80 70 8b 03 9d bb 64 db
d1 de 56 57 d9 fe d0 c1 18 a5 41 43 74 2e 0f f3
c8 7f 74 e4 58 57 64 7a f3 f7 9e b0 a1 4c 9d 75
ea 9a 1a 04 b7 cf 47 8a 89 7a 70 8f d9 88 f4 8e
80 1e db 0b 70 39 df 8c 23 bb 3c 56 f4 e8 21 ac

# Salt:
8b 2b dd 4b 40 fa f5 45 c7 78 dd f9 bc 1a 49 cb
57 f9 b7 1b

# Signature:
14 ae 35 d9 dd 06 ba 92 f7 f3 b8 97 97 8a ed 7c
d4 bf 5f f0 b5 85 a4 0b d4 6c e1 b4 2c d2 70 30
53 bb 90 44 d6 4e 81 3d 8f 96 db 2d d7 00 7d 10
11 8f 6f 8f 84 96 09 7a d7 5e 1f f6 92 34 1b 28
92 ad 55 a6 33 a1 c5 5e 7f 0a 0a d5 9a 0e 20 3a
5b 82 78 ae c5 4d d8 62 2e 28 31 d8 71 74 f8 ca
ff 43 ee 6c 46 44 53 45 d8 4a 59 65 9b fb 92 ec
d4 c8 18 66 86 95 f3 47 06 f6 68 28 a8 99 59 63
7f 2b f3 e3 25 1c 24 bd ba 4d 4b 76 49 da 00 22
21 8b 11 9c 84 e7 9a 65 27 ec 5b 8a 5f 86 1c 15
99 52 e2 3e c0 5e 1e 71 73 46 fa ef e8 b1 68 68
25 bd 2b 26 2f b2 53 10 66 c0 de 09 ac de 2e 42
31 69 07 28 b5 d8 5e 11 5a 2f 6b 92 b7 9c 25 ab
c9 bd 93 99 ff 8b cf 82 5a 52 ea 1f 56 ea 76 dd
26 f4 3b aa fa 18 bf a9 2a 50 4c bd 35 69 9e 26
d1 dc c5 a2 88 73 85 f3 c6 32 32 f0 6f 32 44 c3


  */
const uint8_t Message[] =
{
  0xdd, 0x67, 0x0a, 0x01, 0x46, 0x58, 0x68, 0xad, 0xc9, 0x3f, 0x26, 0x13, 0x19, 0x57, 0xa5, 0x0c,
  0x52, 0xfb, 0x77, 0x7c, 0xdb, 0xaa, 0x30, 0x89, 0x2c, 0x9e, 0x12, 0x36, 0x11, 0x64, 0xec, 0x13,
  0x97, 0x9d, 0x43, 0x04, 0x81, 0x18, 0xe4, 0x44, 0x5d, 0xb8, 0x7b, 0xee, 0x58, 0xdd, 0x98, 0x7b,
  0x34, 0x25, 0xd0, 0x20, 0x71, 0xd8, 0xdb, 0xae, 0x80, 0x70, 0x8b, 0x03, 0x9d, 0xbb, 0x64, 0xdb,
  0xd1, 0xde, 0x56, 0x57, 0xd9, 0xfe, 0xd0, 0xc1, 0x18, 0xa5, 0x41, 0x43, 0x74, 0x2e, 0x0f, 0xf3,
  0xc8, 0x7f, 0x74, 0xe4, 0x58, 0x57, 0x64, 0x7a, 0xf3, 0xf7, 0x9e, 0xb0, 0xa1, 0x4c, 0x9d, 0x75,
  0xea, 0x9a, 0x1a, 0x04, 0xb7, 0xcf, 0x47, 0x8a, 0x89, 0x7a, 0x70, 0x8f, 0xd9, 0x88, 0xf4, 0x8e,
  0x80, 0x1e, 0xdb, 0x0b, 0x70, 0x39, 0xdf, 0x8c, 0x23, 0xbb, 0x3c, 0x56, 0xf4, 0xe8, 0x21, 0xac
};
const uint8_t Salt[] =
{
  0x8b, 0x2b, 0xdd, 0x4b, 0x40, 0xfa, 0xf5, 0x45, 0xc7, 0x78, 0xdd, 0xf9, 0xbc, 0x1a, 0x49, 0xcb,
  0x57, 0xf9, 0xb7, 0x1b
};
const uint8_t Known_Signature[] =
{
  0x14, 0xae, 0x35, 0xd9, 0xdd, 0x06, 0xba, 0x92, 0xf7, 0xf3, 0xb8, 0x97, 0x97, 0x8a, 0xed, 0x7c,
  0xd4, 0xbf, 0x5f, 0xf0, 0xb5, 0x85, 0xa4, 0x0b, 0xd4, 0x6c, 0xe1, 0xb4, 0x2c, 0xd2, 0x70, 0x30,
  0x53, 0xbb, 0x90, 0x44, 0xd6, 0x4e, 0x81, 0x3d, 0x8f, 0x96, 0xdb, 0x2d, 0xd7, 0x00, 0x7d, 0x10,
  0x11, 0x8f, 0x6f, 0x8f, 0x84, 0x96, 0x09, 0x7a, 0xd7, 0x5e, 0x1f, 0xf6, 0x92, 0x34, 0x1b, 0x28,
  0x92, 0xad, 0x55, 0xa6, 0x33, 0xa1, 0xc5, 0x5e, 0x7f, 0x0a, 0x0a, 0xd5, 0x9a, 0x0e, 0x20, 0x3a,
  0x5b, 0x82, 0x78, 0xae, 0xc5, 0x4d, 0xd8, 0x62, 0x2e, 0x28, 0x31, 0xd8, 0x71, 0x74, 0xf8, 0xca,
  0xff, 0x43, 0xee, 0x6c, 0x46, 0x44, 0x53, 0x45, 0xd8, 0x4a, 0x59, 0x65, 0x9b, 0xfb, 0x92, 0xec,
  0xd4, 0xc8, 0x18, 0x66, 0x86, 0x95, 0xf3, 0x47, 0x06, 0xf6, 0x68, 0x28, 0xa8, 0x99, 0x59, 0x63,
  0x7f, 0x2b, 0xf3, 0xe3, 0x25, 0x1c, 0x24, 0xbd, 0xba, 0x4d, 0x4b, 0x76, 0x49, 0xda, 0x00, 0x22,
  0x21, 0x8b, 0x11, 0x9c, 0x84, 0xe7, 0x9a, 0x65, 0x27, 0xec, 0x5b, 0x8a, 0x5f, 0x86, 0x1c, 0x15,
  0x99, 0x52, 0xe2, 0x3e, 0xc0, 0x5e, 0x1e, 0x71, 0x73, 0x46, 0xfa, 0xef, 0xe8, 0xb1, 0x68, 0x68,
  0x25, 0xbd, 0x2b, 0x26, 0x2f, 0xb2, 0x53, 0x10, 0x66, 0xc0, 0xde, 0x09, 0xac, 0xde, 0x2e, 0x42,
  0x31, 0x69, 0x07, 0x28, 0xb5, 0xd8, 0x5e, 0x11, 0x5a, 0x2f, 0x6b, 0x92, 0xb7, 0x9c, 0x25, 0xab,
  0xc9, 0xbd, 0x93, 0x99, 0xff, 0x8b, 0xcf, 0x82, 0x5a, 0x52, 0xea, 0x1f, 0x56, 0xea, 0x76, 0xdd,
  0x26, 0xf4, 0x3b, 0xaa, 0xfa, 0x18, 0xbf, 0xa9, 0x2a, 0x50, 0x4c, 0xbd, 0x35, 0x69, 0x9e, 0x26,
  0xd1, 0xdc, 0xc5, 0xa2, 0x88, 0x73, 0x85, 0xf3, 0xc6, 0x32, 0x32, 0xf0, 0x6f, 0x32, 0x44, 0xc3
};

const uint8_t Modulus[] =
{
  0xa5, 0xdd, 0x86, 0x7a, 0xc4, 0xcb, 0x02, 0xf9, 0x0b, 0x94, 0x57, 0xd4, 0x8c, 0x14, 0xa7, 0x70,
  0xef, 0x99, 0x1c, 0x56, 0xc3, 0x9c, 0x0e, 0xc6, 0x5f, 0xd1, 0x1a, 0xfa, 0x89, 0x37, 0xce, 0xa5,
  0x7b, 0x9b, 0xe7, 0xac, 0x73, 0xb4, 0x5c, 0x00, 0x17, 0x61, 0x5b, 0x82, 0xd6, 0x22, 0xe3, 0x18,
  0x75, 0x3b, 0x60, 0x27, 0xc0, 0xfd, 0x15, 0x7b, 0xe1, 0x2f, 0x80, 0x90, 0xfe, 0xe2, 0xa7, 0xad,
  0xcd, 0x0e, 0xef, 0x75, 0x9f, 0x88, 0xba, 0x49, 0x97, 0xc7, 0xa4, 0x2d, 0x58, 0xc9, 0xaa, 0x12,
  0xcb, 0x99, 0xae, 0x00, 0x1f, 0xe5, 0x21, 0xc1, 0x3b, 0xb5, 0x43, 0x14, 0x45, 0xa8, 0xd5, 0xae,
  0x4f, 0x5e, 0x4c, 0x7e, 0x94, 0x8a, 0xc2, 0x27, 0xd3, 0x60, 0x40, 0x71, 0xf2, 0x0e, 0x57, 0x7e,
  0x90, 0x5f, 0xbe, 0xb1, 0x5d, 0xfa, 0xf0, 0x6d, 0x1d, 0xe5, 0xae, 0x62, 0x53, 0xd6, 0x3a, 0x6a,
  0x21, 0x20, 0xb3, 0x1a, 0x5d, 0xa5, 0xda, 0xbc, 0x95, 0x50, 0x60, 0x0e, 0x20, 0xf2, 0x7d, 0x37,
  0x39, 0xe2, 0x62, 0x79, 0x25, 0xfe, 0xa3, 0xcc, 0x50, 0x9f, 0x21, 0xdf, 0xf0, 0x4e, 0x6e, 0xea,
  0x45, 0x49, 0xc5, 0x40, 0xd6, 0x80, 0x9f, 0xf9, 0x30, 0x7e, 0xed, 0xe9, 0x1f, 0xff, 0x58, 0x73,
  0x3d, 0x83, 0x85, 0xa2, 0x37, 0xd6, 0xd3, 0x70, 0x5a, 0x33, 0xe3, 0x91, 0x90, 0x09, 0x92, 0x07,
  0x0d, 0xf7, 0xad, 0xf1, 0x35, 0x7c, 0xf7, 0xe3, 0x70, 0x0c, 0xe3, 0x66, 0x7d, 0xe8, 0x3f, 0x17,
  0xb8, 0xdf, 0x17, 0x78, 0xdb, 0x38, 0x1d, 0xce, 0x09, 0xcb, 0x4a, 0xd0, 0x58, 0xa5, 0x11, 0x00,
  0x1a, 0x73, 0x81, 0x98, 0xee, 0x27, 0xcf, 0x55, 0xa1, 0x3b, 0x75, 0x45, 0x39, 0x90, 0x65, 0x82,
  0xec, 0x8b, 0x17, 0x4b, 0xd5, 0x8d, 0x5d, 0x1f, 0x3d, 0x76, 0x7c, 0x61, 0x37, 0x21, 0xae, 0x05
};
const uint8_t Public_Exponent[] =
{
  0x01, 0x00, 0x01
};
const uint8_t Private_Exponent[] =
{
  0x2d, 0x2f, 0xf5, 0x67, 0xb3, 0xfe, 0x74, 0xe0, 0x61, 0x91, 0xb7, 0xfd, 0xed, 0x6d, 0xe1, 0x12,
  0x29, 0x0c, 0x67, 0x06, 0x92, 0x43, 0x0d, 0x59, 0x69, 0x18, 0x40, 0x47, 0xda, 0x23, 0x4c, 0x96,
  0x93, 0xde, 0xed, 0x16, 0x73, 0xed, 0x42, 0x95, 0x39, 0xc9, 0x69, 0xd3, 0x72, 0xc0, 0x4d, 0x6b,
  0x47, 0xe0, 0xf5, 0xb8, 0xce, 0xe0, 0x84, 0x3e, 0x5c, 0x22, 0x83, 0x5d, 0xbd, 0x3b, 0x05, 0xa0,
  0x99, 0x79, 0x84, 0xae, 0x60, 0x58, 0xb1, 0x1b, 0xc4, 0x90, 0x7c, 0xbf, 0x67, 0xed, 0x84, 0xfa,
  0x9a, 0xe2, 0x52, 0xdf, 0xb0, 0xd0, 0xcd, 0x49, 0xe6, 0x18, 0xe3, 0x5d, 0xfd, 0xfe, 0x59, 0xbc,
  0xa3, 0xdd, 0xd6, 0x6c, 0x33, 0xce, 0xbb, 0xc7, 0x7a, 0xd4, 0x41, 0xaa, 0x69, 0x5e, 0x13, 0xe3,
  0x24, 0xb5, 0x18, 0xf0, 0x1c, 0x60, 0xf5, 0xa8, 0x5c, 0x99, 0x4a, 0xd1, 0x79, 0xf2, 0xa6, 0xb5,
  0xfb, 0xe9, 0x34, 0x02, 0xb1, 0x17, 0x67, 0xbe, 0x01, 0xbf, 0x07, 0x34, 0x44, 0xd6, 0xba, 0x1d,
  0xd2, 0xbc, 0xa5, 0xbd, 0x07, 0x4d, 0x4a, 0x5f, 0xae, 0x35, 0x31, 0xad, 0x13, 0x03, 0xd8, 0x4b,
  0x30, 0xd8, 0x97, 0x31, 0x8c, 0xbb, 0xba, 0x04, 0xe0, 0x3c, 0x2e, 0x66, 0xde, 0x6d, 0x91, 0xf8,
  0x2f, 0x96, 0xea, 0x1d, 0x4b, 0xb5, 0x4a, 0x5a, 0xae, 0x10, 0x2d, 0x59, 0x46, 0x57, 0xf5, 0xc9,
  0x78, 0x95, 0x53, 0x51, 0x2b, 0x29, 0x6d, 0xea, 0x29, 0xd8, 0x02, 0x31, 0x96, 0x35, 0x7e, 0x3e,
  0x3a, 0x6e, 0x95, 0x8f, 0x39, 0xe3, 0xc2, 0x34, 0x40, 0x38, 0xea, 0x60, 0x4b, 0x31, 0xed, 0xc6,
  0xf0, 0xf7, 0xff, 0x6e, 0x71, 0x81, 0xa5, 0x7c, 0x92, 0x82, 0x6a, 0x26, 0x8f, 0x86, 0x76, 0x8e,
  0x96, 0xf8, 0x78, 0x56, 0x2f, 0xc7, 0x1d, 0x85, 0xd6, 0x9e, 0x44, 0x86, 0x12, 0xf7, 0x04, 0x8f
};
const uint8_t P_Prime[] =
{
  0xcf, 0xd5, 0x02, 0x83, 0xfe, 0xee, 0xb9, 0x7f, 0x6f, 0x08, 0xd7, 0x3c, 0xbc, 0x7b, 0x38, 0x36,
  0xf8, 0x2b, 0xbc, 0xd4, 0x99, 0x47, 0x9f, 0x5e, 0x6f, 0x76, 0xfd, 0xfc, 0xb8, 0xb3, 0x8c, 0x4f,
  0x71, 0xdc, 0x9e, 0x88, 0xbd, 0x6a, 0x6f, 0x76, 0x37, 0x1a, 0xfd, 0x65, 0xd2, 0xaf, 0x18, 0x62,
  0xb3, 0x2a, 0xfb, 0x34, 0xa9, 0x5f, 0x71, 0xb8, 0xb1, 0x32, 0x04, 0x3f, 0xfe, 0xbe, 0x3a, 0x95,
  0x2b, 0xaf, 0x75, 0x92, 0x44, 0x81, 0x48, 0xc0, 0x3f, 0x9c, 0x69, 0xb1, 0xd6, 0x8e, 0x4c, 0xe5,
  0xcf, 0x32, 0xc8, 0x6b, 0xaf, 0x46, 0xfe, 0xd3, 0x01, 0xca, 0x1a, 0xb4, 0x03, 0x06, 0x9b, 0x32,
  0xf4, 0x56, 0xb9, 0x1f, 0x71, 0x89, 0x8a, 0xb0, 0x81, 0xcd, 0x8c, 0x42, 0x52, 0xef, 0x52, 0x71,
  0x91, 0x5c, 0x97, 0x94, 0xb8, 0xf2, 0x95, 0x85, 0x1d, 0xa7, 0x51, 0x0f, 0x99, 0xcb, 0x73, 0xeb
};
const uint8_t Q_Prime[] =
{
  0xcc, 0x4e, 0x90, 0xd2, 0xa1, 0xb3, 0xa0, 0x65, 0xd3, 0xb2, 0xd1, 0xf5, 0xa8, 0xfc, 0xe3, 0x1b,
  0x54, 0x44, 0x75, 0x66, 0x4e, 0xab, 0x56, 0x1d, 0x29, 0x71, 0xb9, 0x9f, 0xb7, 0xbe, 0xf8, 0x44,
  0xe8, 0xec, 0x1f, 0x36, 0x0b, 0x8c, 0x2a, 0xc8, 0x35, 0x96, 0x92, 0x97, 0x1e, 0xa6, 0xa3, 0x8f,
  0x72, 0x3f, 0xcc, 0x21, 0x1f, 0x5d, 0xbc, 0xb1, 0x77, 0xa0, 0xfd, 0xac, 0x51, 0x64, 0xa1, 0xd4,
  0xff, 0x7f, 0xbb, 0x4e, 0x82, 0x99, 0x86, 0x35, 0x3c, 0xb9, 0x83, 0x65, 0x9a, 0x14, 0x8c, 0xdd,
  0x42, 0x0c, 0x7d, 0x31, 0xba, 0x38, 0x22, 0xea, 0x90, 0xa3, 0x2b, 0xe4, 0x6c, 0x03, 0x0e, 0x8c,
  0x17, 0xe1, 0xfa, 0x0a, 0xd3, 0x78, 0x59, 0xe0, 0x6b, 0x0a, 0xa6, 0xfa, 0x3b, 0x21, 0x6d, 0x9c,
  0xbe, 0x6c, 0x0e, 0x22, 0x33, 0x97, 0x69, 0xc0, 0xa6, 0x15, 0x91, 0x3e, 0x5d, 0xa7, 0x19, 0xcf
};
const uint8_t P_Prime_Exponent[] =
{
  0x1c, 0x2d, 0x1f, 0xc3, 0x2f, 0x6b, 0xc4, 0x00, 0x4f, 0xd8, 0x5d, 0xfd, 0xe0, 0xfb, 0xbf, 0x9a,
  0x4c, 0x38, 0xf9, 0xc7, 0xc4, 0xe4, 0x1d, 0xea, 0x1a, 0xa8, 0x82, 0x34, 0xa2, 0x01, 0xcd, 0x92,
  0xf3, 0xb7, 0xda, 0x52, 0x65, 0x83, 0xa9, 0x8a, 0xd8, 0x5b, 0xb3, 0x60, 0xfb, 0x98, 0x3b, 0x71,
  0x1e, 0x23, 0x44, 0x9d, 0x56, 0x1d, 0x17, 0x78, 0xd7, 0xa5, 0x15, 0x48, 0x6b, 0xcb, 0xf4, 0x7b,
  0x46, 0xc9, 0xe9, 0xe1, 0xa3, 0xa1, 0xf7, 0x70, 0x00, 0xef, 0xbe, 0xb0, 0x9a, 0x8a, 0xfe, 0x47,
  0xe5, 0xb8, 0x57, 0xcd, 0xa9, 0x9c, 0xb1, 0x6d, 0x7f, 0xff, 0x9b, 0x71, 0x2e, 0x3b, 0xd6, 0x0c,
  0xa9, 0x6d, 0x9c, 0x79, 0x73, 0xd6, 0x16, 0xd4, 0x69, 0x34, 0xa9, 0xc0, 0x50, 0x28, 0x1c, 0x00,
  0x43, 0x99, 0xce, 0xff, 0x1d, 0xb7, 0xdd, 0xa7, 0x87, 0x66, 0xa8, 0xa9, 0xb9, 0xcb, 0x08, 0x73
};
const uint8_t Q_Prime_Exponent[] =
{
  0xcb, 0x3b, 0x3c, 0x04, 0xca, 0xa5, 0x8c, 0x60, 0xbe, 0x7d, 0x9b, 0x2d, 0xeb, 0xb3, 0xe3, 0x96,
  0x43, 0xf4, 0xf5, 0x73, 0x97, 0xbe, 0x08, 0x23, 0x6a, 0x1e, 0x9e, 0xaf, 0xaa, 0x70, 0x65, 0x36,
  0xe7, 0x1c, 0x3a, 0xcf, 0xe0, 0x1c, 0xc6, 0x51, 0xf2, 0x3c, 0x9e, 0x05, 0x85, 0x8f, 0xee, 0x13,
  0xbb, 0x6a, 0x8a, 0xfc, 0x47, 0xdf, 0x4e, 0xdc, 0x9a, 0x4b, 0xa3, 0x0b, 0xce, 0xcb, 0x73, 0xd0,
  0x15, 0x78, 0x52, 0x32, 0x7e, 0xe7, 0x89, 0x01, 0x5c, 0x2e, 0x8d, 0xee, 0x7b, 0x9f, 0x05, 0xa0,
  0xf3, 0x1a, 0xc9, 0x4e, 0xb6, 0x17, 0x31, 0x64, 0x74, 0x0c, 0x5c, 0x95, 0x14, 0x7c, 0xd5, 0xf3,
  0xb5, 0xae, 0x2c, 0xb4, 0xa8, 0x37, 0x87, 0xf0, 0x1d, 0x8a, 0xb3, 0x1f, 0x27, 0xc2, 0xd0, 0xee,
  0xa2, 0xdd, 0x8a, 0x11, 0xab, 0x90, 0x6a, 0xba, 0x20, 0x7c, 0x43, 0xc6, 0xee, 0x12, 0x53, 0x31
};
const uint8_t Coefficient[] =
{
  0x12, 0xf6, 0xb2, 0xcf, 0x13, 0x74, 0xa7, 0x36, 0xfa, 0xd0, 0x56, 0x16, 0x05, 0x0f, 0x96, 0xab,
  0x4b, 0x61, 0xd1, 0x17, 0x7c, 0x7f, 0x9d, 0x52, 0x5a, 0x29, 0xf3, 0xd1, 0x80, 0xe7, 0x76, 0x67,
  0xe9, 0x9d, 0x99, 0xab, 0xf0, 0x52, 0x5d, 0x07, 0x58, 0x66, 0x0f, 0x37, 0x52, 0x65, 0x5b, 0x0f,
  0x25, 0xb8, 0xdf, 0x84, 0x31, 0xd9, 0xa8, 0xff, 0x77, 0xc1, 0x6c, 0x12, 0xa0, 0xa5, 0x12, 0x2a,
  0x9f, 0x0b, 0xf7, 0xcf, 0xd5, 0xa2, 0x66, 0xa3, 0x5c, 0x15, 0x9f, 0x99, 0x12, 0x08, 0xb9, 0x03,
  0x16, 0xff, 0x44, 0x4f, 0x3e, 0x0b, 0x6b, 0xd0, 0xe9, 0x3b, 0x8a, 0x7a, 0x24, 0x48, 0xe9, 0x57,
  0xe3, 0xdd, 0xa6, 0xcf, 0xcf, 0x22, 0x66, 0xb1, 0x06, 0x01, 0x3a, 0xc4, 0x68, 0x08, 0xd3, 0xb3,
  0x88, 0x7b, 0x3b, 0x00, 0x34, 0x4b, 0xaa, 0xc9, 0x53, 0x0b, 0x4c, 0xe7, 0x08, 0xfc, 0x32, 0xb6
};

/* Computed data buffer */
uint8_t Computed_Hash[CMOX_SHA1_SIZE];
uint8_t Computed_Signature[sizeof(Known_Signature)];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  cmox_hash_retval_t hretval;
  cmox_rsa_retval_t retval;
  size_t computed_size;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_RSA_AUTH_FAIL;

  /* STM32WBxx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the System clock */
  SystemClock_Config();


  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Initialize cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Compute directly the digest passing all the needed parameters */
  hretval = cmox_hash_compute(CMOX_SHA1_ALGO,           /* Use SHA1 algorithm */
                              Message, sizeof(Message), /* Message to digest */
                              Computed_Hash,            /* Data buffer to receive digest data */
                              CMOX_SHA1_SIZE,           /* Expected digest size */
                              &computed_size);          /* Size of computed digest */

  /* Verify API returned value */
  if (hretval != CMOX_HASH_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != CMOX_SHA1_SIZE)
  {
    Error_Handler();
  }

  /* --------------------------------------------------------------------------
   * USING REGULAR PRIVATE KEY REPRESENTATION
   * --------------------------------------------------------------------------
   */

  /* Construct a RSA context, specifying mathematics implementation and working buffer for later processing */
  /* Note: CMOX_RSA_MATH_FUNCS refer to the default mathematics implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MATH_FUNCS_SMALL to select the mathematics small implementation
   * - CMOX_MATH_FUNCS_FAST to select the mathematics fast implementation
   */
  /* Note: CMOX_MODEXP_PRIVATE refer to the default modular exponentiation implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MODEXP_PRIVATE_LOWMEM  to select the modular exponentiation low RAM usage implementation
   * - CMOX_MODEXP_PRIVATE_MIDMEM to select the modular exponentiation mid RAM usage implementation
   * - CMOX_MODEXP_PRIVATE_HIGHMEM to select the modular exponentiation high RAM usage implementation
   */
  cmox_rsa_construct(&Rsa_Ctx, CMOX_RSA_MATH_FUNCS, CMOX_MODEXP_PRIVATE, Working_Buffer, sizeof(Working_Buffer));

  /* Fill in RSA key structure using the regular private key representation */
  retval = cmox_rsa_setKey(&Rsa_Key,                                      /* RSA key structure to fill */
                           Modulus, sizeof(Modulus),                      /* Private key modulus */
                           Private_Exponent, sizeof(Private_Exponent));   /* Private key exponent */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_sign(&Rsa_Ctx,                                 /* RSA context */
                                  &Rsa_Key,                                 /* RSA key to use */
                                  Computed_Hash,                            /* Digest to sign */
                                  CMOX_RSA_PKCS1V22_HASH_SHA1,              /* Method used to compute the digest */
                                  Salt, sizeof(Salt),                       /* Random salt */
                                  Computed_Signature, &computed_size);      /* Data buffer to receive signature */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    Error_Handler();
  }

  /* Cleanup context */
  cmox_rsa_cleanup(&Rsa_Ctx);

  /* --------------------------------------------------------------------------
   * USING CHINESE REMAINDERTHEOREM (CRT) PRIVATE KEY REPRESENTATION
   * --------------------------------------------------------------------------
   */

  /* Construct a RSA context, specifying mathematics implementation and working buffer for later processing */
  /* Note: CMOX_RSA_MATH_FUNCS refer to the default mathematics implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MATH_FUNCS_SMALL to select the mathematics small implementation
   * - CMOX_MATH_FUNCS_FAST to select the mathematics fast implementation
   */
  /* Note: CMOX_MODEXP_PRIVATE refer to the default modular exponentiation implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MODEXP_PRIVATE_LOWMEM  to select the modular exponentiation low RAM usage implementation
   * - CMOX_MODEXP_PRIVATE_MIDMEM to select the modular exponentiation mid RAM usage implementation
   * - CMOX_MODEXP_PRIVATE_HIGHMEM to select the modular exponentiation high RAM usage implementation
   */
  cmox_rsa_construct(&Rsa_Ctx, CMOX_RSA_MATH_FUNCS, CMOX_MODEXP_PRIVATE, Working_Buffer, sizeof(Working_Buffer));

  /* Fill in RSA key structure using the CRT private key representation */
  retval = cmox_rsa_setKeyCRT(&Rsa_Key,                                       /* RSA key structure to fill */
                              sizeof(Modulus) * 8,                            /* Private key modulus bit length */
                              P_Prime_Exponent, sizeof(P_Prime_Exponent),     /* P prime */
                              Q_Prime_Exponent, sizeof(Q_Prime_Exponent),     /* Q prime */
                              P_Prime, sizeof(P_Prime),                       /* P prime exponent */
                              Q_Prime, sizeof(Q_Prime),                       /* Q prime exponent */
                              Coefficient, sizeof(Coefficient));              /* Coefficient */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_sign(&Rsa_Ctx,                                 /* RSA context */
                                  &Rsa_Key,                                 /* RSA key to use */
                                  Computed_Hash,                            /* Digest to sign */
                                  CMOX_RSA_PKCS1V22_HASH_SHA1,              /* Method used to compute the digest */
                                  Salt, sizeof(Salt),                       /* Random salt */
                                  Computed_Signature, &computed_size);      /* Data buffer to receive signature */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_Signature))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Signature, Known_Signature, computed_size) != 0)
  {
    Error_Handler();
  }

  /* Cleanup context */
  cmox_rsa_cleanup(&Rsa_Ctx);


  /* Construct a RSA context, specifying mathematics implementation and working buffer for later processing */
  /* Note: CMOX_RSA_MATH_FUNCS refer to the default mathematics implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_MATH_FUNCS_SMALL to select the mathematics small implementation
   * - CMOX_MATH_FUNCS_FAST to select the mathematics fast implementation
   */
  cmox_rsa_construct(&Rsa_Ctx, CMOX_RSA_MATH_FUNCS, CMOX_MODEXP_PUBLIC, Working_Buffer, sizeof(Working_Buffer));

  /* Fill in RSA key structure using the public key representation */
  retval = cmox_rsa_setKey(&Rsa_Key,                                      /* RSA key structure to fill */
                           Modulus, sizeof(Modulus),                      /* Key modulus */
                           Public_Exponent, sizeof(Public_Exponent));     /* Public key exponent */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Compute directly the signature passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_verify(&Rsa_Ctx,                                 /* RSA context */
                                    &Rsa_Key,                                 /* RSA key to use */
                                    Computed_Hash,                            /* Digest to sign */
                                    CMOX_RSA_PKCS1V22_HASH_SHA1,              /* Method used to compute the digest */
                                    sizeof(Salt),                             /* Random salt length */
                                    Known_Signature, sizeof(Known_Signature), /* Signature to verify */
                                    &fault_check);                            /* Fault check variable:
                                                             to ensure no fault injection occurs during this API call */

  /* Verify API returned value */
  if (retval != CMOX_RSA_AUTH_SUCCESS)
  {
    Error_Handler();
  }
  /* Verify Fault check variable value */
  if (fault_check != CMOX_RSA_AUTH_SUCCESS)
  {
    Error_Handler();
  }

  /* Cleanup context */
  cmox_rsa_cleanup(&Rsa_Ctx);

  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
    Error_Handler();
  }

  /* Turn on LED3 in an infinite loop in case of RSA operations are successful */
  BSP_LED_On(LED3);
  glob_status = PASSED;
  while (1)
  {}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            HCLK1 Prescaler                = 1
  *            HCKL2 Prescaler                = 2
  *            HCKLS Prescaler                = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 32
  *            PLL_P                          = 5
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 3
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct ={0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV5;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_HCLK2 | RCC_CLOCKTYPE_HCLK4);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }
}


/**
  * @brief  This function is executed in case of error occurrence
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Toggle LED3 @2Hz to notify error condition */
  while (1)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(250);
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
