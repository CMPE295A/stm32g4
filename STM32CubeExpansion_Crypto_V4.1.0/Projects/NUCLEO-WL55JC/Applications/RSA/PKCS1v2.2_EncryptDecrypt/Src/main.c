/**
  ******************************************************************************
  * @file    RSA/PKCS1v2.2_EncryptDecrypt/Src/main.c
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
/** Extract from pkcs-1v2-1d2-vec/oaep-vect.txt
# Example 10: A 2048-bit RSA key pair
# -----------------------------------


# Public key
# ----------

# Modulus:
ae 45 ed 56 01 ce c6 b8 cc 05 f8 03 93 5c 67 4d
db e0 d7 5c 4c 09 fd 79 51 fc 6b 0c ae c3 13 a8
df 39 97 0c 51 8b ff ba 5e d6 8f 3f 0d 7f 22 a4
02 9d 41 3f 1a e0 7e 4e be 9e 41 77 ce 23 e7 f5
40 4b 56 9e 4e e1 bd cf 3c 1f b0 3e f1 13 80 2d
4f 85 5e b9 b5 13 4b 5a 7c 80 85 ad ca e6 fa 2f
a1 41 7e c3 76 3b e1 71 b0 c6 2b 76 0e de 23 c1
2a d9 2b 98 08 84 c6 41 f5 a8 fa c2 6b da d4 a0
33 81 a2 2f e1 b7 54 88 50 94 c8 25 06 d4 01 9a
53 5a 28 6a fe b2 71 bb 9b a5 92 de 18 dc f6 00
c2 ae ea e5 6e 02 f7 cf 79 fc 14 cf 3b dc 7c d8
4f eb bb f9 50 ca 90 30 4b 22 19 a7 aa 06 3a ef
a2 c3 c1 98 0e 56 0c d6 4a fe 77 95 85 b6 10 76
57 b9 57 85 7e fd e6 01 09 88 ab 7d e4 17 fc 88
d8 f3 84 c4 e6 e7 2c 3f 94 3e 0c 31 c0 c4 a5 cc
36 f8 79 d8 a3 ac 9d 7d 59 86 0e aa da 6b 83 bb

# Exponent:
01 00 01

# Private key
# -----------

# Modulus:
ae 45 ed 56 01 ce c6 b8 cc 05 f8 03 93 5c 67 4d
db e0 d7 5c 4c 09 fd 79 51 fc 6b 0c ae c3 13 a8
df 39 97 0c 51 8b ff ba 5e d6 8f 3f 0d 7f 22 a4
02 9d 41 3f 1a e0 7e 4e be 9e 41 77 ce 23 e7 f5
40 4b 56 9e 4e e1 bd cf 3c 1f b0 3e f1 13 80 2d
4f 85 5e b9 b5 13 4b 5a 7c 80 85 ad ca e6 fa 2f
a1 41 7e c3 76 3b e1 71 b0 c6 2b 76 0e de 23 c1
2a d9 2b 98 08 84 c6 41 f5 a8 fa c2 6b da d4 a0
33 81 a2 2f e1 b7 54 88 50 94 c8 25 06 d4 01 9a
53 5a 28 6a fe b2 71 bb 9b a5 92 de 18 dc f6 00
c2 ae ea e5 6e 02 f7 cf 79 fc 14 cf 3b dc 7c d8
4f eb bb f9 50 ca 90 30 4b 22 19 a7 aa 06 3a ef
a2 c3 c1 98 0e 56 0c d6 4a fe 77 95 85 b6 10 76
57 b9 57 85 7e fd e6 01 09 88 ab 7d e4 17 fc 88
d8 f3 84 c4 e6 e7 2c 3f 94 3e 0c 31 c0 c4 a5 cc
36 f8 79 d8 a3 ac 9d 7d 59 86 0e aa da 6b 83 bb

# Public exponent:
01 00 01

# Exponent:
05 6b 04 21 6f e5 f3 54 ac 77 25 0a 4b 6b 0c 85
25 a8 5c 59 b0 bd 80 c5 64 50 a2 2d 5f 43 8e 59
6a 33 3a a8 75 e2 91 dd 43 f4 8c b8 8b 9d 5f c0
d4 99 f9 fc d1 c3 97 f9 af c0 70 cd 9e 39 8c 8d
19 e6 1d b7 c7 41 0a 6b 26 75 df bf 5d 34 5b 80
4d 20 1a dd 50 2d 5c e2 df cb 09 1c e9 99 7b be
be 57 30 6f 38 3e 4d 58 81 03 f0 36 f7 e8 5d 19
34 d1 52 a3 23 e4 a8 db 45 1d 6f 4a 5b 1b 0f 10
2c c1 50 e0 2f ee e2 b8 8d ea 4a d4 c1 ba cc b2
4d 84 07 2d 14 e1 d2 4a 67 71 f7 40 8e e3 05 64
fb 86 d4 39 3a 34 bc f0 b7 88 50 1d 19 33 03 f1
3a 22 84 b0 01 f0 f6 49 ea f7 93 28 d4 ac 5c 43
0a b4 41 49 20 a9 46 0e d1 b7 bc 40 ec 65 3e 87
6d 09 ab c5 09 ae 45 b5 25 19 01 16 a0 c2 61 01
84 82 98 50 9c 1c 3b f3 a4 83 e7 27 40 54 e1 5e
97 07 50 36 e9 89 f6 09 32 80 7b 52 57 75 1e 79

# Prime 1:
ec f5 ae cd 1e 55 15 ff fa cb d7 5a 28 16 c6 eb
f4 90 18 cd fb 46 38 e1 85 d6 6a 73 96 b6 f8 09
0f 80 18 c7 fd 95 cc 34 b8 57 dc 17 f0 cc 65 16
bb 13 46 ab 4d 58 2c ad ad 7b 41 03 35 23 87 b7
03 38 d0 84 04 7c 9d 95 39 b6 49 62 04 b3 dd 6e
a4 42 49 92 07 be c0 1f 96 42 87 ff 63 36 c3 98
46 58 33 68 46 f5 6e 46 86 18 81 c1 02 33 d2 17
6b f1 5a 5e 96 dd c7 80 bc 86 8a a7 7d 3c e7 69

# Prime 2:
bc 46 c4 64 fc 6a c4 ca 78 3b 0e b0 8a 3c 84 1b
77 2f 7e 9b 2f 28 ba bd 58 8a e8 85 e1 a0 c6 1e
48 58 a0 fb 25 ac 29 99 90 f3 5b e8 51 64 c2 59
ba 11 75 cd d7 19 27 07 13 51 84 99 2b 6c 29 b7
46 dd 0d 2c ab e1 42 83 5f 7d 14 8c c1 61 52 4b
4a 09 94 6d 48 b8 28 47 3f 1c e7 6b 6c b6 88 6c
34 5c 03 e0 5f 41 d5 1b 5c 3a 90 a3 f2 40 73 c7
d7 4a 4f e2 5d 9c f2 1c 75 96 0f 3f c3 86 31 83

# Prime exponent 1:
c7 35 64 57 1d 00 fb 15 d0 8a 3d e9 95 7a 50 91
5d 71 26 e9 44 2d ac f4 2b c8 2e 86 2e 56 73 ff
6a 00 8e d4 d2 e3 74 61 7d f8 9f 17 a1 60 b4 3b
7f da 9c b6 b6 b7 42 18 60 98 15 f7 d4 5c a2 63
c1 59 aa 32 d2 72 d1 27 fa f4 bc 8c a2 d7 73 78
e8 ae b1 9b 0a d7 da 3c b3 de 0a e7 31 49 80 f6
2b 6d 4b 0a 87 5d 1d f0 3c 1b ae 39 cc d8 33 ef
6c d7 e2 d9 52 8b f0 84 d1 f9 69 e7 94 e9 f6 c1

# Prime exponent 2:
26 58 b3 7f 6d f9 c1 03 0b e1 db 68 11 7f a9 d8
7e 39 ea 2b 69 3b 7e 6d 3a 2f 70 94 74 13 ee c6
14 2e 18 fb 8d fc b6 ac 54 5d 7c 86 a0 ad 48 f8
45 71 70 f0 ef b2 6b c4 81 26 c5 3e fd 1d 16 92
01 98 dc 2a 11 07 dc 28 2d b6 a8 0c d3 06 23 60
ba 3f a1 3f 70 e4 31 2f f1 a6 cd 6b 8f c4 cd 9c
5c 3d b1 7c 6d 6a 57 21 2f 73 ae 29 f6 19 32 7b
ad 59 b1 53 85 85 85 ba 4e 28 b6 0a 62 a4 5e 49

# Coefficient:
6f 38 52 6b 39 25 08 55 34 ef 3e 41 5a 83 6e de
8b 86 15 8a 2c 7c bf ec cb 0b d8 34 30 4f ec 68
3b a8 d4 f4 79 c4 33 d4 34 16 e6 32 69 62 3c ea
10 07 76 d8 5a ff 40 1d 3f ff 61 0e e6 54 11 ce
3b 13 63 d6 3a 97 09 ee de 42 64 7c ea 56 14 93
d5 45 70 a8 79 c1 86 82 cd 97 71 0b 96 20 5e c3
11 17 d7 3b 5f 36 22 3f ad d6 e8 ba 90 dd 7c 0e
e6 1d 44 e1 63 25 1e 20 c7 f6 6e b3 05 11 7c b8

# OAEP Example 10.4
# ------------------

# Message:
bc dd 19 0d a3 b7 d3 00 df 9a 06 e2 2c aa e2 a7
5f 10 c9 1f f6 67 b7 c1 6b de 8b 53 06 4a 26 49
a9 40 45 c9

# Seed:
5c ac a6 a0 f7 64 16 1a 96 84 f8 5d 92 b6 e0 ef
37 ca 8b 65

# Encryption:
63 18 e9 fb 5c 0d 05 e5 30 7e 16 83 43 6e 90 32
93 ac 46 42 35 8a aa 22 3d 71 63 01 3a ba 87 e2
df da 8e 60 c6 86 0e 29 a1 e9 26 86 16 3e a0 b9
17 5f 32 9c a3 b1 31 a1 ed d3 a7 77 59 a8 b9 7b
ad 6a 4f 8f 43 96 f2 8c f6 f3 9c a5 81 12 e4 81
60 d6 e2 03 da a5 85 6f 3a ca 5f fe d5 77 af 49
94 08 e3 df d2 33 e3 e6 04 db e3 4a 9c 4c 90 82
de 65 52 7c ac 63 31 d2 9d c8 0e 05 08 a0 fa 71
22 e7 f3 29 f6 cc a5 cf a3 4d 4d 1d a4 17 80 54
57 e0 08 be c5 49 e4 78 ff 9e 12 a7 63 c4 77 d1
5b bb 78 f5 b6 9b d5 78 30 fc 2c 4e d6 86 d7 9b
c7 2a 95 d8 5f 88 13 4c 6b 0a fe 56 a8 cc fb c8
55 82 8b b3 39 bd 17 90 9c f1 d7 0d e3 33 5a e0
70 39 09 3e 60 6d 65 53 65 de 65 50 b8 72 cd 6d
e1 d4 40 ee 03 1b 61 94 5f 62 9a d8 a3 53 b0 d4
09 39 e9 6a 3c 45 0d 2a 8d 5e ee 9f 67 80 93 c8



  */
