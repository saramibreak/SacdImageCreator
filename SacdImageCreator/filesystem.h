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
#include <stdio.h>

typedef struct _TOC {
	unsigned char Disc_Catalog_Number[16];
} TOC;

void OutputSACDHeader(
	FILE* fpHeader,
	unsigned char* lpBuf,
	unsigned int* lpSectorNumber,
	unsigned int* lpEdc,
	int nLBA
);

int ReadSACDFileSystem(
	int fd,
	FILE* fpLog,
	FILE* fpSector,
	TOC* pToc
);
