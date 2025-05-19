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
#include "sacd_read_ioctl.h"

#define SACD_IOCTL_INIT		_IOW('S', 1, sacd_ioctl_buffer_t)
#define SACD_IOCTL_AUTH		_IOW('S', 2, sacd_ioctl_buffer_t)
#define SACD_IOCTL_READ		_IOW('S', 3, sacd_ioctl_buffer_t)
#define SACD_IOCTL_DECRYPT	_IOW('S', 4, sacd_ioctl_buffer_t)
#define SACD_IOCTL_UNINIT	_IOW('S', 5, sacd_ioctl_buffer_t)
#define SACD_IOCTL_DISC		_IOW('S', 6, sacd_ioctl_buffer_t)
#define SACD_IOCTL_SACD		_IOW('S', 7, sacd_ioctl_buffer_t)

int init_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
	if (ioctl(fd, SACD_IOCTL_INIT, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
#ifdef DEBUG
	FILE* fp = fopen("/mnt/sda1/AutoScript/iolog.txt", "w");
	if (!fp) {
		perror("fopen error\n");
		return -1;
	}
	fprintf(fp, "init_sacd: user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
	fclose(fp);
#endif
	return 0;
}

int auth_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
	if (ioctl(fd, SACD_IOCTL_AUTH, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
#ifdef DEBUG
	FILE* fp = fopen("/mnt/sda1/AutoScript/iolog.txt", "a");
	if (!fp) {
		printf("fopen error\n");
		return -1;
	}
	fprintf(fp, "auth_sacd: user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
	fclose(fp);
#endif
	return 0;
}

int read_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
#ifdef DEBUG
	FILE* fp = fopen("/mnt/sda1/AutoScript/iolog.txt", "a");
	if (!fp) {
		printf("fopen error\n");
		return -1;
	}
	fprintf(fp, "   read_sacd_before: user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
#endif

	if (ioctl(fd, SACD_IOCTL_READ, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
#ifdef DEBUG
	fprintf(fp, "   read_sacd_after : user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
	fclose(fp);
#endif
	return ioctl_buf->param2;
}

int decrypt_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
#ifdef DEBUG
	FILE* fp = fopen("/mnt/sda1/AutoScript/iolog.txt", "a");
	if (!fp) {
		printf("fopen error\n");
		return -1;
	}
	fprintf(fp, "decrypt_sacd_before: user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
#endif

	if (ioctl(fd, SACD_IOCTL_DECRYPT, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
#ifdef DEBUG
	fprintf(fp, "decrypt_sacd_after : user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
	fclose(fp);
#endif
	return 0;
}

int uninit_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
	if (ioctl(fd, SACD_IOCTL_UNINIT, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
#ifdef DEBUG
	FILE* fp = fopen("/mnt/sda1/AutoScript/iolog.txt", "a");
	if (!fp) {
		printf("fopen error\n");
		return -1;
	}
	fprintf(fp, "uninit_sacd: user_ptr:%x, count:%x, param1:%x, param2:%x\n"
		, ioctl_buf->user_ptr, ioctl_buf->count, ioctl_buf->param1, ioctl_buf->param2);
	fclose(fp);
#endif
	return 0;
}

int disc_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
	if (ioctl(fd, SACD_IOCTL_DISC, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
	return 0;
}

int sacd_sacd(int fd, sacd_ioctl_buffer_t* ioctl_buf)
{
	if (ioctl(fd, SACD_IOCTL_SACD, ioctl_buf) < 0) {
		perror("ioctl");
		return -1;
	}
	return 0;
}