const uint8_t Message[] =
{
  0xbc, 0xdd, 0x19, 0x0d, 0xa3, 0xb7, 0xd3, 0x00, 0xdf, 0x9a, 0x06, 0xe2, 0x2c, 0xaa, 0xe2, 0xa7,
  0x5f, 0x10, 0xc9, 0x1f, 0xf6, 0x67, 0xb7, 0xc1, 0x6b, 0xde, 0x8b, 0x53, 0x06, 0x4a, 0x26, 0x49,
  0xa9, 0x40, 0x45, 0xc9
};
const uint8_t Seed[] =
{
  0x5c, 0xac, 0xa6, 0xa0, 0xf7, 0x64, 0x16, 0x1a, 0x96, 0x84, 0xf8, 0x5d, 0x92, 0xb6, 0xe0, 0xef,
  0x37, 0xca, 0x8b, 0x65
};
const uint8_t Known_EncryptedText[] =
{
  0x63, 0x18, 0xe9, 0xfb, 0x5c, 0x0d, 0x05, 0xe5, 0x30, 0x7e, 0x16, 0x83, 0x43, 0x6e, 0x90, 0x32,
  0x93, 0xac, 0x46, 0x42, 0x35, 0x8a, 0xaa, 0x22, 0x3d, 0x71, 0x63, 0x01, 0x3a, 0xba, 0x87, 0xe2,
  0xdf, 0xda, 0x8e, 0x60, 0xc6, 0x86, 0x0e, 0x29, 0xa1, 0xe9, 0x26, 0x86, 0x16, 0x3e, 0xa0, 0xb9,
  0x17, 0x5f, 0x32, 0x9c, 0xa3, 0xb1, 0x31, 0xa1, 0xed, 0xd3, 0xa7, 0x77, 0x59, 0xa8, 0xb9, 0x7b,
  0xad, 0x6a, 0x4f, 0x8f, 0x43, 0x96, 0xf2, 0x8c, 0xf6, 0xf3, 0x9c, 0xa5, 0x81, 0x12, 0xe4, 0x81,
  0x60, 0xd6, 0xe2, 0x03, 0xda, 0xa5, 0x85, 0x6f, 0x3a, 0xca, 0x5f, 0xfe, 0xd5, 0x77, 0xaf, 0x49,
  0x94, 0x08, 0xe3, 0xdf, 0xd2, 0x33, 0xe3, 0xe6, 0x04, 0xdb, 0xe3, 0x4a, 0x9c, 0x4c, 0x90, 0x82,
  0xde, 0x65, 0x52, 0x7c, 0xac, 0x63, 0x31, 0xd2, 0x9d, 0xc8, 0x0e, 0x05, 0x08, 0xa0, 0xfa, 0x71,
  0x22, 0xe7, 0xf3, 0x29, 0xf6, 0xcc, 0xa5, 0xcf, 0xa3, 0x4d, 0x4d, 0x1d, 0xa4, 0x17, 0x80, 0x54,
  0x57, 0xe0, 0x08, 0xbe, 0xc5, 0x49, 0xe4, 0x78, 0xff, 0x9e, 0x12, 0xa7, 0x63, 0xc4, 0x77, 0xd1,
  0x5b, 0xbb, 0x78, 0xf5, 0xb6, 0x9b, 0xd5, 0x78, 0x30, 0xfc, 0x2c, 0x4e, 0xd6, 0x86, 0xd7, 0x9b,
  0xc7, 0x2a, 0x95, 0xd8, 0x5f, 0x88, 0x13, 0x4c, 0x6b, 0x0a, 0xfe, 0x56, 0xa8, 0xcc, 0xfb, 0xc8,
  0x55, 0x82, 0x8b, 0xb3, 0x39, 0xbd, 0x17, 0x90, 0x9c, 0xf1, 0xd7, 0x0d, 0xe3, 0x33, 0x5a, 0xe0,
  0x70, 0x39, 0x09, 0x3e, 0x60, 0x6d, 0x65, 0x53, 0x65, 0xde, 0x65, 0x50, 0xb8, 0x72, 0xcd, 0x6d,
  0xe1, 0xd4, 0x40, 0xee, 0x03, 0x1b, 0x61, 0x94, 0x5f, 0x62, 0x9a, 0xd8, 0xa3, 0x53, 0xb0, 0xd4,
  0x09, 0x39, 0xe9, 0x6a, 0x3c, 0x45, 0x0d, 0x2a, 0x8d, 0x5e, 0xee, 0x9f, 0x67, 0x80, 0x93, 0xc8
};

