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

typedef struct _Locale {
	char Language_Code[2];
	unsigned char Character_Set_Code;
	unsigned char Reserved;
} Locale;

typedef struct _TextChannel {
	unsigned char Number_of_Text_Channels;
	unsigned char Reserved6[7];
	Locale locale[8];
} Text_Channel;

typedef struct _Master_TOC {
	char Master_TOC_Signature[8];	// 0..15 M_TOC_0_Header
	unsigned char Major_Version;
	unsigned char Minor_Version;
	unsigned char Reserved1[6];
	unsigned short Album_Set_Size;			// 16..63 Album_Info
	unsigned short Album_Sequence_Number;
	unsigned char Reserved2[4];
	char Album_Catalog_Number[16];
	unsigned char Album_Genre[4][4];
	unsigned char Reserved3[8];
	unsigned int TWOCH_TOC_1_Address;		// 64..127 Disc_Info
	unsigned int TWOCH_TOC_2_Address;
	unsigned int MC_TOC_1_Address;
	unsigned int MC_TOC_2_Address;
	unsigned char Disc_Flags_Reserved : 7;
	unsigned char Disc_Flags_Hybrid : 1;
	unsigned char Reserved4[3];
	unsigned short TWOCH_TOC_Len;
	unsigned short MC_TOC_Len;
	unsigned char Disc_Catalog_Number[16];
	unsigned char Disc_Genre[4][4];
	unsigned short Disc_Date_Y;
	unsigned char Disc_Date_M;
	unsigned char Disc_Date_D;
	unsigned int Reserved5;
	Text_Channel Txt_Ch;
	unsigned char Disc_WebLink_Info[128];
} Master_TOC;

typedef struct _Master_Text {
	char Master_Text_Signature[8];
	unsigned char Reserved1[8];
	unsigned short Album_Title_Ptr;
	unsigned short Album_Artist_Ptr;
	unsigned short Album_Publisher_Ptr;
	unsigned short Album_Copyright_Ptr;
	unsigned short Album_Title_Phonetic_Ptr;
	unsigned short Album_Artist_Phonetic_Ptr;
	unsigned short Album_Publisher_Phonetic_Ptr;
	unsigned short Album_Copyright_Phonetic_Ptr;
	unsigned short Disc_Title_Ptr;
	unsigned short Disc_Artist_Ptr;
	unsigned short Disc_Publisher_Ptr;
	unsigned short Disc_Copyright_Ptr;
	unsigned short Disc_Title_Phonetic_Ptr;
	unsigned short Disc_Artist_Phonetic_Ptr;
	unsigned short Disc_Publisher_Phonetic_Ptr;
	unsigned short Disc_Copyright_Phonetic_Ptr;
	unsigned char Reserved2[16];
} Master_Text;

typedef struct _Area_TOC {
	char Area_TOC_Signature[8];	// 0..15 A_TOC_0_Header
	unsigned char Major_Version;
	unsigned char Minor_Version;
	unsigned short Area_TOC_Length;
	unsigned int Reserved1;
	unsigned int Max_Byte_Rate;			// 16..127 Area_Data 
	unsigned char FS_Code;
	unsigned char Frame_Format : 4;
	unsigned char Reserved2 : 4;
	unsigned char Reserved3[10];
	unsigned char Number_of_Channels;
	unsigned char Loudspeaker_Config : 5;
	unsigned char Extra_Setting : 3;
	unsigned char Max_Available_Channels;
	unsigned char Reserved4 : 3;
	unsigned char AMF1 : 1;
	unsigned char AMF2 : 1;
	unsigned char AMF3 : 1;
	unsigned char AMF4 : 1;
	unsigned char Reserved5 : 1;
	unsigned char Reserved6[12];
	unsigned char Track_Attribute : 4;
	unsigned char Reserved7 : 4;
	unsigned char Reserved8[15];
	unsigned char m;
	unsigned char s;
	unsigned char f;
	unsigned char Reserved9;
	unsigned char Track_Offset;
	unsigned char Number_of_Track;
	unsigned char First_Bonus_Track_Number;
	unsigned char Reserved10;
	unsigned int Track_Area_Start_Address;
	unsigned int Track_Area_End_Address;
	Text_Channel Txt_Ch;
	unsigned char Reserved11[8];
	unsigned short Track_Text_Ptr;		// 128..143 List_Pointers
	unsigned short Index_List_Ptr;
	unsigned short Access_List_Ptr;
	unsigned short Track_WebLink_List_Ptr;
	unsigned short Track_List_3_Ptr;
	unsigned short Set_Of_Playlists_Ptr;
	unsigned char Reserved12[4];
	unsigned char Area_Text[1904];
} Area_TOC;

typedef struct _Track_List_1 {
	char Track_List_1_Signature[8];
	unsigned int Track_Start_Address[255];
	unsigned int Track_Length[255];
} Track_List_1;

typedef struct _Track_Start_Time {
	unsigned char m;
	unsigned char s;
	unsigned char f;
	unsigned char Track_Mode;
} Track_Start_Time;

