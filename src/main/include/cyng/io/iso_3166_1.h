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
			CC_FK,	// Falkland Islands (Malvinas)  
			CC_FO,	// Faroe Islands  
			CC_FJ,	// Fiji  
			CC_FI,	// Finland  
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
			CC_HT,	// Haiti  
			CC_HM,	// Heard Island and McDonald Islands  
			CC_VA,	// Holy See (Vatican City State)  
			CC_HN,	// Honduras  
			CC_HK,	// Hong Kong  
			CC_HU,	// Hungary  
			CC_IS,	// Iceland  
			CC_IN,	// India  
			CC_ID,	// Indonesia  
			CC_IR,	// Iran, Islamic Republic of  
			CC_IQ,	// Iraq  
			CC_IE,	// Ireland  
			CC_IM,	// Isle of Man  
			CC_IL,	// Israel  
			CC_IT,	// Italy  
			CC_JM,	// Jamaica  
			CC_JP,	// Japan  
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
			CC_MO,	// Macao  
			CC_MK,	// Macedonia, the Former Yugoslav Republic of  
			CC_MG,	// Madagascar  
			CC_MW,	// Malawi  
			CC_MY,	// Malaysia  
			CC_MV,	// Maldives  
			CC_ML,	// Mali  
			CC_MT,	// Malta  
			CC_MH,	// Marshall Islands  
			CC_MQ,	// Martinique  
			CC_MR,	// Mauritania  
			CC_MU,	// Mauritius  
			CC_YT,	// Mayotte  
			CC_MX,	// Mexico  
			CC_FM,	// Micronesia, Federated States of  
			CC_MD,	// Moldova, Republic of  
			CC_MC,	// Monaco  
			CC_MN,	// Mongolia  
			CC_ME,	// Montenegro  
			CC_MS,	// Montserrat  
			CC_MA,	// Morocco  
			CC_MZ,	// Mozambique  
			CC_MM,	// Myanmar  
			CC_NA,	// Namibia  
			CC_NR,	// Nauru  
			CC_NP,	// Nepal  
			CC_NL,	// Netherlands  
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
			CC_RE,	// R\u00e9union  
			CC_RO,	// Romania  
			CC_RU,	// Russian Federation  
			CC_RW,	// Rwanda  
			CC_BL,	// Saint Barth\u00e9lemy  
			CC_SH,	// Saint Helena, Ascension and Tristan da Cunha  
			CC_KN,	// Saint Kitts and Nevis  
			CC_LC,	// Saint Lucia  
			CC_MF,	// Saint Martin (French part)  
			CC_PM,	// Saint Pierre and Miquelon  
			CC_VC,	// Saint Vincent and the Grenadines  
			CC_WS,	// Samoa  
			CC_SM,	// San Marino  
			CC_ST,	// Sao Tome and Principe  
			CC_SA,	// Saudi Arabia  
			CC_SN,	// Senegal  
			CC_RS,	// Serbia  
			CC_SC,	// Seychelles  
			CC_SL,	// Sierra Leone  
			CC_SG,	// Singapore  
			CC_SX,	// Sint Maarten (Dutch part)  
			CC_SK,	// Slovakia  
			CC_SI,	// Slovenia  
			CC_SB,	// Solomon Islands  
			CC_SO,	// Somalia  
			CC_ZA,	// South Africa  
			CC_GS,	// South Georgia and the South Sandwich Islands  
			CC_SS,	// South Sudan  
			CC_ES = calulate_cc('E', 'S'),	// Spain  
			CC_LK,	// Sri Lanka  
			CC_SD,	// Sudan  
			CC_SR,	// Suriname  
			CC_SJ,	// Svalbard and Jan Mayen  
			CC_SZ,	// Swaziland  
			CC_SE = calulate_cc('S', 'E'),	// Sweden  
			CC_CH = calulate_cc('C', 'H'),	// Switzerland  
			CC_SY,	// Syrian Arab Republic  
			CC_TW,	// Taiwan, Province of China  
			CC_TJ,	// Tajikistan  
			CC_TZ,	// Tanzania, United Republic of  
			CC_TH,	// Thailand  
			CC_TL,	// Timor-Leste  
			CC_TG,	// Togo  
			CC_TK,	// Tokelau  
			CC_TO,	// Tonga  
			CC_TT,	// Trinidad and Tobago  
			CC_TN,	// Tunisia  
			CC_TR,	// Turkey  
			CC_TM,	// Turkmenistan  
			CC_TC,	// Turks and Caicos Islands  
			CC_TV,	// Tuvalu  
			CC_UG,	// Uganda  
			CC_UA,	// Ukraine  
			CC_AE,	// United Arab Emirates  
			CC_GB = calulate_cc('U', 'K'),	// United Kingdom  
			CC_US = calulate_cc('U', 'S'),	// United States  
			CC_UM,	// United States Minor Outlying Islands  
			CC_UY,	// Uruguay  
			CC_UZ,	// Uzbekistan  
			CC_VU,	// Vanuatu  
			CC_VE,	// Venezuela, Bolivarian Republic of  
			CC_VN,	// Viet Nam  
			CC_VG,	// Virgin Islands, British  
			CC_VI,	// Virgin Islands, U.S.  
			CC_WF,	// Wallis and Futuna  
			CC_EH,	// Western Sahara  
			CC_YE,	// Yemen  
			CC_ZM,	// Zambia  		CC_ZW,	// Zimbabwe
			CC_UNDEFINED
		};

		country_codes get_code(char a, char b);
	}
}

#endif 
