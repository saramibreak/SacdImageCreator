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
#include "filesystem.h"
#include "get.h"
#include "module.h"
#include "sacd_read_ioctl.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int main(int argc, char **argv)
{
	if (argc < 3) {
		perror("argv");
		return 1;
	}

	char curdir[256] = {};
	if (get_current_dir(curdir) != 0) {
		return 1;
	}
	
	if (init_module(curdir) != 0) {
		return 1;
	}

	unsigned long long disc_size = 0;
	if (get_disc_size(argv[1], &disc_size) != 0) {
		terminate_module();
		return 1;
	}

	char filesystem[256] = {};
	snprintf(filesystem, sizeof(filesystem), "%s/filesystem.txt", curdir);

	FILE* fpLog = fopen(filesystem, "w");
	if (!fpLog) {
		perror("fpLog failed");
		terminate_module();
		return 1;
	}
	unsigned int all_sector = (unsigned int)(disc_size / DISC_SECTOR_SIZE);
	fprintf(fpLog, "Disc size: %llu bytes, %u sectors\n", disc_size, all_sector);

	int fdsacd = open("/dev/sacd_read", O_RDONLY);
	if (fdsacd < 0) {
		perror("/dev/sacd_read open failed");
		fclose(fpLog);
		terminate_module();
		return 1;
	}
	sacd_ioctl_buffer_t ioctl_buf = {0, 0, 0, 0};
	if (init_sacd(fdsacd, &ioctl_buf) < 0) {
		perror("init_sacd failed");
		fclose(fpLog);
		close(fdsacd);
		terminate_module();
		return 1;
	}
	if (auth_sacd(fdsacd, &ioctl_buf) < 0) {
		perror("auth_sacd failed");
		fclose(fpLog);
		close(fdsacd);
		terminate_module();
		return 1;
	}

	char sector[256] = {};
	snprintf(sector, sizeof(sector), "%s/sector.txt", curdir);

	FILE* fpSector = fopen(sector, "w");
	if (!fpSector) {
		perror("fpSector failed");
		fclose(fpLog);
		close(fdsacd);
		terminate_module();
		return 1;
	}
	TOC toc[2];
	if (ReadSACDFileSystem(fdsacd, fpLog, fpSector, toc) < 0) {
		perror("ReadSACDFileSystem failed");
		fclose(fpLog);
		fclose(fpSector);
		close(fdsacd);
		terminate_module();
        return 1;
	}
	fclose(fpLog);
	fclose(fpSector);

	char filesystem_new[256] = {};
	snprintf(filesystem_new, sizeof(filesystem_new), "%s/%s_filesystem.txt", curdir, toc[0].Disc_Catalog_Number);
	rename(filesystem, filesystem_new);

	char sector_new[256] = {};
	snprintf(sector_new, sizeof(sector_new), "%s/%s_sector.txt", curdir, toc[0].Disc_Catalog_Number);
	rename(sector, sector_new);

	char* endptr = NULL;
	unsigned int MAX_BLOCK_SIZE = strtoul(argv[2], &endptr, 10);
	if (*endptr) {
		perror("strtoul failed");
		close(fdsacd);
		terminate_module();
		return 1;
	}
	unsigned int block_size = MAX_BLOCK_SIZE;

	void* data_buffer = malloc(block_size * DISC_RAW_SECTOR_SIZE);
	if (!data_buffer) {
		perror("malloc failed");
		close(fdsacd);
		terminate_module();
		return 1;
	}
	memset(data_buffer, 0, block_size * DISC_RAW_SECTOR_SIZE);

	void* raw_buffer = malloc(block_size * DISC_RAW_SECTOR_SIZE);
	if (!raw_buffer) {
		perror("malloc failed");
		free(data_buffer);
		close(fdsacd);
		terminate_module();
		return 1;
	}
	memset(raw_buffer, 0, block_size * DISC_RAW_SECTOR_SIZE);

	void* iso_buffer = malloc(block_size * DISC_RAW_SECTOR_SIZE);
	if (!iso_buffer) {
		perror("malloc failed");
		free(data_buffer);
		free(raw_buffer);
		close(fdsacd);
		terminate_module();
		return 1;
	}
	memset(iso_buffer, 0, block_size * DISC_RAW_SECTOR_SIZE);

	char rawfile[256] = {};
	snprintf(rawfile, sizeof(rawfile), "%s/%s.raw", curdir, toc[0].Disc_Catalog_Number);

	FILE* fpraw = fopen(rawfile, "wb");
	if (!fpraw) {
		perror("fpraw failed");
		free(data_buffer);
		free(raw_buffer);
		free(iso_buffer);
		close(fdsacd);
		terminate_module();
		return 1;
	}

	char isofile[256] = {};
	snprintf(isofile, sizeof(isofile), "%s/%s.iso", curdir, toc[0].Disc_Catalog_Number);

	FILE* fpiso = fopen(isofile, "wb");
	if (!fpiso) {
		perror("fpiso failed");
		fclose(fpraw);
		free(data_buffer);
		free(raw_buffer);
		free(iso_buffer);
		close(fdsacd);
		terminate_module();
		return 1;
	}

	char header[256] = {};
	snprintf(header, sizeof(header), "%s/%s_header.txt", curdir, toc[0].Disc_Catalog_Number);

	FILE* fpHeader = fopen(header, "w");
	if (!fpHeader) {
		perror("fpHeader failed");
		fclose(fpraw);
		fclose(fpiso);
		free(data_buffer);
		free(raw_buffer);
		free(iso_buffer);
		close(fdsacd);
		terminate_module();
		return 1;
	}
	unsigned int poly = 0x80000011; /* x^32 + x^31 + x^4 + 1 */
	unsigned int edcLut[UCHAR_MAX + 1] = {};
	for (unsigned int i = 0; i <= UCHAR_MAX; i++) {
		unsigned int edc = i;
		for (int k = 0; k < 32; k++) {
			if (edc & 0x80000000) {
				edc = (edc << 1) ^ poly;
			}
			else {
				edc <<= 1;
			}
		}
		edcLut[i] = edc;
	}
#ifdef DEBUG
	all_sector = 896;
#endif
	unsigned int current_lsn = 0x30000;
	for (unsigned int lbn = 0; lbn < all_sector; lbn += block_size) {
		block_size = MIN(all_sector - lbn, MAX_BLOCK_SIZE);

		ioctl_buf.param2   = 0;
		ioctl_buf.param1   = lbn;
		ioctl_buf.count    = block_size;
		ioctl_buf.user_ptr = (unsigned int)data_buffer;

		if (read_sacd(fdsacd, &ioctl_buf) < 0) {
			perror("read_sacd failed");
		}

		for (unsigned int i = 0; i < block_size; i++) {
			unsigned int sectorEdc = 0;
			unsigned int SectorNumber = 0;
			unsigned int ofs_raw = DISC_RAW_SECTOR_SIZE * i;
			unsigned int ofs_iso = DISC_SECTOR_SIZE * i;
			OutputSACDHeader(fpHeader, data_buffer + ofs_raw, &SectorNumber, &sectorEdc, lbn + i);

			unsigned int edc = 0;
			unsigned int uiEdcCalcSize = DISC_RAW_SECTOR_SIZE - 4;
			unsigned char* src = data_buffer + ofs_raw;
#if 1
			// unroll by 4 bytes
			unsigned int j = 0;
			for (; j + 4 <= uiEdcCalcSize; j += 4) {
				edc = (edc << 8) ^ edcLut[((edc >> 24) ^ src[j]) & 0xFF];
				edc = (edc << 8) ^ edcLut[((edc >> 24) ^ src[j + 1]) & 0xFF];
				edc = (edc << 8) ^ edcLut[((edc >> 24) ^ src[j + 2]) & 0xFF];
				edc = (edc << 8) ^ edcLut[((edc >> 24) ^ src[j + 3]) & 0xFF];
			}
			// remain
			for (; j < uiEdcCalcSize; ++j) {
			    edc = (edc << 8) ^ edcLut[((edc >> 24) ^ src[j]) & 0xFF];
			}
#else
			while (uiEdcCalcSize--) {
				edc = (edc << 8) ^ edcLut[((edc >> 24) ^ (*src++)) & 0xFF];
			}
#endif
			if (edc != sectorEdc) {
				fprintf(fpHeader
					, "LBA[%07d, 0x%06x]: EDC mismatch. CalcEDC: %08x, SectorEDC: %08x\n", lbn + i, (unsigned int)lbn + i, edc, sectorEdc);
			}
			else if (SectorNumber != current_lsn + i) {
				fprintf(fpHeader
					, "LBA[%07d, 0x%06x]: SectorNumber(%06x) mismatch. Expected: %08x\n", lbn + i, (unsigned int)lbn + i, SectorNumber, current_lsn + i);
			}
			else {
				memcpy(raw_buffer + ofs_raw, data_buffer + ofs_raw, DISC_RAW_SECTOR_SIZE);
				if (*(unsigned char*)(data_buffer + ofs_raw + 6) == 0x40) {
					ioctl_buf.param2   = 0;
					ioctl_buf.param1   = 0;
					ioctl_buf.count    = 1;
					ioctl_buf.user_ptr = (unsigned int)(data_buffer + ofs_raw);

					if (decrypt_sacd(fdsacd, &ioctl_buf) < 0) {
						perror("decrypt_sacd failed");
					}
					else {
						memcpy(iso_buffer + ofs_iso, data_buffer + ofs_raw, DISC_SECTOR_SIZE);
					}
				}
				else {
					memcpy(iso_buffer + ofs_iso, data_buffer + ofs_raw + 12, DISC_SECTOR_SIZE);
				}
			}
		}
		fwrite(raw_buffer, DISC_RAW_SECTOR_SIZE, block_size, fpraw);
		fwrite(iso_buffer, DISC_SECTOR_SIZE, block_size, fpiso);
		current_lsn += block_size;
	}
	fclose(fpraw);
	fclose(fpiso);
	fclose(fpHeader);
	free(data_buffer);
	free(raw_buffer);
	free(iso_buffer);

	ioctl_buf.param2   = 0;
	ioctl_buf.param1   = 0;
	ioctl_buf.count    = 0;
	ioctl_buf.user_ptr = 0;
	uninit_sacd(fdsacd, &ioctl_buf);
	close(fdsacd);
	terminate_module();

	return 0;
}
