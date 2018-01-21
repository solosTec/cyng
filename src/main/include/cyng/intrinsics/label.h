/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_LABEL_H
#define CYNG_VM_LABEL_H

#include <CYNG_project_info.h>
#include <string>
#include <iostream>

namespace cyng 
{
	/**
	 * jump address name
	 */
	struct label
	{
		const std::string name_;
		label();
		label(label const&);
		label(std::string const&);
	};

	//	comparison
	bool operator==(label const&, label const&);
	bool operator<(label const&, label const&);
	bool operator!=(label const&, label const&);
	bool operator>(label const&, label const&);
	bool operator<=(label const&, label const&);
	bool operator>=(label const&, label const&);

}

#endif	//	CYNG_VM_LABEL_H