typedef struct _Track_Time_Length {
	unsigned char m;
	unsigned char s;
	unsigned char f;
	unsigned char Track_Flags;
} Track_Time_Length;

typedef struct _Track_List_2 {
	char Track_List_2_Signature[8];
	Track_Start_Time Start_Time[255];
	Track_Time_Length Time_Length[255];
} Track_List_2;

#pragma pack(push, acc, 1)
typedef struct _Main_Acc_List {
	unsigned short Access_Flags;
	unsigned char Entrys[3];
} Main_Acc_List;

typedef struct _Detailed_Access {
	unsigned char Sub_Entrys[9][3];
} Detailed_Access;

typedef struct _Access_List {
	char Access_List_Signature[8];
	unsigned short N_Entries;
	unsigned char Main_Step_Size;
	unsigned char Reserved[5];
	Main_Acc_List mAList[6550];
	unsigned char Reserved2[2];
	Detailed_Access dAccess[1213];
	unsigned char Reserved3[16];
} Access_List;
#pragma pack(pop, acc)

typedef struct _Track_Text {
	char Track_Text_Signature[8];
	unsigned short Track_Text_Item_Ptr[8][255];
} Track_Text;

void OutputMainChannel(
	FILE* fp,
	unsigned char* lpBuf,
	char* szLabel,
	int nLBA,
	unsigned long dwSize
) {
	if (szLabel != NULL) {
		fprintf(fp, "%s ", szLabel);
	}
	fprintf(fp, "LBA: %d (0x%x)\n"
		"       +0 +1 +2 +3 +4 +5 +6 +7  +8 +9 +A +B +C +D +E +F\n", nLBA, nLBA);

	for (unsigned long i = 0; i < dwSize; i += 16) {
		if (16 > dwSize - i) {
			fprintf(fp, "%04lX : ", i);
			for (unsigned long j = 0; j < dwSize - i; j++) {
				if (j == 8) {
					fprintf(fp, " ");
				}
				fprintf(fp, "%02X ", lpBuf[i + j]);
			}
		}
		else {
			fprintf(fp,
				"%04lX : %02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X   "
				, i, lpBuf[i], lpBuf[i + 1], lpBuf[i + 2], lpBuf[i + 3], lpBuf[i + 4], lpBuf[i + 5]
				, lpBuf[i + 6], lpBuf[i + 7], lpBuf[i + 8], lpBuf[i + 9], lpBuf[i + 10], lpBuf[i + 11]
				, lpBuf[i + 12], lpBuf[i + 13], lpBuf[i + 14], lpBuf[i + 15]);
			for (int j = 0; j < 16; j++) {
				int ch = isprint(lpBuf[i + j]) ? lpBuf[i + j] : '.';
				fprintf(fp, "%c", ch);
			}
		}
		fprintf(fp, "\n");
	}
}

void OutputSACDHeader(
	FILE* fpHeader,
	unsigned char* lpBuf,
	unsigned int* lpSectorNumber,
	unsigned int* lpEdc,
	int nLBA
) {
	fprintf(fpHeader,
		"LBA[%07d, 0x%06x]: SectorInfo[%02x]{Layer %d, %s, "
		, nLBA, (unsigned int)nLBA, lpBuf[0], lpBuf[0] & 0x01
		, (lpBuf[0] & 0x02) == 0 ? "read-only" : "other"
	);
	switch ((lpBuf[0] & 0x0c) >> 2) {
	case 0:
		fprintf(fpHeader, "Data Zone, ");
		break;
	case 1:
		fprintf(fpHeader, "Lead-in Zone, ");
		break;
	case 2:
		fprintf(fpHeader, "Lead-out Zone, ");
		break;
	case 3:
		fprintf(fpHeader, "Middle Zone, ");
		break;
	default:
		break;
	}
	*lpSectorNumber = MAKEUINT(lpBuf[3], lpBuf[2], lpBuf[1], 0);
	fprintf(fpHeader,
		"%s}, SectorNumber[%06x (%8u)], IED[%04x], CPSI[%02x%02x%02x%02x%02x%02x], "
		, (lpBuf[0] & 0x20) == 0 ? "greater than 40%" : "         40% max"
		, *lpSectorNumber, *lpSectorNumber, MAKEWORD(lpBuf[5], lpBuf[4])
		, lpBuf[6], lpBuf[7], lpBuf[8], lpBuf[9], lpBuf[10], lpBuf[11]
	);
	*lpEdc = MAKEUINT(lpBuf[2063], lpBuf[2062], lpBuf[2061], lpBuf[2060]);
	fprintf(fpHeader, "EDC[%08x]\n", *lpEdc);
}

