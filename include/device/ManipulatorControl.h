/*************************************************************

 file: ManipulatorControl.h
 author: H.Y. lee
 begin: January 31 2011
 copyright: (c) 2013 ED, OPRoS
 email: hylee@ed.co.kr

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
#ifndef	_MANIPULATORCONTROL_H_
#define	_MANIPULATORCONTROL_H_

#include "OprosDevice.h"
#include "ObjectPositionData.h"
#include "device/OprosPrintMessage.h"

using namespace std;


class ManipulatorControl : public OprosDevice
{
public:
	ManipulatorControl(void)	{	}
	virtual ~ManipulatorControl(void)	{	}

	protected:
		int	m_MotionID;
		int	m_Interpolation;
		int m_EncoderType;

	virtual int32_t SetPower(vector<OPRoS::Float64> power)			{ return API_NOT_SUPPORTED; }
	virtual int32_t GetPower(vector<OPRoS::Float64> &power)			{ return API_NOT_SUPPORTED; }
	virtual int32_t SetVelocity(vector<OPRoS::Float64> velocity)	{ return API_NOT_SUPPORTED; }
	virtual int32_t GetVelocity(vector<OPRoS::Float64> &velocity)	{ return API_NOT_SUPPORTED; }
	virtual int32_t SetPosition(vector<OPRoS::Float64> position)	{ return API_NOT_SUPPORTED; }
	virtual int32_t GetPosition(vector<OPRoS::Float64> &position)	{ return API_NOT_SUPPORTED; }

public:
	virtual int32_t		StopMotion (void)														{ return API_NOT_SUPPORTED; }
	virtual int32_t		RunHardHome(void)														{ return API_NOT_SUPPORTED; }
	virtual int32_t		RunSoftHome(void)														{ return API_NOT_SUPPORTED; }
	virtual	int32_t		GetJointAngles (vector<OPRoS::Float32> &pAngles)						{ return API_NOT_SUPPORTED; }
    virtual int32_t		GetJointAngle (int16_t nJointNo,float32_t &pVlaue)						{ return API_NOT_SUPPORTED; }
	virtual int32_t		RunXYZList (vector<OPRoS::ObjectPositionData>  &pXYZList)				{ return API_NOT_SUPPORTED; }
	virtual int32_t		RunJointList (vector<OPRoS:: Float32Array > &JointList)					{ return API_NOT_SUPPORTED; }
	virtual int32_t		RunJointListContinue (vector<OPRoS::Float32Array> &JointList,OPRoS::Int32Array  &time)			{ return API_NOT_SUPPORTED; }
	virtual int32_t		SetMotionID(int16_t nMotionID)											{ return API_NOT_SUPPORTED; }
	virtual int32_t		GetCompletedMotionID(int16_t &nID)										{ return API_NOT_SUPPORTED; }
	virtual int32_t		SetAccel (float32_t fAccel)												{ return API_NOT_SUPPORTED; }
	virtual int32_t		SetVelocity (float32_t fVelocity)										{ return API_NOT_SUPPORTED; }
	virtual int32_t		SetMotionDelay (int16_t nTime)											{ return API_NOT_SUPPORTED; }
	virtual int32_t		InterpolationType (int16_t nType)										{ return API_NOT_SUPPORTED; }
	virtual int32_t		SetEncoderType(int16_t nType)											{ return API_NOT_SUPPORTED; }
};


#endif	//	_MANIPULATORCONTROL_H_