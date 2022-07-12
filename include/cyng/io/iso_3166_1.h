/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_ISO_3166_1_H
#define CYNG_IO_ISO_3166_1_H

#include <cstdint>

namespace cyng 
{	
	namespace io
	{
		constexpr std::uint32_t calulate_cc(char a, char b) {
			return ((a - 65) * 100) + (b - 65);
		}

		enum country_codes : std::uint32_t {
			CC_AF = calulate_cc('A', 'F'),	//	Afghanistan
			CC_AX = calulate_cc('A', 'X'),	//	Aland Islands
			CC_AL = calulate_cc('A', 'L'),	//	Albania
			CC_DZ = calulate_cc('D', 'Z'),	//	Algeria
			CC_AS = calulate_cc('A', 'S'),	//	American Samoa
			CC_AD = calulate_cc('A', 'D'),	// Andorra  
			CC_AO = calulate_cc('A', 'O'),	// Angola  
			CC_AI = calulate_cc('A', 'I'),	// Anguilla  
			CC_AQ = calulate_cc('A', 'Q'),	// Antarctica  
			CC_AG = calulate_cc('A', 'B'),	// Antigua and Barbuda  
			CC_AR = calulate_cc('A', 'R'),	// Argentina  
			CC_AM = calulate_cc('A', 'M'),	// Armenia  
			CC_AW = calulate_cc('A', 'W'),	// Aruba  
			CC_AU = calulate_cc('A', 'U'),	// Australia  
			CC_AT = calulate_cc('A', 'T'),	// Austria  
			CC_AZ = calulate_cc('A', 'Z'),	// Azerbaijan  
			CC_BS = calulate_cc('B', 'S'),	// Bahamas  
			CC_BH = calulate_cc('B', 'H'),	// Bahrain  
			CC_BD = calulate_cc('B', 'D'),	// Bangladesh  
			CC_BB = calulate_cc('B', 'B'),	// Barbados  
			CC_BY = calulate_cc('B', 'Y'),	// Belarus  
			CC_BE = calulate_cc('B', 'E'),	// Belgium  
			CC_BZ = calulate_cc('B', 'Z'),	// Belize  
			CC_BJ = calulate_cc('B', 'J'),	// Benin  
			CC_BM = calulate_cc('B', 'M'),	// Bermuda  
			CC_BT = calulate_cc('B', 'T'),	// Bhutan  
			CC_BO = calulate_cc('B', 'O'),	// Bolivia, Plurinational State of  
			CC_BQ = calulate_cc('B', 'Q'),	// Bonaire, Sint Eustatius and Saba  
			CC_BA = calulate_cc('B', 'A'),	// Bosnia and Herzegovina  
			CC_BW = calulate_cc('B', 'W'),	// Botswana  
			CC_BV = calulate_cc('B', 'V'),	// Bouvet Island  
			CC_BR = calulate_cc('B', 'R'),	// Brazil  
			CC_IO = calulate_cc('I', 'O'),	// British Indian Ocean Territory  
			CC_BN = calulate_cc('B', 'N'),	// Brunei Darussalam  
			CC_BG = calulate_cc('B', 'G'),	// Bulgaria  
			CC_BF = calulate_cc('B', 'F'),	// Burkina Faso  
			CC_BI = calulate_cc('B', 'I'),	// Burundi  
			CC_KH = calulate_cc('K', 'H'),	// Cambodia  
			CC_CM = calulate_cc('C', 'M'),	// Cameroon  
			CC_CA = calulate_cc('C', 'A'),	// Canada  
			CC_CV = calulate_cc('C', 'V'),	// Cape Verde  
			CC_KY = calulate_cc('K', 'Y'),	// Cayman Islands  
			CC_CF = calulate_cc('C', 'F'),	// Central African Republic  
			CC_TD,	// Chad  
			CC_CL = calulate_cc('C', 'L'),	// Chile  
			CC_CN = calulate_cc('C', 'N'),	// China  
			CC_CX = calulate_cc('C', 'X'),	// Christmas Island  
			CC_CC = calulate_cc('C', 'C'),	// Cocos (Keeling) Islands  
			CC_CO = calulate_cc('C', 'O'),	// Colombia  
			CC_KM,	// Comoros  
			CC_CG = calulate_cc('C', 'G'),	// Congo  
			CC_CD = calulate_cc('C', 'D'),	// Congo, the Democratic Republic of the  
			CC_CK = calulate_cc('C', 'K'),	// Cook Islands  
			CC_CR = calulate_cc('C', 'R'),	// Costa Rica  
			CC_CI = calulate_cc('C', 'I'),	// C\u00f4te d'Ivoire  
			CC_HR,	// Croatia  
			CC_CU = calulate_cc('C', 'U'),	// Cuba  
			CC_CW = calulate_cc('C', 'W'),	// Cura\u00e7ao  
			CC_CY = calulate_cc('C', 'Y'),	// Cyprus  
			CC_CZ = calulate_cc('C', 'Z'),	// Czech Republic  
			CC_DK = calulate_cc('D', 'K'),	// Denmark  
			CC_DJ = calulate_cc('D', 'J'),	// Djibouti  
			CC_DM = calulate_cc('D', 'M'),	// Dominica  
			CC_DO = calulate_cc('D', 'O'),	// Dominican Republic  
			CC_EC = calulate_cc('E', 'C'),	// Ecuador  
			CC_EG = calulate_cc('E', 'G'),	// Egypt  
			CC_SV,	// El Salvador  
			CC_GQ,	// Equatorial Guinea  
			CC_ER = calulate_cc('E', 'R'),	// Eritrea  
			CC_EE = calulate_cc('E', 'E'),	// Estonia  
			CC_ET = calulate_cc('E', 'T'),	// Ethiopia  
			CC_FK = calulate_cc('F', 'K'),	// Falkland Islands (Malvinas)  
			CC_FO = calulate_cc('F', 'O'),	// Faroe Islands  
			CC_FJ = calulate_cc('F', 'J'),	// Fiji  
			CC_FI = calulate_cc('F', 'I'),	// Finland  
			CC_FR = calulate_cc('F', 'R'),	// France  
			CC_GF = calulate_cc('G', 'F'),	// French Guiana  
			CC_PF,	// French Polynesia  
			CC_TF,	// French Southern Territories  
			CC_GA = calulate_cc('G', 'A'),	// Gabon  
			CC_GM = calulate_cc('G', 'M'),	// Gambia  
			CC_GE = calulate_cc('G', 'E'),	// Georgia  
			CC_DE = calulate_cc('D', 'E'),	// Germany  
			CC_GH = calulate_cc('G', 'H'),	// Ghana  
			CC_GI = calulate_cc('G', 'I'),	// Gibraltar  
			CC_GR = calulate_cc('G', 'R'),	// Greece  
			CC_GL = calulate_cc('G', 'L'),	// Greenland  
			CC_GD = calulate_cc('G', 'D'),	// Grenada  
			CC_GP = calulate_cc('G', 'P'),	// Guadeloupe  
			CC_GU = calulate_cc('G', 'U'),	// Guam  
			CC_GT = calulate_cc('G', 'T'),	// Guatemala  
			CC_GG = calulate_cc('G', 'G'),	// Guernsey  
			CC_GN = calulate_cc('G', 'N'),	// Guinea  
			CC_GW = calulate_cc('G', 'W'),	// Guinea-Bissau  
			CC_GY = calulate_cc('G', 'Y'),	// Guyana  
			CC_HT = calulate_cc('H', 'T'),	// Haiti  
			CC_HM,	// Heard Island and McDonald Islands  
			CC_VA,	// Holy See (Vatican City State)  
			CC_HN = calulate_cc('H', 'N'),	// Honduras  
			CC_HK = calulate_cc('H', 'K'),	// Hong Kong  
			CC_HU = calulate_cc('H', 'U'),	// Hungary  
			CC_IS,	// Iceland  
			CC_IN,	// India  
			CC_ID,	// Indonesia  
			CC_IR,	// Iran, Islamic Republic of  
			CC_IQ,	// Iraq  
			CC_IE = calulate_cc('I', 'E'),	// Ireland  
			CC_IM,	// Isle of Man  
			CC_IL = calulate_cc('I', 'L'),	// Israel  
			CC_IT,	// Italy  
			CC_JM,	// Jamaica  
			CC_JP = calulate_cc('J', 'P'),	// Japan  
			CC_JE,	// Jersey  
			CC_JO,	// Jordan  
			CC_KZ,	// Kazakhstan  
			CC_KE,	// Kenya  
			CC_KI,	// Kiribati  
			CC_KP,	// Korea, Democratic People's Republic of  
			CC_KR,	// Korea, Republic of  
			CC_KW,	// Kuwait  
			CC_KG,	// Kyrgyzstan  
			CC_LA,	// Lao People's Democratic Republic  
			CC_LV,	// Latvia  
			CC_LB,	// Lebanon  
			CC_LS,	// Lesotho  
			CC_LR,	// Liberia  
			CC_LY,	// Libya  
			CC_LI,	// Liechtenstein  
			CC_LT,	// Lithuania  
			CC_LU,	// Luxembourg  
			CC_MO = calulate_cc('M', 'O'),	// Macao  
			CC_MK = calulate_cc('M', 'K'),	// Macedonia, the Former Yugoslav Republic of  
			CC_MG = calulate_cc('M', 'G'),	// Madagascar  
			CC_MW = calulate_cc('M', 'W'),	// Malawi  
			CC_MY = calulate_cc('M', 'Y'),	// Malaysia  
			CC_MV = calulate_cc('M', 'V'),	// Maldives  
			CC_ML = calulate_cc('M', 'L'),	// Mali  
			CC_MT = calulate_cc('M', 'T'),	// Malta  
			CC_MH = calulate_cc('M', 'H'),	// Marshall Islands  
			CC_MQ = calulate_cc('M', 'Q'),	// Martinique  
			CC_MR = calulate_cc('M', 'R'),	// Mauritania  
			CC_MU = calulate_cc('M', 'U'),	// Mauritius  
			CC_YT,	// Mayotte  
			CC_MX = calulate_cc('M', 'X'),	// Mexico  
			CC_FM,	// Micronesia, Federated States of  
			CC_MD = calulate_cc('M', 'D'),	// Moldova, Republic of  
			CC_MC = calulate_cc('M', 'C'),	// Monaco  
			CC_MN = calulate_cc('M', 'N'),	// Mongolia  
			CC_ME = calulate_cc('M', 'E'),	// Montenegro  
			CC_MS = calulate_cc('M', 'S'),	// Montserrat  
			CC_MA = calulate_cc('M', 'A'),	// Morocco  
			CC_MZ = calulate_cc('M', 'Z'),	// Mozambique  
			CC_MM = calulate_cc('M', 'M'),	// Myanmar  
			CC_NA = calulate_cc('N', 'A'),	// Namibia  
			CC_NR,	// Nauru  
			CC_NP,	// Nepal  
			CC_NL = calulate_cc('N', 'L'),	// Netherlands  
			CC_NC,	// New Caledonia  
			CC_NZ,	// New Zealand  
			CC_NI,	// Nicaragua  
			CC_NE,	// Niger  
			CC_NG,	// Nigeria  
			CC_NU,	// Niue  
			CC_NF,	// Norfolk Island  
			CC_MP,	// Northern Mariana Islands  
			CC_NO,	// Norway  
			CC_OM,	// Oman  
			CC_PK,	// Pakistan  
			CC_PW,	// Palau  
			CC_PS,	// Palestine, State of  
			CC_PA,	// Panama  
			CC_PG,	// Papua New Guinea  
			CC_PY,	// Paraguay  
			CC_PE,	// Peru  
			CC_PH,	// Philippines  
			CC_PN,	// Pitcairn  
			CC_PL,	// Poland  
			CC_PT,	// Portugal  
			CC_PR,	// Puerto Rico  
			CC_QA,	// Qatar  
			CC_RE = calulate_cc('R', 'E'),	// Reunion  
			CC_RO = calulate_cc('R', 'O'),	// Romania  
			CC_RU = calulate_cc('R', 'U'),	// Russian Federation  
			CC_RW = calulate_cc('R', 'W'),	// Rwanda  
			CC_BL,	// Saint Barth\u00e9lemy  
			CC_SH = calulate_cc('S', 'H'),	// Saint Helena, Ascension and Tristan da Cunha  
			CC_KN,	// Saint Kitts and Nevis  
			CC_LC,	// Saint Lucia  
			CC_MF,	// Saint Martin (French part)  
			CC_PM,	// Saint Pierre and Miquelon  
			CC_VC,	// Saint Vincent and the Grenadines  
			CC_WS,	// Samoa  
			CC_SM = calulate_cc('S', 'M'),	// San Marino  
			CC_ST = calulate_cc('S', 'T'),	// Sao Tome and Principe  
			CC_SA = calulate_cc('S', 'A'),	// Saudi Arabia  
			CC_SN = calulate_cc('S', 'N'),	// Senegal  
			CC_RS = calulate_cc('R', 'S'),	// Serbia  
			CC_SC = calulate_cc('S', 'C'),	// Seychelles  
			CC_SL = calulate_cc('S', 'L'),	// Sierra Leone  
			CC_SG = calulate_cc('S', 'G'),	// Singapore  
			CC_SX = calulate_cc('S', 'X'),	// Sint Maarten (Dutch part)  
			CC_SK = calulate_cc('S', 'K'),	// Slovakia  
			CC_SI = calulate_cc('S', 'I'),	// Slovenia  
			CC_SB = calulate_cc('S', 'B'),	// Solomon Islands  
			CC_SO = calulate_cc('S', 'O'),	// Somalia  
			CC_ZA = calulate_cc('Z', 'A'),	// South Africa  
			CC_GS = calulate_cc('G', 'S'),	// South Georgia and the South Sandwich Islands  
			CC_SS = calulate_cc('S', 'S'),	// South Sudan  
			CC_ES = calulate_cc('E', 'S'),	// Spain  
			CC_LK = calulate_cc('L', 'K'),	// Sri Lanka  
			CC_SD = calulate_cc('S', 'D'),	// Sudan  
			CC_SR = calulate_cc('S', 'R'),	// Suriname  
			CC_SJ = calulate_cc('S', 'J'),	// Svalbard and Jan Mayen  
			CC_SZ = calulate_cc('S', 'Z'),	// Swaziland  
			CC_SE = calulate_cc('S', 'E'),	// Sweden  
			CC_CH = calulate_cc('C', 'H'),	// Switzerland  
			CC_SY = calulate_cc('S', 'Y'),	// Syrian Arab Republic  
			CC_TW = calulate_cc('T', 'W'),	// Taiwan
			CC_TJ = calulate_cc('T', 'J'),	// Tajikistan  
			CC_TZ = calulate_cc('T', 'Z'),	// Tanzania, United Republic of  
			CC_TH = calulate_cc('T', 'H'),	// Thailand  
			CC_TL = calulate_cc('T', 'L'),	// Timor-Leste  
			CC_TG = calulate_cc('T', 'G'),	// Togo  
			CC_TK = calulate_cc('T', 'K'),	// Tokelau  
			CC_TO = calulate_cc('T', 'O'),	// Tonga  
			CC_TT = calulate_cc('T', 'T'),	// Trinidad and Tobago  
			CC_TN = calulate_cc('T', 'N'),	// Tunisia  
			CC_TR = calulate_cc('T', 'R'),	// Türkiye  
			CC_TM = calulate_cc('T', 'M'),	// Turkmenistan  
			CC_TC = calulate_cc('T', 'C'),	// Turks and Caicos Islands  
			CC_TV = calulate_cc('T', 'Z'),	// Tuvalu  
			CC_UG = calulate_cc('U', 'G'),	// Uganda  
			CC_UA = calulate_cc('U', 'A'),	// Ukraine  
			CC_AE = calulate_cc('A', 'E'),	// United Arab Emirates  
			CC_GB = calulate_cc('U', 'K'),	// United Kingdom  
			CC_US = calulate_cc('U', 'S'),	// United States  
			CC_UM = calulate_cc('U', 'M'),	// United States Minor Outlying Islands  
			CC_UY = calulate_cc('U', 'Y'),	// Uruguay  
			CC_UZ = calulate_cc('U', 'Z'),	// Uzbekistan  
			CC_VU = calulate_cc('V', 'U'),	// Vanuatu  
			CC_VE = calulate_cc('V', 'E'),	// Venezuela, Bolivarian Republic of  
			CC_VN = calulate_cc('V', 'N'),	// Viet Nam  
			CC_VG = calulate_cc('V', 'G'),	// Virgin Islands, British  
			CC_VI = calulate_cc('V', 'I'),	// Virgin Islands, U.S.  
			CC_WF = calulate_cc('W', 'F'),	// Wallis and Futuna  
			CC_EH = calulate_cc('E', 'H'),	// Western Sahara  
			CC_YE = calulate_cc('V', 'E'),	// Yemen  
			CC_ZM = calulate_cc('Z', 'M'),	// Zambia  		CC_ZW,	// Zimbabwe
			CC_UNDEFINED
		};

		country_codes get_code(char a, char b);
	}
}

#endif 