int ReadSACDFileSystem(
	int fd,
	FILE* fpLog,
	FILE* fpSector,
	TOC* pToc
) {
	unsigned char* lpBuf = (unsigned char*)malloc(DISC_SECTOR_SIZE * 33);
	if (!lpBuf) {
		perror("malloc failed");
		return -1;
	}

	int nLBA = 510;
	off64_t fdofs = nLBA * DISC_SECTOR_SIZE;
	off_t result = lseek64(fd, fdofs, SEEK_SET);
	if (result == -1) {
		perror("lseek failed (Master TOC)");
		return -1;
	}

	ssize_t bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
	if (bytesRead == -1) {
		perror("Read failed (Master TOC)");
		return -1;
	}

	OutputMainChannel(fpSector, lpBuf, "Master TOC", nLBA, DISC_SECTOR_SIZE);

	Master_TOC mToc;
	memcpy(&mToc, lpBuf, sizeof(Master_TOC));
	mToc.Album_Set_Size = MAKEWORD(lpBuf[0x11], lpBuf[0x10]);
	mToc.Album_Sequence_Number = MAKEWORD(lpBuf[0x13], lpBuf[0x12]);
	mToc.TWOCH_TOC_1_Address = MAKEUINT(lpBuf[0x43], lpBuf[0x42], lpBuf[0x41], lpBuf[0x40]);
	mToc.TWOCH_TOC_2_Address = MAKEUINT(lpBuf[0x47], lpBuf[0x46], lpBuf[0x45], lpBuf[0x44]);
	mToc.MC_TOC_1_Address = MAKEUINT(lpBuf[0x4b], lpBuf[0x4a], lpBuf[0x49], lpBuf[0x48]);
	mToc.MC_TOC_2_Address = MAKEUINT(lpBuf[0x4f], lpBuf[0x4e], lpBuf[0x4d], lpBuf[0x4c]);
	mToc.TWOCH_TOC_Len = MAKEWORD(lpBuf[0x55], lpBuf[0x54]);
	mToc.MC_TOC_Len = MAKEWORD(lpBuf[0x57], lpBuf[0x56]);
	mToc.Disc_Date_Y = MAKEWORD(lpBuf[0x79], lpBuf[0x78]);

	fprintf(fpLog,
		"Master_TOC\n"
		"\t   Master_TOC_Signature: %.8s\n"
		"\t           Spec_Version: %d.%02d\n"
		"\t         Album_Set_Size: %u\n"
		"\t  Album_Sequence_Number: %u\n"
		"\t   Album_Catalog_Number: %.16s\n"
		"\t           Album_Genre1: %02x %02x %02x %02x\n"
		"\t           Album_Genre2: %02x %02x %02x %02x\n"
		"\t           Album_Genre3: %02x %02x %02x %02x\n"
		"\t           Album_Genre4: %02x %02x %02x %02x\n"
		"\t    TWOCH_TOC_1_Address: %u (%#08x)\n"
		"\t    TWOCH_TOC_2_Address: %u (%#08x)\n"
		"\t       MC_TOC_1_Address: %u (%#08x)\n"
		"\t       MC_TOC_2_Address: %u (%#08x)\n"
		"\t      Disc_Flags_Hybrid: %hhu\n"
		"\t          TWOCH_TOC_Len: %u (%#08x)\n"
		"\t             MC_TOC_Len: %u (%#08x)\n"
		"\t    Disc_Catalog_Number: %.16s\n"
		"\t            Disc_Genre1: %02x %02x %02x %02x\n"
		"\t            Disc_Genre2: %02x %02x %02x %02x\n"
		"\t            Disc_Genre3: %02x %02x %02x %02x\n"
		"\t            Disc_Genre4: %02x %02x %02x %02x\n"
		"\t              Disc_Date: %u-%02u-%02u\n"
		"\tNumber_of_Text_Channels: %u\n"
		, &mToc.Master_TOC_Signature[0]
		, mToc.Major_Version, mToc.Minor_Version
		, mToc.Album_Set_Size
		, mToc.Album_Sequence_Number
		, &mToc.Album_Catalog_Number[0]
		, mToc.Album_Genre[0][0], mToc.Album_Genre[0][1], mToc.Album_Genre[0][2], mToc.Album_Genre[0][3]
		, mToc.Album_Genre[1][0], mToc.Album_Genre[1][1], mToc.Album_Genre[1][2], mToc.Album_Genre[1][3]
		, mToc.Album_Genre[2][0], mToc.Album_Genre[2][1], mToc.Album_Genre[2][2], mToc.Album_Genre[2][3]
		, mToc.Album_Genre[3][0], mToc.Album_Genre[3][1], mToc.Album_Genre[3][2], mToc.Album_Genre[3][3]
		, mToc.TWOCH_TOC_1_Address, mToc.TWOCH_TOC_1_Address
		, mToc.TWOCH_TOC_2_Address, mToc.TWOCH_TOC_2_Address
		, mToc.MC_TOC_1_Address, mToc.MC_TOC_1_Address
		, mToc.MC_TOC_2_Address, mToc.MC_TOC_2_Address
		, mToc.Disc_Flags_Hybrid
		, mToc.TWOCH_TOC_Len, mToc.TWOCH_TOC_Len
		, mToc.MC_TOC_Len, mToc.MC_TOC_Len
		, &mToc.Disc_Catalog_Number[0]
		, mToc.Disc_Genre[0][0], mToc.Disc_Genre[0][1], mToc.Disc_Genre[0][2], mToc.Disc_Genre[0][3]
		, mToc.Disc_Genre[1][0], mToc.Disc_Genre[1][1], mToc.Disc_Genre[1][2], mToc.Disc_Genre[1][3]
		, mToc.Disc_Genre[2][0], mToc.Disc_Genre[2][1], mToc.Disc_Genre[2][2], mToc.Disc_Genre[2][3]
		, mToc.Disc_Genre[3][0], mToc.Disc_Genre[3][1], mToc.Disc_Genre[3][2], mToc.Disc_Genre[3][3]
		, mToc.Disc_Date_Y, mToc.Disc_Date_M, mToc.Disc_Date_D
		, mToc.Txt_Ch.Number_of_Text_Channels
	);
	memcpy(pToc[0].Disc_Catalog_Number, mToc.Disc_Catalog_Number, 16);
	for (int i = 0; pToc[0].Disc_Catalog_Number[i] != '\0'; i++) {
		if (pToc[0].Disc_Catalog_Number[i] == ' ') {
			pToc[0].Disc_Catalog_Number[i] = '\0';
			break;
		}
	}

	char Lang[][14] = {
		"UNKNOWN", "ISO646", "ISO8859_1", "RIS506"
		, "KSC5601", "GB2312", "BIG5", "ISO8859_1_ESC"
	};
	for (int i = 0; i < mToc.Txt_Ch.Number_of_Text_Channels; i++) {
		fprintf(fpLog,
			"\t     Language_Code[%d]: %.2s\n"
			"\tCharacter_Set_Code[%d]: %s\n"
			, i + 1, &mToc.Txt_Ch.locale[i].Language_Code[0]
			, i + 1, &Lang[mToc.Txt_Ch.locale[i].Character_Set_Code][0]
		);
	}

	for (nLBA = 511; nLBA <= 518; nLBA++) {
		bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
		if (bytesRead == -1) {
			perror("Read failed (Master_Text)");
			return -1;
		}
		OutputMainChannel(fpSector, lpBuf, "Master_Text", nLBA, DISC_SECTOR_SIZE);
		Master_Text mText;
		mText.Album_Title_Ptr = MAKEWORD(lpBuf[0x11], lpBuf[0x10]);
		mText.Album_Artist_Ptr = MAKEWORD(lpBuf[0x13], lpBuf[0x12]);
		mText.Album_Publisher_Ptr = MAKEWORD(lpBuf[0x15], lpBuf[0x14]);
		mText.Album_Copyright_Ptr = MAKEWORD(lpBuf[0x17], lpBuf[0x16]);
		mText.Album_Title_Phonetic_Ptr = MAKEWORD(lpBuf[0x19], lpBuf[0x18]);
		mText.Album_Artist_Phonetic_Ptr = MAKEWORD(lpBuf[0x1b], lpBuf[0x1a]);
		mText.Album_Publisher_Phonetic_Ptr = MAKEWORD(lpBuf[0x1d], lpBuf[0x1c]);
		mText.Album_Copyright_Phonetic_Ptr = MAKEWORD(lpBuf[0x1f], lpBuf[0x1e]);
		mText.Disc_Title_Ptr = MAKEWORD(lpBuf[0x21], lpBuf[0x20]);
		mText.Disc_Artist_Ptr = MAKEWORD(lpBuf[0x23], lpBuf[0x22]);
		mText.Disc_Publisher_Ptr = MAKEWORD(lpBuf[0x25], lpBuf[0x24]);
		mText.Disc_Copyright_Ptr = MAKEWORD(lpBuf[0x27], lpBuf[0x26]);
		mText.Disc_Title_Phonetic_Ptr = MAKEWORD(lpBuf[0x29], lpBuf[0x28]);
		mText.Disc_Artist_Phonetic_Ptr = MAKEWORD(lpBuf[0x2b], lpBuf[0x2a]);
		mText.Disc_Publisher_Phonetic_Ptr = MAKEWORD(lpBuf[0x2d], lpBuf[0x2c]);
		mText.Disc_Copyright_Phonetic_Ptr = MAKEWORD(lpBuf[0x2f], lpBuf[0x2e]);

		fprintf(fpLog,
			"Master_Text\n"
			"\t   Master_Text_Signature: %8s\n", &lpBuf[0]
		);
		fprintf(fpLog, "\t             Album_Title: ");
		if (mText.Album_Title_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Title_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Title_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t            Album_Artist: ");
		if (mText.Album_Artist_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Artist_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Artist_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t         Album_Publisher: ");
		if (mText.Album_Publisher_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Publisher_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Publisher_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t         Album_Copyright: ");
		if (mText.Album_Copyright_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Copyright_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Copyright_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t    Album_Title_Phonetic: ");
		if (mText.Album_Title_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Title_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Title_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t   Album_Artist_Phonetic: ");
		if (mText.Album_Artist_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Artist_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Artist_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\tAlbum_Publisher_Phonetic: ");
		if (mText.Album_Publisher_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Publisher_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Publisher_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\tAlbum_Copyright_Phonetic: ");
		if (mText.Album_Copyright_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Album_Copyright_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Album_Copyright_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t              Disc_Title: ");
		if (mText.Disc_Title_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Title_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Title_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t             Disc_Artist: ");
		if (mText.Disc_Artist_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Artist_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Artist_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t          Disc_Publisher: ");
		if (mText.Disc_Publisher_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Publisher_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Publisher_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t          Disc_Copyright: ");
		if (mText.Disc_Copyright_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Copyright_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Copyright_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t     Disc_Title_Phonetic: ");
		if (mText.Disc_Title_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Title_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Title_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t    Disc_Artist_Phonetic: ");
		if (mText.Disc_Artist_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Artist_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Artist_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t Disc_Publisher_Phonetic: ");
		if (mText.Disc_Publisher_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Publisher_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Publisher_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n\t Disc_Copyright_Phonetic: ");
		if (mText.Disc_Copyright_Phonetic_Ptr) {
			for (int i = 0; lpBuf[mText.Disc_Copyright_Phonetic_Ptr + i] != 0; i++) {
				fprintf(fpLog, "%c", lpBuf[mText.Disc_Copyright_Phonetic_Ptr + i]);
			}
		}
		fprintf(fpLog, "\n");
	}
	nLBA = 519;
	bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
	if (bytesRead == -1) {
		perror("Read failed (Manufacture)");
		return -1;
	}

	OutputMainChannel(fpSector, lpBuf, "Manufacture", nLBA, DISC_SECTOR_SIZE);
	fprintf(fpLog,
		"Manufacture\n"
		"\tManuf_Info_Signature: %.8s\n", &lpBuf[0]);

	unsigned long ulChToc[] = { (unsigned long)mToc.TWOCH_TOC_1_Address , (unsigned long)mToc.MC_TOC_1_Address, 0 };
	for (unsigned long c = 0; ulChToc[c] != 0; c++) {
		nLBA = ulChToc[c];
		fdofs = (off64_t)nLBA * (off64_t)DISC_SECTOR_SIZE;
		result = lseek64(fd, fdofs, SEEK_SET);
		if (result == -1) {
			perror("lseek failed (Area_TOC)");
			return -1;
		}
		bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
		if (bytesRead == -1) {
			perror("Read failed (Area_TOC)");
			return -1;
		}
		OutputMainChannel(fpSector, lpBuf, "Area_TOC", nLBA, DISC_SECTOR_SIZE);

		Area_TOC aToc;
		memcpy(&aToc, lpBuf, sizeof(Area_TOC));
		aToc.Max_Byte_Rate = MAKEUINT(lpBuf[0x13], lpBuf[0x12], lpBuf[0x11], lpBuf[0x10]);
		aToc.Area_TOC_Length = MAKEWORD(lpBuf[0xb], lpBuf[0xa]);
		aToc.Track_Area_Start_Address = MAKEUINT(lpBuf[0x4b], lpBuf[0x4a], lpBuf[0x49], lpBuf[0x48]);
		aToc.Track_Area_End_Address = MAKEUINT(lpBuf[0x4f], lpBuf[0x4e], lpBuf[0x4d], lpBuf[0x4c]);

		fprintf(fpLog,
			"Area_TOC\n"
			"\t      Area_TOC_Signature: %.8s\n"
			"\t            Spec_Version: %d.%02d\n"
			"\t         Area_TOC_Length: %u (%#02x)\n"
			"\t           Max_Byte_Rate: %u\n"
			"\t                 FS_Code: %u\n"
			"\t            Frame_Format: %d\n"
			"\t      Number_of_Channels: %d\n"
			"\t      Loudspeaker_Config: %d\n"
			"\t           Extra_Setting: %d\n"
			"\t  Max_Available_Channels: %d\n"
			"\t                    AMF1: %d\n"
			"\t                    AMF2: %d\n"
			"\t                    AMF3: %d\n"
			"\t                    AMF4: %d\n"
			"\t         Track_Attribute: %d\n"
			"\t               Total MSF: %02d:%02d:%02d\n"
			"\t            Track_Offset: %d\n"
			"\t         Number_of_Track: %d\n"
			"\tFirst_Bonus_Track_Number: %d\n"
			"\tTrack_Area_Start_Address: %u (%#02x)\n"
			"\t  Track_Area_End_Address: %u (%#02x)\n"
			"\t       Max_Text_Channels: %d\n"
			, &lpBuf[0]
			, aToc.Major_Version, aToc.Minor_Version
			, aToc.Area_TOC_Length, aToc.Area_TOC_Length
			, aToc.Max_Byte_Rate
			, aToc.FS_Code
			, aToc.Frame_Format
			, aToc.Number_of_Channels
			, aToc.Loudspeaker_Config
			, aToc.Extra_Setting
			, aToc.Max_Available_Channels
			, aToc.AMF1, aToc.AMF2, aToc.AMF3, aToc.AMF4
			, aToc.Track_Attribute
			, aToc.m, aToc.s, aToc.f
			, aToc.Track_Offset
			, aToc.Number_of_Track
			, aToc.First_Bonus_Track_Number
			, aToc.Track_Area_Start_Address, aToc.Track_Area_Start_Address
			, aToc.Track_Area_End_Address, aToc.Track_Area_End_Address
			, aToc.Txt_Ch.Number_of_Text_Channels
		);

		for (int i = 0; i < aToc.Txt_Ch.Number_of_Text_Channels; i++) {
			fprintf(fpLog,
				"\t      Language_Code[%d]: %.2s\n"
				"\t Character_Set_Code[%d]: %s\n"
				, i + 1, &aToc.Txt_Ch.locale[i].Language_Code[0]
				, i + 1, &Lang[aToc.Txt_Ch.locale[i].Character_Set_Code][0]
			);
		}
		aToc.Track_Text_Ptr = MAKEWORD(lpBuf[0x81], lpBuf[0x80]);
		aToc.Index_List_Ptr = MAKEWORD(lpBuf[0x83], lpBuf[0x82]);
		aToc.Access_List_Ptr = MAKEWORD(lpBuf[0x85], lpBuf[0x84]);
		aToc.Track_WebLink_List_Ptr = MAKEWORD(lpBuf[0x87], lpBuf[0x86]);
		fprintf(fpLog,
			"\t        Track_Text_Ptr: %u\n"
			"\t        Index_List_Ptr: %u\n"
			"\t       Access_List_Ptr: %u\n"
			"\tTrack_WebLink_List_Ptr: %u\n"
			"\t      Track_List_3_Ptr: %u\n"
			"\t  Set_Of_Playlists_Ptr: %u\n"
			, aToc.Track_Text_Ptr
			, aToc.Index_List_Ptr
			, aToc.Access_List_Ptr
			, aToc.Track_WebLink_List_Ptr
			, aToc.Track_List_3_Ptr
			, aToc.Set_Of_Playlists_Ptr
		);

		for (int i = 0; i < aToc.Txt_Ch.Number_of_Text_Channels; i++) {
			unsigned short AD_Ptr = MAKEWORD(aToc.Area_Text[1 + i * 8], aToc.Area_Text[0 + i * 8]);
			unsigned short AC_Ptr = MAKEWORD(aToc.Area_Text[3 + i * 8], aToc.Area_Text[2 + i * 8]);
			unsigned short ADP_Ptr = MAKEWORD(aToc.Area_Text[5 + i * 8], aToc.Area_Text[4 + i * 8]);
			unsigned short ACP_Ptr = MAKEWORD(aToc.Area_Text[7 + i * 8], aToc.Area_Text[6 + i * 8]);
			fprintf(fpLog,
				"\t         Area_Description_Ptr[%d]: %u\n"
				"\t           Area_Copyright_Ptr[%d]: %u\n"
				"\tArea_Description_Phonetic_Ptr[%d]: %u\n"
				"\t  Area_Copyright_Phonetic_Ptr[%d]: %u\n"
				, i + 1, AD_Ptr
				, i + 1, AC_Ptr
				, i + 1, ADP_Ptr
				, i + 1, ACP_Ptr
			);
			if (AD_Ptr) {
				fprintf(fpLog, "\t         Area_Description: %s\n", &aToc.Area_Text[AD_Ptr]);
			}
			if (AC_Ptr) {
				fprintf(fpLog, "\t          Area_Copyrightn: %s\n", &aToc.Area_Text[AC_Ptr]);
			}
			if (ADP_Ptr) {
				fprintf(fpLog, "\tArea_Description_Phonetic: %s\n", &aToc.Area_Text[ADP_Ptr]);
			}
			if (ACP_Ptr) {
				fprintf(fpLog, "\t  Area_Copyright_Phonetic: %s\n", &aToc.Area_Text[ACP_Ptr]);
			}
		}

		nLBA = ulChToc[c] + 1;
		bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
		if (bytesRead == -1) {
			perror("Read failed (Track_List_1)");
			return -1;
		}
		OutputMainChannel(fpSector, lpBuf, "Track_List_1", nLBA, DISC_SECTOR_SIZE);

		Track_List_1 tList1;

		fprintf(fpLog,
			"Track_List_1\n"
			"\t Track_List_1_Signature: %.8s\n"
			, &lpBuf[0]
		);
		for (int i = 0; i < aToc.Number_of_Track; i++) {
			tList1.Track_Start_Address[i] = MAKEUINT(
				lpBuf[0xb + sizeof(unsigned int) * i], lpBuf[0xa + sizeof(unsigned int) * i]
				, lpBuf[0x9 + sizeof(unsigned int) * i], lpBuf[0x8 + sizeof(unsigned int) * i]);
			fprintf(fpLog,
				"\tTrack_Start_Address[%02d]: %9u (%#x)\n"
				, i + 1, tList1.Track_Start_Address[i], tList1.Track_Start_Address[i]
			);
		}
		for (int i = 0; i < aToc.Number_of_Track; i++) {
			tList1.Track_Length[i] = MAKEUINT(
				lpBuf[0x407 + sizeof(unsigned int) * i], lpBuf[0x406 + sizeof(unsigned int) * i]
				, lpBuf[0x405 + sizeof(unsigned int) * i], lpBuf[0x404 + sizeof(unsigned int) * i]);
			fprintf(fpLog,
				"\t       Track_Length[%02d]: %9u (%#x)\n"
				, i + 1, tList1.Track_Length[i], tList1.Track_Length[i]
			);
		}
		nLBA = ulChToc[c] + 2;
		bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
		if (bytesRead == -1) {
			perror("Read failed (Track_List_2)");
			return -1;
		}
		OutputMainChannel(fpSector, lpBuf, "Track_List_2", nLBA, DISC_SECTOR_SIZE);

		Track_List_2 tList2;
		memcpy(&tList2, lpBuf, DISC_SECTOR_SIZE);

		fprintf(fpLog,
			"Track_List_2\n"
			"\t   Track_List_2_Signature: %.8s\n"
			, &lpBuf[0]
		);
		for (int i = 0; i < aToc.Number_of_Track; i++) {
			fprintf(fpLog,
				"\tTrack_Start_Time_Code[%02d]: %02d:%02d:%02d, Track_Mode[%02d]: %02d\n"
				, i + 1, tList2.Start_Time[i].m, tList2.Start_Time[i].s, tList2.Start_Time[i].f
				, i + 1, tList2.Start_Time[i].Track_Mode
			);
		}
		for (int i = 0; i < aToc.Number_of_Track; i++) {
			fprintf(fpLog,
				"\t    Track_Time_Length[%02d]: %02d:%02d:%02d, Track_Flags[%02d]: %02d\n"
				, i + 1, tList2.Time_Length[i].m, tList2.Time_Length[i].s, tList2.Time_Length[i].f
				, i + 1, tList2.Time_Length[i].Track_Flags
			);
		}

		nLBA = ulChToc[c] + 3;
		bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
		if (bytesRead == -1) {
			perror("Read failed (ISRC_and_Genre_List)");
			return -1;
		}
		OutputMainChannel(fpSector, lpBuf, "ISRC_and_Genre_List", nLBA, DISC_SECTOR_SIZE * 2);

		fprintf(fpLog,
			"ISRC_and_Genre_List\n"
			"\tISRC_and_Genre_List_Signature: %.8s\n"
			, &lpBuf[0]
		);
		for (int i = 0; i < aToc.Number_of_Track; i++) {
			fprintf(fpLog,
				"\t           ISRC_and_Genre[%02d]: %.12s\n"
				, i + 1, &lpBuf[0x8 + 12 * i]
			);
		}

		if (aToc.Access_List_Ptr) {
			nLBA = ulChToc[c] + aToc.Access_List_Ptr;
			fdofs = (off64_t)nLBA * (off64_t)DISC_SECTOR_SIZE;
			result = lseek64(fd, fdofs, SEEK_SET);
			if (result == -1) {
				perror("lseek failed (Access_List)");
				return -1;
			}
			bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE * 32);
			if (bytesRead == -1) {
				perror("Read failed (Access_List)");
				return -1;
			}
			OutputMainChannel(fpSector, lpBuf, "Access_List", nLBA, DISC_SECTOR_SIZE * 32);

			Access_List alist;
			memcpy(&alist, lpBuf, sizeof(Access_List));
			alist.N_Entries = MAKEWORD(lpBuf[0x9], lpBuf[0x8]);

			fprintf(fpLog,
				"Access_List\n"
				"\tAccess_List_Signature: %.8s\n"
				"\t            N_Entries: %d\n"
				"\t       Main_Step_Size: %d\n"
				, &lpBuf[0]
				, alist.N_Entries
				, alist.Main_Step_Size
			);
			for (int i = 0; i < alist.N_Entries; i++) {
				int detailed = alist.mAList[i].Access_Flags & 0x0001;
				fprintf(fpLog, "\tDetailed: %d, ", detailed);
				if (detailed) {
					fprintf(fpLog,
						"Detailed_List_Ptr: %5d, "
						, alist.mAList[i].Access_Flags >> 1 & 0x7fff
					);
				}
				else {
					fprintf(fpLog,
						"    Access_Margin: %5d, "
						, alist.mAList[i].Access_Flags >> 1 & 0x7fff
					);
				}
				fprintf(fpLog,
					"Entrys[%04d]: %02x %02x %02x\n"
					, i + 1, alist.mAList[i].Entrys[0], alist.mAList[i].Entrys[1], alist.mAList[i].Entrys[2]
				);
			}
			for (int i = 0, m = 0; i < alist.N_Entries; i++) {
				int detailed = alist.mAList[i].Access_Flags & 0x0001;
				int Access_Margin = alist.mAList[i].Access_Flags >> 1 & 0x7fff;
				if (detailed || (!detailed && Access_Margin != 0 && Access_Margin != 128 && Access_Margin != 256)) {
					for (int j = 0; j < 9; j++) {
						fprintf(fpLog,
							"\tEntrys[%04d] Sub_Entrys[%04d][%02d]: %02x %02x %02x\n"
							, i + 1, m + 1, j + 1, alist.dAccess[m].Sub_Entrys[j][0]
							, alist.dAccess[m].Sub_Entrys[j][1], alist.dAccess[m].Sub_Entrys[j][2]
						);
					}
					m++;
				}
			}
		}

		if (aToc.Track_Text_Ptr) {
			nLBA = ulChToc[c] + aToc.Track_Text_Ptr;
			fdofs = (off64_t)nLBA * (off64_t)DISC_SECTOR_SIZE;
			result = lseek64(fd, fdofs, SEEK_SET);
			if (result == -1) {
				perror("lseek failed (Track_Text)");
				return -1;
			}
			bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
			if (bytesRead == -1) {
				perror("Read failed (Track_Text)");
				return -1;
			}
			OutputMainChannel(fpSector, lpBuf, "Track_Text", nLBA, DISC_SECTOR_SIZE);

			Track_Text TTxt;
			fprintf(fpLog,
				"Track_Text\n"
				"\t      Track_Text_Signature: %.8s\n"
				, &lpBuf[0]
			);
			for (int h = 0; h < aToc.Txt_Ch.Number_of_Text_Channels; h++) {
				for (int i = 0; i < aToc.Number_of_Track; i++) {
					TTxt.Track_Text_Item_Ptr[h][i] = MAKEWORD(lpBuf[0x9 + sizeof(unsigned short) * i], lpBuf[0x8 + sizeof(unsigned short) * i]);
					fprintf(fpLog,
						"\tTrack_Text_Item_Ptr[%d][%02d]: %d (%#x)\n"
						, h + 1, i + 1, TTxt.Track_Text_Item_Ptr[h][i], TTxt.Track_Text_Item_Ptr[h][i]
					);
				}
			}

			int nLBAbak = 0;
			for (int h = 0; h < aToc.Txt_Ch.Number_of_Text_Channels; h++) {
				int nOfs = 0;
				for (int i = 0; i < aToc.Number_of_Track; i++) {
					if (TTxt.Track_Text_Item_Ptr[h][i]) {
						nLBA = ulChToc[c] + aToc.Track_Text_Ptr + (TTxt.Track_Text_Item_Ptr[h][i] / DISC_SECTOR_SIZE);
						if (nLBAbak < nLBA) {
							fdofs = (off64_t)nLBA * (off64_t)DISC_SECTOR_SIZE;
							result = lseek64(fd, fdofs, SEEK_SET);
							if (result == -1) {
								perror("lseek failed (Title)");
								return 1;
							}
							bytesRead = read(fd, lpBuf, DISC_SECTOR_SIZE);
							if (bytesRead == -1) {
								perror("Read failed (Title)");
								return 1;
							}
							OutputMainChannel(fpSector, lpBuf, "Title", nLBA, DISC_SECTOR_SIZE);
						}
						fprintf(fpLog, "\t     Number_of_Item[%d][%02d]: %02d, "
							, h + 1, i + 1, lpBuf[nOfs]);
						switch (lpBuf[nOfs + 4]) {
						case 1:
							fprintf(fpLog, "Title[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
						fflush(fpLog);
							break;
						case 2:
							fprintf(fpLog, "Performer[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
						fflush(fpLog);
							break;
						case 3:
							fprintf(fpLog, "Songwriter[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
						fflush(fpLog);
							break;
						case 4:
							fprintf(fpLog, "Composer[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
						fflush(fpLog);
							break;
						case 5:
							fprintf(fpLog, "Arranger[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
						fflush(fpLog);
							break;
						case 6:
							fprintf(fpLog, "Message[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 7:
							fprintf(fpLog, "Extra Message[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 8:
							fprintf(fpLog, "Copyright[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 129:
							fprintf(fpLog, "Title, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 130:
							fprintf(fpLog, "Performer, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 131:
							fprintf(fpLog, "Songwriter, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 132:
							fprintf(fpLog, "Composer, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 133:
							fprintf(fpLog, "Arranger, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 134:
							fprintf(fpLog, "Message, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 135:
							fprintf(fpLog, "Extra Message, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						case 136:
							fprintf(fpLog, "Copyright, phonetic text[%d][%02d]: %s\n"
								, h + 1, i + 1, &lpBuf[nOfs + 6]);
							break;
						default:
							fprintf(fpLog, "Reserved\n");
							break;
						}
						nOfs = TTxt.Track_Text_Item_Ptr[h][i + 1] - TTxt.Track_Text_Item_Ptr[h][0];
						nLBAbak = nLBA;
					}
				}
			}
		}
	}
	free(lpBuf);
	return 0;
}
