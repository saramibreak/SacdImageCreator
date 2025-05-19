/**
 * Copyright 2025 sarami
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE
#define DISC_SECTOR_SIZE 2048
#define DISC_RAW_SECTOR_SIZE 2064

#define MAKEWORD(a, b)            \
                      ((b) << 8 | \
                      ((a)))
#define MAKEUINT(a, b, c, d)               \
                            (((d) << 24) | \
                             ((c) << 16) | \
                             ((b) <<  8) | \
                             ((a)))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <errno.h>
#include <linux/fs.h>
#include <limits.h>

typedef struct {
    unsigned int param2;    // Often used as a return value or internal status
    unsigned int param1;    // May be used as flags, control codes, or user-defined values
    unsigned int count;     // number of blocks
    unsigned int user_ptr;  // Pointer to user-space memory (source/destination buffer)
} sacd_ioctl_buffer_t;

#endif