const uint8_t Modulus[] =
{
  0xae, 0x45, 0xed, 0x56, 0x01, 0xce, 0xc6, 0xb8, 0xcc, 0x05, 0xf8, 0x03, 0x93, 0x5c, 0x67, 0x4d,
  0xdb, 0xe0, 0xd7, 0x5c, 0x4c, 0x09, 0xfd, 0x79, 0x51, 0xfc, 0x6b, 0x0c, 0xae, 0xc3, 0x13, 0xa8,
  0xdf, 0x39, 0x97, 0x0c, 0x51, 0x8b, 0xff, 0xba, 0x5e, 0xd6, 0x8f, 0x3f, 0x0d, 0x7f, 0x22, 0xa4,
  0x02, 0x9d, 0x41, 0x3f, 0x1a, 0xe0, 0x7e, 0x4e, 0xbe, 0x9e, 0x41, 0x77, 0xce, 0x23, 0xe7, 0xf5,
  0x40, 0x4b, 0x56, 0x9e, 0x4e, 0xe1, 0xbd, 0xcf, 0x3c, 0x1f, 0xb0, 0x3e, 0xf1, 0x13, 0x80, 0x2d,
  0x4f, 0x85, 0x5e, 0xb9, 0xb5, 0x13, 0x4b, 0x5a, 0x7c, 0x80, 0x85, 0xad, 0xca, 0xe6, 0xfa, 0x2f,
  0xa1, 0x41, 0x7e, 0xc3, 0x76, 0x3b, 0xe1, 0x71, 0xb0, 0xc6, 0x2b, 0x76, 0x0e, 0xde, 0x23, 0xc1,
  0x2a, 0xd9, 0x2b, 0x98, 0x08, 0x84, 0xc6, 0x41, 0xf5, 0xa8, 0xfa, 0xc2, 0x6b, 0xda, 0xd4, 0xa0,
  0x33, 0x81, 0xa2, 0x2f, 0xe1, 0xb7, 0x54, 0x88, 0x50, 0x94, 0xc8, 0x25, 0x06, 0xd4, 0x01, 0x9a,
  0x53, 0x5a, 0x28, 0x6a, 0xfe, 0xb2, 0x71, 0xbb, 0x9b, 0xa5, 0x92, 0xde, 0x18, 0xdc, 0xf6, 0x00,
  0xc2, 0xae, 0xea, 0xe5, 0x6e, 0x02, 0xf7, 0xcf, 0x79, 0xfc, 0x14, 0xcf, 0x3b, 0xdc, 0x7c, 0xd8,
  0x4f, 0xeb, 0xbb, 0xf9, 0x50, 0xca, 0x90, 0x30, 0x4b, 0x22, 0x19, 0xa7, 0xaa, 0x06, 0x3a, 0xef,
  0xa2, 0xc3, 0xc1, 0x98, 0x0e, 0x56, 0x0c, 0xd6, 0x4a, 0xfe, 0x77, 0x95, 0x85, 0xb6, 0x10, 0x76,
  0x57, 0xb9, 0x57, 0x85, 0x7e, 0xfd, 0xe6, 0x01, 0x09, 0x88, 0xab, 0x7d, 0xe4, 0x17, 0xfc, 0x88,
  0xd8, 0xf3, 0x84, 0xc4, 0xe6, 0xe7, 0x2c, 0x3f, 0x94, 0x3e, 0x0c, 0x31, 0xc0, 0xc4, 0xa5, 0xcc,
  0x36, 0xf8, 0x79, 0xd8, 0xa3, 0xac, 0x9d, 0x7d, 0x59, 0x86, 0x0e, 0xaa, 0xda, 0x6b, 0x83, 0xbb
};
const uint8_t Public_Exponent[] =
{
  0x01, 0x00, 0x01
};
const uint8_t Private_Exponent[] =
{
  0x05, 0x6b, 0x04, 0x21, 0x6f, 0xe5, 0xf3, 0x54, 0xac, 0x77, 0x25, 0x0a, 0x4b, 0x6b, 0x0c, 0x85,
  0x25, 0xa8, 0x5c, 0x59, 0xb0, 0xbd, 0x80, 0xc5, 0x64, 0x50, 0xa2, 0x2d, 0x5f, 0x43, 0x8e, 0x59,
  0x6a, 0x33, 0x3a, 0xa8, 0x75, 0xe2, 0x91, 0xdd, 0x43, 0xf4, 0x8c, 0xb8, 0x8b, 0x9d, 0x5f, 0xc0,
  0xd4, 0x99, 0xf9, 0xfc, 0xd1, 0xc3, 0x97, 0xf9, 0xaf, 0xc0, 0x70, 0xcd, 0x9e, 0x39, 0x8c, 0x8d,
  0x19, 0xe6, 0x1d, 0xb7, 0xc7, 0x41, 0x0a, 0x6b, 0x26, 0x75, 0xdf, 0xbf, 0x5d, 0x34, 0x5b, 0x80,
  0x4d, 0x20, 0x1a, 0xdd, 0x50, 0x2d, 0x5c, 0xe2, 0xdf, 0xcb, 0x09, 0x1c, 0xe9, 0x99, 0x7b, 0xbe,
  0xbe, 0x57, 0x30, 0x6f, 0x38, 0x3e, 0x4d, 0x58, 0x81, 0x03, 0xf0, 0x36, 0xf7, 0xe8, 0x5d, 0x19,
  0x34, 0xd1, 0x52, 0xa3, 0x23, 0xe4, 0xa8, 0xdb, 0x45, 0x1d, 0x6f, 0x4a, 0x5b, 0x1b, 0x0f, 0x10,
  0x2c, 0xc1, 0x50, 0xe0, 0x2f, 0xee, 0xe2, 0xb8, 0x8d, 0xea, 0x4a, 0xd4, 0xc1, 0xba, 0xcc, 0xb2,
  0x4d, 0x84, 0x07, 0x2d, 0x14, 0xe1, 0xd2, 0x4a, 0x67, 0x71, 0xf7, 0x40, 0x8e, 0xe3, 0x05, 0x64,
  0xfb, 0x86, 0xd4, 0x39, 0x3a, 0x34, 0xbc, 0xf0, 0xb7, 0x88, 0x50, 0x1d, 0x19, 0x33, 0x03, 0xf1,
  0x3a, 0x22, 0x84, 0xb0, 0x01, 0xf0, 0xf6, 0x49, 0xea, 0xf7, 0x93, 0x28, 0xd4, 0xac, 0x5c, 0x43,
  0x0a, 0xb4, 0x41, 0x49, 0x20, 0xa9, 0x46, 0x0e, 0xd1, 0xb7, 0xbc, 0x40, 0xec, 0x65, 0x3e, 0x87,
  0x6d, 0x09, 0xab, 0xc5, 0x09, 0xae, 0x45, 0xb5, 0x25, 0x19, 0x01, 0x16, 0xa0, 0xc2, 0x61, 0x01,
  0x84, 0x82, 0x98, 0x50, 0x9c, 0x1c, 0x3b, 0xf3, 0xa4, 0x83, 0xe7, 0x27, 0x40, 0x54, 0xe1, 0x5e,
  0x97, 0x07, 0x50, 0x36, 0xe9, 0x89, 0xf6, 0x09, 0x32, 0x80, 0x7b, 0x52, 0x57, 0x75, 0x1e, 0x79
};
const uint8_t P_Prime[] =
{
  0xec, 0xf5, 0xae, 0xcd, 0x1e, 0x55, 0x15, 0xff, 0xfa, 0xcb, 0xd7, 0x5a, 0x28, 0x16, 0xc6, 0xeb,
  0xf4, 0x90, 0x18, 0xcd, 0xfb, 0x46, 0x38, 0xe1, 0x85, 0xd6, 0x6a, 0x73, 0x96, 0xb6, 0xf8, 0x09,
  0x0f, 0x80, 0x18, 0xc7, 0xfd, 0x95, 0xcc, 0x34, 0xb8, 0x57, 0xdc, 0x17, 0xf0, 0xcc, 0x65, 0x16,
  0xbb, 0x13, 0x46, 0xab, 0x4d, 0x58, 0x2c, 0xad, 0xad, 0x7b, 0x41, 0x03, 0x35, 0x23, 0x87, 0xb7,
  0x03, 0x38, 0xd0, 0x84, 0x04, 0x7c, 0x9d, 0x95, 0x39, 0xb6, 0x49, 0x62, 0x04, 0xb3, 0xdd, 0x6e,
  0xa4, 0x42, 0x49, 0x92, 0x07, 0xbe, 0xc0, 0x1f, 0x96, 0x42, 0x87, 0xff, 0x63, 0x36, 0xc3, 0x98,
  0x46, 0x58, 0x33, 0x68, 0x46, 0xf5, 0x6e, 0x46, 0x86, 0x18, 0x81, 0xc1, 0x02, 0x33, 0xd2, 0x17,
  0x6b, 0xf1, 0x5a, 0x5e, 0x96, 0xdd, 0xc7, 0x80, 0xbc, 0x86, 0x8a, 0xa7, 0x7d, 0x3c, 0xe7, 0x69
};
const uint8_t Q_Prime[] =
{
  0xbc, 0x46, 0xc4, 0x64, 0xfc, 0x6a, 0xc4, 0xca, 0x78, 0x3b, 0x0e, 0xb0, 0x8a, 0x3c, 0x84, 0x1b,
  0x77, 0x2f, 0x7e, 0x9b, 0x2f, 0x28, 0xba, 0xbd, 0x58, 0x8a, 0xe8, 0x85, 0xe1, 0xa0, 0xc6, 0x1e,
  0x48, 0x58, 0xa0, 0xfb, 0x25, 0xac, 0x29, 0x99, 0x90, 0xf3, 0x5b, 0xe8, 0x51, 0x64, 0xc2, 0x59,
  0xba, 0x11, 0x75, 0xcd, 0xd7, 0x19, 0x27, 0x07, 0x13, 0x51, 0x84, 0x99, 0x2b, 0x6c, 0x29, 0xb7,
  0x46, 0xdd, 0x0d, 0x2c, 0xab, 0xe1, 0x42, 0x83, 0x5f, 0x7d, 0x14, 0x8c, 0xc1, 0x61, 0x52, 0x4b,
  0x4a, 0x09, 0x94, 0x6d, 0x48, 0xb8, 0x28, 0x47, 0x3f, 0x1c, 0xe7, 0x6b, 0x6c, 0xb6, 0x88, 0x6c,
  0x34, 0x5c, 0x03, 0xe0, 0x5f, 0x41, 0xd5, 0x1b, 0x5c, 0x3a, 0x90, 0xa3, 0xf2, 0x40, 0x73, 0xc7,
  0xd7, 0x4a, 0x4f, 0xe2, 0x5d, 0x9c, 0xf2, 0x1c, 0x75, 0x96, 0x0f, 0x3f, 0xc3, 0x86, 0x31, 0x83
};
const uint8_t P_Prime_Exponent[] =
{
  0xc7, 0x35, 0x64, 0x57, 0x1d, 0x00, 0xfb, 0x15, 0xd0, 0x8a, 0x3d, 0xe9, 0x95, 0x7a, 0x50, 0x91,
  0x5d, 0x71, 0x26, 0xe9, 0x44, 0x2d, 0xac, 0xf4, 0x2b, 0xc8, 0x2e, 0x86, 0x2e, 0x56, 0x73, 0xff,
  0x6a, 0x00, 0x8e, 0xd4, 0xd2, 0xe3, 0x74, 0x61, 0x7d, 0xf8, 0x9f, 0x17, 0xa1, 0x60, 0xb4, 0x3b,
  0x7f, 0xda, 0x9c, 0xb6, 0xb6, 0xb7, 0x42, 0x18, 0x60, 0x98, 0x15, 0xf7, 0xd4, 0x5c, 0xa2, 0x63,
  0xc1, 0x59, 0xaa, 0x32, 0xd2, 0x72, 0xd1, 0x27, 0xfa, 0xf4, 0xbc, 0x8c, 0xa2, 0xd7, 0x73, 0x78,
  0xe8, 0xae, 0xb1, 0x9b, 0x0a, 0xd7, 0xda, 0x3c, 0xb3, 0xde, 0x0a, 0xe7, 0x31, 0x49, 0x80, 0xf6,
  0x2b, 0x6d, 0x4b, 0x0a, 0x87, 0x5d, 0x1d, 0xf0, 0x3c, 0x1b, 0xae, 0x39, 0xcc, 0xd8, 0x33, 0xef,
  0x6c, 0xd7, 0xe2, 0xd9, 0x52, 0x8b, 0xf0, 0x84, 0xd1, 0xf9, 0x69, 0xe7, 0x94, 0xe9, 0xf6, 0xc1
};
const uint8_t Q_Prime_Exponent[] =
{
  0x26, 0x58, 0xb3, 0x7f, 0x6d, 0xf9, 0xc1, 0x03, 0x0b, 0xe1, 0xdb, 0x68, 0x11, 0x7f, 0xa9, 0xd8,
  0x7e, 0x39, 0xea, 0x2b, 0x69, 0x3b, 0x7e, 0x6d, 0x3a, 0x2f, 0x70, 0x94, 0x74, 0x13, 0xee, 0xc6,
  0x14, 0x2e, 0x18, 0xfb, 0x8d, 0xfc, 0xb6, 0xac, 0x54, 0x5d, 0x7c, 0x86, 0xa0, 0xad, 0x48, 0xf8,
  0x45, 0x71, 0x70, 0xf0, 0xef, 0xb2, 0x6b, 0xc4, 0x81, 0x26, 0xc5, 0x3e, 0xfd, 0x1d, 0x16, 0x92,
  0x01, 0x98, 0xdc, 0x2a, 0x11, 0x07, 0xdc, 0x28, 0x2d, 0xb6, 0xa8, 0x0c, 0xd3, 0x06, 0x23, 0x60,
  0xba, 0x3f, 0xa1, 0x3f, 0x70, 0xe4, 0x31, 0x2f, 0xf1, 0xa6, 0xcd, 0x6b, 0x8f, 0xc4, 0xcd, 0x9c,
  0x5c, 0x3d, 0xb1, 0x7c, 0x6d, 0x6a, 0x57, 0x21, 0x2f, 0x73, 0xae, 0x29, 0xf6, 0x19, 0x32, 0x7b,
  0xad, 0x59, 0xb1, 0x53, 0x85, 0x85, 0x85, 0xba, 0x4e, 0x28, 0xb6, 0x0a, 0x62, 0xa4, 0x5e, 0x49
};
const uint8_t Coefficient[] =
{
  0x6f, 0x38, 0x52, 0x6b, 0x39, 0x25, 0x08, 0x55, 0x34, 0xef, 0x3e, 0x41, 0x5a, 0x83, 0x6e, 0xde,
  0x8b, 0x86, 0x15, 0x8a, 0x2c, 0x7c, 0xbf, 0xec, 0xcb, 0x0b, 0xd8, 0x34, 0x30, 0x4f, 0xec, 0x68,
  0x3b, 0xa8, 0xd4, 0xf4, 0x79, 0xc4, 0x33, 0xd4, 0x34, 0x16, 0xe6, 0x32, 0x69, 0x62, 0x3c, 0xea,
  0x10, 0x07, 0x76, 0xd8, 0x5a, 0xff, 0x40, 0x1d, 0x3f, 0xff, 0x61, 0x0e, 0xe6, 0x54, 0x11, 0xce,
  0x3b, 0x13, 0x63, 0xd6, 0x3a, 0x97, 0x09, 0xee, 0xde, 0x42, 0x64, 0x7c, 0xea, 0x56, 0x14, 0x93,
  0xd5, 0x45, 0x70, 0xa8, 0x79, 0xc1, 0x86, 0x82, 0xcd, 0x97, 0x71, 0x0b, 0x96, 0x20, 0x5e, 0xc3,
  0x11, 0x17, 0xd7, 0x3b, 0x5f, 0x36, 0x22, 0x3f, 0xad, 0xd6, 0xe8, 0xba, 0x90, 0xdd, 0x7c, 0x0e,
  0xe6, 0x1d, 0x44, 0xe1, 0x63, 0x25, 0x1e, 0x20, 0xc7, 0xf6, 0x6e, 0xb3, 0x05, 0x11, 0x7c, 0xb8
};

