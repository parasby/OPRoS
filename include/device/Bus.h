/*************************************************************

 file: Bus.h
 author: E.C. Shin
 begin: January 31 2011
 copyright: (c) 2011 KITECH, OPRoS
 email: unchol@kitech.re.kr

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

#ifndef	_BUS_H_
#define	_BUS_H_

#include "OprosDevice.h"
#include "OprosLock.h"

class Bus : public OprosDevice
{
public:
	Bus(void) {
	}
	virtual ~Bus(void) {
	};

private:
	OprosLock _lock;

public:
	void Lock(void) {
		_lock.Lock();
	}

	void Unlock(void) {
		_lock.Unlock();
	}
};


#endif	//	_BUS_H_
