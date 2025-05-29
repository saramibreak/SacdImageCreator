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

int init_module(char* curdir)
{
	char insmod_cmd[256] = {};
	snprintf(insmod_cmd, sizeof(insmod_cmd), "insmod %s/sacd_read.ko minor=201", curdir);
	int status = system(insmod_cmd);
	if (status == -1) {
		perror("system");
		return 1;
	}
	else if (WIFEXITED(status)) {
		int code = WEXITSTATUS(status);
		if (code == 0) {
			printf("Module loaded successfully\n");
		}
		else {
			printf("insmod failed with exit code %d\n", code);
			return 1;
		}
	}
	else if (WIFSIGNALED(status)) {
		printf("insmod was killed by signal %d\n", WTERMSIG(status));
		return 1;
	}

	if (system("mknod -m0666 /dev/sacd_read c 10 201") != 0) {
		perror("mknod failed");
		return 1;
	}
#ifdef DEBUG
	system("ls -lR / > ls_all_after_insmod.txt 2>&1");
	system("lsmod > lsmod_after_insmod.txt 2>&1");
#endif
	return 0;
}

int terminate_module(void)
{
	int status = system("rmmod sacd_read");
	if (status == -1) {
		perror("system");
		return 1;
	}
	else if (WIFEXITED(status)) {
		int code = WEXITSTATUS(status);
		if (code == 0) {
			 printf("Module unloaded successfully\n");
		}
		else {
			 printf("rmmod failed with exit code %d\n", code);
			return 1;
		}
	}
	else if (WIFSIGNALED(status)) {
		printf("rmmod was killed by signal %d\n", WTERMSIG(status));
		return 1;
	}

	status = system("rm /dev/sacd_read");
	if (status == -1) {
		perror("system");
		return 1;
	}
	else if (WIFEXITED(status)) {
		int code = WEXITSTATUS(status);
		if (code == 0) {
			 printf("/dev/sacd_read deleted successfully\n");
		}
		else {
			 printf("rm failed with exit code %d\n", code);
			return 1;
		}
	}
	else if (WIFSIGNALED(status)) {
		printf("rm was killed by signal %d\n", WTERMSIG(status));
		return 1;
	}
	return 0;
}
