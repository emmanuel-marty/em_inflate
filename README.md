em_inflate -- Fast, in-memory, single file decompressor for gzip and zlib
=========================================================================

em_inflate is a one-file, single function call, memory-to-memory decompressor for the gzip and zlib bitstream formats:

[RFC 1950: ZLIB specification](https://www.ietf.org/rfc/rfc1950.txt)

[RFC 1952: GZIP specification](https://www.ietf.org/rfc/rfc1952.txt)

You just need to add em_inflate.c and em_inflate.h to your project and call em_inflate() to decompress some data.

em_inflate is less than 1,200 lines of C (including 300 lines of two different flavors of checksums), and decompresses faster than zlib.

enwik8 decompression (gzip bitstream, 100,000,000 bytes compressed to 36,445,248 with gzip -9):

    Decompressor                      Time (microseconds), core i7
    zlib inflate 1.2.11               426,167 (100%)
    em_inflate (verify checksum)      358,118 (84%)
    em_inflate (skip checksum)        275,847 (64%)

large raw texture decompression (zlib bitstream, 32,212,992 bytes compressed to 13,721,537 with gzip -9):

    Decompressor                      Time (microseconds), core i7
    zlib inflate 1.2.11               127,945 (100%)
    em_inflate (verify checksum)      114,369 (89%)
    em_inflate (skip checksum)        105,476 (82%)

Code size (linux x86_64, clang 6.0.0 -O3, most of the rodata is checksum tables)

    With checksums                    10,334 (code) + 4,624 (rodata)  14,958
    Without checksums                 8,510 (code) + 518 (rodata)     9,028

em_inflate is developed by Emmanuel Marty. gzip crc32 checksum by Stephen Brumme. adler checksum computation by Mark Adler. All code is licensed
under the zlib license, with the gzip crc32 code under a zlib-like license. All mentions are included in the sourcecode.

