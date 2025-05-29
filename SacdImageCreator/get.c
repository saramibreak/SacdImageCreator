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

int get_disc_size(char* drive, unsigned long long* disc_size)
{
	int fd = open(drive, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		perror("drive open failed");
		return 1;
	}
	if (ioctl(fd, BLKGETSIZE64, disc_size) == -1) {
		perror("ioctl BLKGETSIZE64 failed");
		close(fd);
		return 1;
    }
	close(fd);
	return 0;
}

int get_current_dir(char* curdir)
{
	FILE* fp = popen("pwd", "r");
	if (fp == NULL) {
		perror("popen failed");
		return 1;
	}
    if (fgets(curdir, 256, fp)) {
		curdir[strcspn(curdir, "\n")] = '\0';
    }
	int status = pclose(fp);
	if (status == -1) {
		perror("pclose");
		return 1;
	}
	return 0;
}
