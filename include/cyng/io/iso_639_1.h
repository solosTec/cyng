/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_ISO_639_1_H
#define CYNG_IO_ISO_639_1_H

#include <cstdint>

 /** @file iso_639_1 contains language codes.
  */
namespace cyng
{
	namespace io
	{
		constexpr std::uint32_t calulate_lc(char a, char b) {
			return ((a - 65) * 100) + (b - 65);
		}

		enum language_codes : std::uint32_t {
			LC_AA = calulate_lc('A', 'A'),	//	Afar
			LC_AB = calulate_lc('A', 'B'),	//	Abkhazian
			LC_AE = calulate_lc('A', 'E'),	//	Avestan
			LC_AF = calulate_lc('A', 'F'),	//	Afrikaans
			LC_AK = calulate_lc('A', 'K'),	//	Akan
			LC_AM = calulate_lc('A', 'M'),	//	Amharic
			LC_AN = calulate_lc('A', 'N'),	//	Aragonese
			LC_AR = calulate_lc('A', 'R'),	//	Arabic
			LC_AS = calulate_lc('A', 'S'),	//	Assamese
			LC_AV = calulate_lc('A', 'V'),	//	Avaric
			LC_AY = calulate_lc('A', 'Y'),	//	Aymara
			LC_AZ = calulate_lc('A', 'Z'),	//	Azerbaijani
			LC_BA = calulate_lc('B', 'A'),	//	Bashkir
			LC_BE = calulate_lc('B', 'E'),	//	Belarusian
			LC_BG = calulate_lc('B', 'G'),	//	Bulgarian
			LC_BH = calulate_lc('B', 'H'),	//	Bihari languages
			LC_BI = calulate_lc('B', 'I'),	//	Bislama
			LC_BM = calulate_lc('B', 'M'),	//	Bambara
			LC_BN = calulate_lc('B', 'N'),	//	Bengali
			LC_BO = calulate_lc('B', 'O'),	//	Tibetan
			LC_BR = calulate_lc('B', 'R'), //	Breton
			LC_BS = calulate_lc('B', 'S'), // Bosnian
			LC_CA = calulate_lc('C', 'A'), // Catalan; Valencian
			LC_CE = calulate_lc('C', 'E'), // Chechen
			LC_CH = calulate_lc('C', 'H'), // Chamorro
			LC_CO = calulate_lc('C', 'O'), // Corsican
			LC_CR = calulate_lc('C', 'R'), // Cree
			LC_CS = calulate_lc('C', 'S'), // Czech
			LC_CU = calulate_lc('C', 'U'), // Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic
			LC_CV = calulate_lc('C', 'V'), // Chuvash
			LC_CY = calulate_lc('C', 'Y'), // Welsh
			LC_DA = calulate_lc('D', 'A'), // Danish
			LC_DE = calulate_lc('D', 'E'), // German
			LC_DV = calulate_lc('D', 'V'), // Divehi; Dhivehi; Maldivian
			LC_DZ = calulate_lc('D', 'Z'), // Dzongkha
			LC_EE = calulate_lc('E', 'E'), // Ewe
			LC_EL = calulate_lc('E', 'L'), // "Greek, Modern (1453-)"
			LC_EN = calulate_lc('E', 'N'), // English
			LC_EO = calulate_lc('E', 'O'), // Esperanto
			LC_ES = calulate_lc('E', 'S'), // Spanish; Castilian
			LC_ET = calulate_lc('E', 'T'), // Estonian
			LC_EU = calulate_lc('E', 'U'), // Basque
			LC_FA = calulate_lc('F', 'A'), // Persian
			LC_FF = calulate_lc('F', 'F'), // Fulah
			LC_FI = calulate_lc('F', 'I'), // Finnish
			LC_FJ = calulate_lc('F', 'J'), // Fijian
			LC_FO = calulate_lc('F', 'O'), // Faroese
			LC_FR = calulate_lc('F', 'R'), // French
			LC_FY = calulate_lc('F', 'Y'), // Western Frisian
			LC_GA = calulate_lc('G', 'A'), // Irish
			LC_GD = calulate_lc('G', 'D'), // Gaelic; Scottish Gaelic
			LC_GL = calulate_lc('G', 'L'), // Galician
			LC_GN = calulate_lc('G', 'N'), // Guarani
			LC_GU = calulate_lc('G', 'U'), // Gujarati
			LC_GV = calulate_lc('G', 'V'), // Manx
			LC_HA = calulate_lc('H', 'A'), // Hausa
			LC_HE = calulate_lc('H', 'E'), // Hebrew
			LC_HI = calulate_lc('H', 'I'), // Hindi
			LC_HO = calulate_lc('H', 'O'), // Hiri Motu
			LC_HR = calulate_lc('H', 'R'), // Croatian
			LC_HT = calulate_lc('H', 'T'), // Haitian; Haitian Creole
			LC_HU = calulate_lc('H', 'U'), // Hungarian
			LC_HY = calulate_lc('H', 'Y'), // Armenian
			LC_HZ = calulate_lc('H', 'Z'), // Herero
			LC_IA = calulate_lc('I', 'A'), // Interlingua(International Auxiliary Language Association)
			LC_ID = calulate_lc('I', 'D'), // Indonesian
			LC_IE = calulate_lc('I', 'E'), // Interlingue; Occidental
			LC_IG = calulate_lc('I', 'G'), // Igbo
			LC_II = calulate_lc('I', 'I'), // Sichuan Yi; Nuosu
			LC_IK = calulate_lc('I', 'K'), // Inupiaq
			LC_IO = calulate_lc('I', 'O'), // Ido
			LC_IS = calulate_lc('I', 'S'), // Icelandic
			LC_IT = calulate_lc('I', 'T'), // Italian
			LC_IU = calulate_lc('I', 'U'), // Inuktitut
			LC_JA = calulate_lc('J', 'A'), // Japanese
			LC_JV = calulate_lc('J', 'V'), // Javanese
			LC_KA = calulate_lc('K', 'A'), // Georgian
			LC_KG = calulate_lc('K', 'G'), // Kongo
			LC_KI = calulate_lc('K', 'I'), // Kikuyu; Gikuyu
			LC_KJ = calulate_lc('K', 'J'), // Kuanyama; Kwanyama
			LC_KK = calulate_lc('K', 'K'), // Kazakh
			LC_KL = calulate_lc('K', 'L'), // Kalaallisut; Greenlandic
			LC_KM = calulate_lc('K', 'M'), // Central Khmer
			LC_KN = calulate_lc('K', 'N'), // Kannada
			LC_KO = calulate_lc('K', 'O'), // Korean
			LC_KR = calulate_lc('K', 'R'), // Kanuri
			LC_KS = calulate_lc('K', 'S'), // Kashmiri
			LC_KU = calulate_lc('K', 'U'), // Kurdish
			LC_KV = calulate_lc('K', 'V'), // Komi
			LC_KW = calulate_lc('K', 'W'), // Cornish
			LC_KY = calulate_lc('K', 'Y'), // Kirghiz; Kyrgyz
			LC_LA = calulate_lc('L', 'A'), // Latin
			LC_LB = calulate_lc('L', 'B'), // Luxembourgish; Letzeburgesch
			LC_LG = calulate_lc('L', 'G'), // Ganda
			LC_LI = calulate_lc('L', 'I'), // Limburgan; Limburger; Limburgish
			LC_LN = calulate_lc('L', 'N'), // Lingala
			LC_LO = calulate_lc('L', 'O'), // Lao
			LC_LT = calulate_lc('L', 'T'), // Lithuanian
			LC_LU = calulate_lc('L', 'U'), // Luba - Katanga
			LC_LV = calulate_lc('L', 'V'), // Latvian
			LC_MG = calulate_lc('M', 'G'), // Malagasy
			LC_MH = calulate_lc('M', 'H'), // Marshallese
			LC_MI = calulate_lc('M', 'I'), // Maori
			LC_MK = calulate_lc('M', 'K'), // Macedonian
			LC_ML = calulate_lc('M', 'L'), // Malayalam
			LC_MN = calulate_lc('M', 'N'), // Mongolian
			LC_MR = calulate_lc('M', 'R'), // Marathi
			LC_MS = calulate_lc('M', 'S'), // Malay
			LC_MT = calulate_lc('M', 'T'), // Maltese
			LC_MY = calulate_lc('M', 'Y'), // Burmese
			LC_NA = calulate_lc('N', 'A'), // Nauru
			LC_NB = calulate_lc('N', 'B'), // "Bokmål, Norwegian; Norwegian Bokmål"
			LC_ND = calulate_lc('N', 'D'), // "Ndebele, North; North Ndebele"
			LC_NE = calulate_lc('N', 'E'), // Nepali
			LC_NG = calulate_lc('N', 'G'), // Ndonga
			LC_NL = calulate_lc('N', 'L'), // Dutch; Flemish
			LC_NN = calulate_lc('N', 'N'), // "Norwegian Nynorsk; Nynorsk, Norwegian"
			LC_NO = calulate_lc('N', 'O'), // Norwegian
			LC_NR = calulate_lc('N', 'R'), // "Ndebele, South; South Ndebele"
			LC_NV = calulate_lc('N', 'V'), // Navajo; Navaho
			LC_NY = calulate_lc('N', 'Y'), // Chichewa; Chewa; Nyanja
			LC_OC = calulate_lc('O', 'C'), // Occitan(post 1500); Provençal
			LC_OJ = calulate_lc('O', 'J'), // Ojibwa
			LC_OM = calulate_lc('O', 'M'), // Oromo
			LC_OR = calulate_lc('O', 'R'), // Oriya
			LC_OS = calulate_lc('O', 'S'), // Ossetian; Ossetic
			LC_PA = calulate_lc('P', 'A'), // Panjabi; Punjabi
			LC_PI = calulate_lc('P', 'I'), // Pali
			LC_PL = calulate_lc('P', 'L'), // Polish
			LC_PS = calulate_lc('P', 'S'), // Pushto; Pashto
			LC_PT = calulate_lc('P', 'T'), // Portuguese
			LC_QU = calulate_lc('Q', 'U'), // Quechua
			LC_RM = calulate_lc('R', 'M'), // Romansh
			LC_RN = calulate_lc('R', 'N'), // Rundi
			LC_RO = calulate_lc('R', 'O'), // Romanian; Moldavian; Moldovan
			LC_RU = calulate_lc('R', 'U'), // Russian
			LC_RW = calulate_lc('R', 'W'), // Kinyarwanda
			LC_SA = calulate_lc('S', 'A'), // Sanskrit
			LC_SC = calulate_lc('S', 'C'), // Sardinian
			LC_SD = calulate_lc('S', 'D'), // Sindhi
			LC_SE = calulate_lc('S', 'E'), // Northern Sami
			LC_SG = calulate_lc('S', 'G'), // Sango
			LC_SI = calulate_lc('S', 'I'), // Sinhala; Sinhalese
			LC_SK = calulate_lc('S', 'K'), // Slovak
			LC_SL = calulate_lc('S', 'L'), // Slovenian
			LC_SM = calulate_lc('S', 'M'), // Samoan
			LC_SN = calulate_lc('S', 'N'), // Shona
			LC_SO = calulate_lc('S', 'O'), // Somali
			LC_SQ = calulate_lc('S', 'Q'), // Albanian
			LC_SR = calulate_lc('S', 'R'), // Serbian
			LC_SS = calulate_lc('S', 'S'), // Swati
			LC_ST = calulate_lc('S', 'T'), // "Sotho, Southern"
			LC_SU = calulate_lc('S', 'U'), // Sundanese
			LC_SV = calulate_lc('S', 'V'), // Swedish
			LC_SW = calulate_lc('S', 'W'), // Swahili
			LC_TA = calulate_lc('T', 'A'), // Tamil
			LC_TE = calulate_lc('T', 'E'), // Telugu
			LC_TG = calulate_lc('T', 'G'), // Tajik
			LC_TH = calulate_lc('T', 'H'), // Thai
			LC_TI = calulate_lc('T', 'I'), // Tigrinya
			LC_TK = calulate_lc('T', 'K'), // Turkmen
			LC_TL = calulate_lc('T', 'L'), // Tagalog
			LC_TN = calulate_lc('T', 'N'), // Tswana
			LC_TO = calulate_lc('T', 'O'), // Tonga(Tonga Islands)
			LC_TR = calulate_lc('T', 'R'), // Turkish
			LC_TS = calulate_lc('T', 'S'), // Tsonga
			LC_TT = calulate_lc('T', 'T'), // Tatar
			LC_TW = calulate_lc('T', 'W'), // Twi
			LC_TY = calulate_lc('T', 'Y'), // Tahitian
			LC_UG = calulate_lc('U', 'G'), // Uighur; Uyghur
			LC_UK = calulate_lc('U', 'K'), // Ukrainian
			LC_UR = calulate_lc('U', 'R'), // Urdu
			LC_UZ = calulate_lc('U', 'Z'), // Uzbek
			LC_VE = calulate_lc('V', 'E'), // Venda
			LC_VI = calulate_lc('V', 'I'), // Vietnamese
			LC_VO = calulate_lc('V', 'O'), // Volapük
			LC_WA = calulate_lc('W', 'A'), // Walloon
			LC_WO = calulate_lc('W', 'O'), // Wolof
			LC_XH = calulate_lc('X', 'H'), // Xhosa
			LC_YI = calulate_lc('Y', 'I'), // Yiddish
			LC_YO = calulate_lc('Y', 'O'), // Yoruba
			LC_ZA = calulate_lc('Z', 'A'), // Zhuang; Chuang
			LC_ZH = calulate_lc('Z', 'H'), // Chinese
			LC_ZU = calulate_lc('Z', 'U'), // Zulu
		};

		language_codes get_language_code(char a, char b);
	}
}

#endif 
