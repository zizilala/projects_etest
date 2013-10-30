

#ifndef __BCRConfig_H__
#define __BCRConfig_H__


#define   DEFAULT_BCR_COM    L"COM1:"

#define   BCR_Default_Baudrate   9600

#define   BCR_2D_Baudrate 921600

#define   BCR_MAX_RECEIVE_BYTES (1280)

#define   BCR_SCAN_TIMEOUT 6000

#define   BCR_NOTIFY_WAVE_FILE  L"\\windows\\bcgood.wav"

#define	BCR_ENGINE_UNKNOW		0xff
#define	BCR_ENGINE_SSI_955		0x01
#define	BCR_ENGINE_SSI_4400		(0x01<<1)
#define	BCR_ENGINE_SSI_4500		(0x01<<2)
#define	BCR_ENGINE_ZEBEX_Z5111	(0x01<<3)

#define   BCR_SERVICES_REGKEY            TEXT("SOFTWARE\\Barocde")

#define   BCR_POWER_REGVALUE             TEXT("Power")

#define   BCR_OUTPUT_REGVALUE            TEXT("Ouput Mode")

#define   BCR_TERMINAL_CHAR_REGVALUE     TEXT("Terminal Char")

#define   BCR_PREFIX_REGVALUE            TEXT("Prefix")

#define	BCR_SUFFIX_REGVALUE				TEXT("Suffix")

#define	BCR_REPLACE_CHAR_REGVALUE		TEXT("Replace Char")

#define	BCR_REPLACE_CHAR_SRC_REGVALUE	TEXT("Replace Char Src")

#define	BCR_REPLACE_CHAR_DEST_REGVALUE	TEXT("Replace Char Dest")

#define	BCR_UTF8_CODEPAGE				TEXT("UTF8 Code Page")

#define   BCR_POWERPACK_REGKEY           TEXT("SOFTWARE\\PowerPack\\TrayScan")

#define   BCR_COM_REGVALUE               TEXT("Com Port")




#endif


