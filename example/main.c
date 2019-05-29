/*
 * main.c - example of using em_inflate to decompress a gzip file
 *
 * Copyright (C) 2019 Emmanuel Marty
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "em_inflate.h"

int main(int argc, char **argv) {
   unsigned char *pCompressedData, *pOutData;
   size_t nCompressedDataSize, nMaxOutDataSize, nActualOutDataSize;
   FILE *f;

   if (argc != 3) {
      fprintf(stderr, "usage: %s [infile.gz] [outfile]\n", argv[0]);
      return 100;
   }

   /* Load compressed file in memory all at once */

   f = fopen(argv[1], "rb");
   if (!f) {
      fprintf(stderr, "error opening '%s' for reading\n", argv[1]);
      return 100;
   }

   fseek(f, 0, SEEK_END);
   nCompressedDataSize = (size_t)ftell(f);
   fseek(f, 0, SEEK_SET);

   pCompressedData = (unsigned char*)malloc(nCompressedDataSize);
   if (!pCompressedData) {
      fclose(f);
      fprintf(stderr, "out of memory, %zd bytes needed\n", nCompressedDataSize);
      return 100;
   }

   memset(pCompressedData, 0, nCompressedDataSize);
   if (fread(pCompressedData, 1, nCompressedDataSize, f) != nCompressedDataSize) {
      free(pCompressedData);
      fclose(f);
      fprintf(stderr, "I/O error reading '%s'\n", argv[1]);
      return 100;
   }

   fclose(f);
   f = NULL;

   /* Decompress */

   nMaxOutDataSize = 200000000;
   pOutData = (unsigned char*)malloc(nMaxOutDataSize);
   if (!pOutData) {
      fprintf(stderr, "out of memory, %zd bytes needed\n", nMaxOutDataSize);
      free(pCompressedData);
      return 100;
   }

   nActualOutDataSize = em_inflate(pCompressedData, nCompressedDataSize, pOutData, nMaxOutDataSize);
   if (nActualOutDataSize == -1) {
      fprintf(stderr, "decompression error\n");
      free(pOutData);
      free(pCompressedData);
      return 100;
   }

   fprintf(stdout, "decompressed %zd bytes\n", nActualOutDataSize);

   /* Write decompressed file out all at once */

   f = fopen(argv[2], "wb");
   if (!f) {
      free(pOutData);
      free(pCompressedData);
      fprintf(stderr, "error opening '%s' for writing\n", argv[1]);
      return 100;
   }

   if (fwrite(pOutData, 1, nActualOutDataSize, f) != nActualOutDataSize) {
      fclose(f);
      free(pOutData);
      free(pCompressedData);
      fprintf(stderr, "I/O error writing '%s'\n", argv[1]);
      return 100;
   }

   fclose(f);
   free(pOutData);
   free(pCompressedData);

   return 0;
}