/* Computed data buffer */
uint8_t Computed_Encryption[sizeof(Known_EncryptedText)];
uint8_t Computed_Text[sizeof(Message)];

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
  cmox_rsa_retval_t retval;
  size_t computed_size;

  /* STM32WLxx HAL library initialization:
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

  /* Compute directly the clear message passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_decrypt(&Rsa_Ctx,                                           /* RSA context */
                                     &Rsa_Key,                                           /* RSA key to use */
                                     Known_EncryptedText, sizeof(Known_EncryptedText),   /* Encrypted message */
                                     CMOX_RSA_PKCS1V22_HASH_SHA1,                        /* Hash method to use */
                                     NULL, 0,                                            /* No label */
                                     Computed_Text, &computed_size);   /* Data buffer to receive clear message */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Message))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Text, Message, computed_size) != 0)
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

  /* Compute directly the clear message passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_decrypt(&Rsa_Ctx,                                           /* RSA context */
                                     &Rsa_Key,                                           /* RSA key to use */
                                     Known_EncryptedText, sizeof(Known_EncryptedText),   /* Encrypted message */
                                     CMOX_RSA_PKCS1V22_HASH_SHA1,                        /* Hash method to use */
                                     NULL, 0,                                            /* No label */
                                     Computed_Text, &computed_size);   /* Data buffer to receive clear message */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Message))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Text, Message, computed_size) != 0)
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

  /* Compute directly the encrypted message passing all the needed parameters */
  retval = cmox_rsa_pkcs1v22_encrypt(&Rsa_Ctx,                                 /* RSA context */
                                     &Rsa_Key,                                 /* RSA key to use */
                                     Message, sizeof(Message),                 /* Message to encrypt */
                                     CMOX_RSA_PKCS1V22_HASH_SHA1,              /* Hash method to use */
                                     Seed, sizeof(Seed),                       /* Random seed */
                                     NULL, 0,                                  /* No label */
                                     Computed_Encryption, &computed_size);  /* Data buffer to receive encrypted text */

  /* Verify API returned value */
  if (retval != CMOX_RSA_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Known_EncryptedText))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Computed_Encryption, Known_EncryptedText, computed_size) != 0)
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
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            HCLK1 Prescaler                = 1
  *            HCKL2 Prescaler                = 1
  *            HCKL3 Prescaler                = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 1
  *            PLL_N                          = 6
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            PLL_R                          = 2
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 6;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while (1);
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
