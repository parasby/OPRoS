/*************************************************************

 file: GpsData.h
 author: E.C. Shin
 begin: March 13 2013
 copyright: (c) 2012 KITECH, OPRoS
 email: uncholshin@gmail.com

***************************************************************

OPRoS source code is provided under a dual license mode:
 LGPL and OPRoS individually.

LGPL: You can redistribute it and/or modify it under the terms
 of the Less GNU General Public License as published by the Free
 Software Foundation, either version 3 of the License.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of 
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 Less GNU General Public License for more details. 

 You should have received a copy of the Less GNU General Public
 License along with this program. If not, see 
 <http://www.gnu.org/licenses/>.

OPRoS individual license: An individual license is a license for
 modifying the source code of OPRoS and distiributing it in a
 closed format for commercial purposes.

 If you are interested in this option, please see 
 <http://www.opros.or.kr>.

This license policy may be changed without prior notice.

***************************************************************/

#pragma once

#include "ApiTypes.h"

namespace OPRoS
{

class GpsData
{
public:
	GpsData(void) {
		status = OPROS_SUCCESS;
	}
	GpsData(float64_t latitude, float64_t longitude, std::string time, float64_t velocity) {
		status = OPROS_SUCCESS;
		this->latitude = latitude;
		this->longitude = longitude;
		this->time = time;
		this->velocity = velocity;
	}
	virtual ~GpsData(void) {
	}

	ReturnType status;
	float64_t latitude;
	float64_t longitude;
	std::string time;
	float64_t velocity;

	void save(opros::archive::oarchive &ar, const unsigned int)
	{
		ar << status;
		ar << latitude;		ar << longitude;
		ar << time;			ar << velocity;
	}

	void load(opros::archive::iarchive &ar, const unsigned int)
	{
		ar >> status;
		ar >> latitude;		ar >> longitude;
		ar >> time;			ar >> velocity;
	}

	GpsData operator=(const GpsData &arg)
	{
		this->status = arg.status;
		this->latitude = arg.latitude;
		this->longitude = arg.longitude;
		this->time = arg.time;
		this->velocity = arg.velocity;

		return *this;
	}
};
inline std::string typeName(GpsData *p)	{ return "GpsData"; }

}
