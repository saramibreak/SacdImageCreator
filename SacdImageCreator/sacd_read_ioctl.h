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
#include "common.h"

int init_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int auth_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int read_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int decrypt_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int uninit_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int disc_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
int sacd_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf);
