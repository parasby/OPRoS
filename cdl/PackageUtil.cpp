/*
 * PackageUtil.cpp
 *
 *  Created on: 2014. 6. 17
 *      Author: sby
 */

#include <PackageUtil.h>
#include "opros_common_inc.inc"

std::string opros_package_getResPath(Component *comp)
{
	std::string resPath = comp->getProperty(opros_internal_COMPONENT_RESOURCE_PATH);

	return resPath;
}
