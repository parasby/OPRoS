
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.1 [Symbol,Topic])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>


#include "RobotControlProvided.h"

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
	

#include "RobotController.h"


#ifdef dDOUBLE
#define dsDrawBox      dsDrawBoxD
#define dsDrawSphere   dsDrawSphereD
#define dsDrawCapsule  dsDrawCapsuleD
#define dsDrawCylinder dsDrawCylinderD
#endif


#define NUM 4

dWorldID world; 
dSpaceID space; 
dGeomID  ground;
dJointGroupID contactgroup;
dsFunctions fn;

typedef struct {       
  dBodyID body;        
  dGeomID geom;        
  double  l,r,m;       
} MyObject;


MyObject wheel[NUM], base, neck, camera;
dJointID joint[NUM], neck_joint, camera_joint;
static dReal v_r = 0.0,v_l = 0.0 ;                   
static dReal BASE_M    = 9.4;                        
static dReal BASE_S[3] = {0.4,0.4,0.1};              
static const dReal  CAMERA_M  = 0.2;
static const dReal  CAMERA_S[3]  = {0.2,0.2,0.05};
static const dReal  NECK_M    = 0.5;
static const dReal  NECK_L    = 0.2;
static const dReal  NECK_R    = 0.025;
static dReal WH_R0 = 0.05, WH_R1 = 0.105;            
static dReal WH_W  = 0.02, WH_M0 = 0.1, WH_M1 = 0.2; 
static dReal START_X = 0, START_Y = 0, START_Z=0.20; 
static dReal s_x = 0.0, s_y = 0.0;     


static void makeRobot()
{
  dMatrix3 R;
  dMass mass, mass2, mass3;

  base.body  = dBodyCreate(world);
  dMassSetZero(&mass);
  dMassSetBoxTotal(&mass,BASE_M,BASE_S[0],BASE_S[1],BASE_S[2]);
  dBodySetMass(base.body,&mass);

  base.geom = dCreateBox(space,BASE_S[0],BASE_S[1],BASE_S[2]);
  dGeomSetBody(base.geom,base.body);
  dBodySetPosition(base.body,START_X,START_Y,START_Z);


  neck.body  = dBodyCreate(world);
  dMassSetZero(&mass2);
  dMassSetCapsuleTotal(&mass2,NECK_M,2,NECK_R,NECK_L);
  dMassAdjust(&mass2,NECK_M);
  dBodySetMass(neck.body,&mass2);

  neck.geom = dCreateCCylinder(space,NECK_R,NECK_L);
  dGeomSetBody(neck.geom,neck.body);
  dBodySetPosition(neck.body,START_X, START_Y+0.5*BASE_S[1]-NECK_R,
		   START_Z+0.5*NECK_L);
  dGeomDisable(neck.geom);

  
  camera.body  = dBodyCreate(world);
  dMassSetZero(&mass3);
  dMassSetBoxTotal(&mass3,CAMERA_M,CAMERA_S[0],CAMERA_S[1],CAMERA_S[2]);
  dMassAdjust(&mass3,CAMERA_M);
  dBodySetMass(camera.body,&mass3);

  camera.geom = dCreateBox(space,CAMERA_S[0],CAMERA_S[1],CAMERA_S[2]);
  dGeomSetBody(camera.geom,camera.body);
  dBodySetPosition(camera.body,START_X, START_Y+0.5*BASE_S[1]-0.1*CAMERA_S[1],
		   START_Z+NECK_L);
  dGeomDisable(camera.geom);


  
  dRFromAxisAndAngle(R,0,1,0,M_PI/2.0);
  for (int i = 0; i < NUM; i++) {
    wheel[i].body = dBodyCreate(world);

    dMassSetZero(&mass);
    if ((i % 2) == 0) {
      dMassSetCylinderTotal(&mass,WH_M0, 2, WH_R0, WH_W);
      dBodySetMass(wheel[i].body,&mass);
      wheel[i].geom = dCreateCylinder(space, WH_R0, WH_W);
    }
    else {
      dMassSetCylinderTotal(&mass,WH_M1, 2, WH_R1, WH_W);
      dBodySetMass(wheel[i].body,&mass);
      wheel[i].geom = dCreateCylinder(space, WH_R1, WH_W);
    }
    dGeomSetBody(wheel[i].geom, wheel[i].body);
    dBodySetRotation(wheel[i].body,R);
  }

  dReal w1x = 0.5 * (BASE_S[1]+WH_W);
  dReal w1z = START_Z - 0.5 * BASE_S[2];
  dReal w0y = 0.5 * BASE_S[1] - WH_R0;
  dReal w0z = START_Z - 0.5 * BASE_S[2] - WH_R0;

  dBodySetPosition(wheel[0].body,    0,  w0y, w0z);
  dBodySetPosition(wheel[1].body, -w1x,    0, w1z);
  dBodySetPosition(wheel[2].body,    0, -w0y, w0z);
  dBodySetPosition(wheel[3].body,  w1x,    0, w1z);

  
  for (int i = 0; i < NUM; i++) {
    joint[i] = dJointCreateHinge(world,0);
    dJointAttach(joint[i], base.body, wheel[i].body);
  }
  dJointSetHingeAxis(joint[0],1, 0, 0);
  dJointSetHingeAxis(joint[1],1, 0, 0);
  dJointSetHingeAxis(joint[2],1, 0, 0);
  dJointSetHingeAxis(joint[3],1, 0, 0);
  dJointSetHingeAnchor(joint[0],    0,  w0y, w0z);
  dJointSetHingeAnchor(joint[1], -w1x,    0, w1z);
  dJointSetHingeAnchor(joint[2],    0, -w0y, w0z);
  dJointSetHingeAnchor(joint[3],  w1x,    0, w1z);

// joints
  neck_joint = dJointCreateHinge(world,0);
  dJointAttach(neck_joint,neck.body,base.body);
  dJointSetHingeAxis(neck_joint,0, 0, 1);
  dJointSetHingeAnchor(neck_joint, START_X, START_Y+0.5*BASE_S[1]-NECK_R,
		   START_Z);
  dJointSetHingeParam(neck_joint,dParamLoStop, 0);
  dJointSetHingeParam(neck_joint,dParamHiStop, 0);


  // camera joints
  camera_joint = dJointCreateHinge(world,0);
  dJointAttach(camera_joint,neck.body,camera.body);
  dJointSetHingeAxis(camera_joint,1,0,0);
  dJointSetHingeAnchor(camera_joint,START_X, START_Y+0.5*BASE_S[1]-0.1*CAMERA_S[1],
		   START_Z+NECK_L);
  dJointSetHingeParam(camera_joint,dParamLoStop, 0.05 * M_PI);
  dJointSetHingeParam(camera_joint,dParamHiStop, 0.05 * M_PI);
}


static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i,n;

  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

  static const int N = 10;
  dContact contact[N];
  n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
        dContactSoftERP | dContactSoftCFM | dContactApprox1;
     	contact[i].surface.mu       = dInfinity; 
      contact[i].surface.slip1    = 0.01;      
      contact[i].surface.slip2    = 0.01;      
      contact[i].surface.soft_erp = 1.0;       
      contact[i].surface.soft_cfm = 1e-4;      
      dJointID c = dJointCreateContact(world,contactgroup,&contact[i]); 
      dJointAttach(c,b1,b2);              
    }
  }
}



static void drawRobot()
{
  dsSetColor(1.3,1.3,0.0);
  dsDrawBox(dGeomGetPosition(base.geom),
	    dGeomGetRotation(base.geom),BASE_S);


  dsDrawCylinder(dBodyGetPosition(neck.body),
             dBodyGetRotation(neck.body),NECK_L,NECK_R);


  dsDrawBox(dGeomGetPosition(camera.geom),
	    dGeomGetRotation(camera.geom),CAMERA_S);


  dsSetColor(1.1,1.1,1.1);
  for (int i=0; i< NUM; i++) {
		if ((i % 2) == 0) {
    	dsDrawCylinder(dBodyGetPosition(wheel[i].body),
             dBodyGetRotation(wheel[i].body),WH_W,WH_R0);
  	}
  	else {
    	dsDrawCylinder(dBodyGetPosition(wheel[i].body),
             dBodyGetRotation(wheel[i].body),WH_W,WH_R1);
		}
  }
}



static void field2RobotPos(const dReal p[2], const dReal c[2], dReal theta,
  dReal pr[2], dReal *r, dReal *phi)
{
  pr[0] =   (p[0]-c[0]) * cos(theta) + (p[1]-c[1]) * sin(theta);
  pr[1] = - (p[0]-c[0]) * sin(theta) + (p[1]-c[1]) * cos(theta);

  *r   = sqrt(pr[0] * pr[0] + pr[1] * pr[1]); 
  *phi = atan2(pr[0],pr[1]);                  
}

static int vision(dReal *r, dReal *phi, dBodyID obj_body)
{
  const dReal *pa,*pb,*pc;
  dReal pr[2], view_angle = M_PI/8.0;  

  pa  = dBodyGetPosition(obj_body);    
  pb  = dBodyGetPosition(base.body);   
  pc  = dBodyGetPosition(camera.body); 

  dReal theta  = atan2(pc[0]-pb[0],pc[1]-pb[1]); // «í«Ü«Ã«ÈªÎöÜÖØ
 
	theta = - theta;

  field2RobotPos(pa, pb, theta, pr, r, phi);

  if ((-view_angle <= *phi) && (*phi <= view_angle)) return 1;
  else                                               return 0;
}

static void controlWheel(dReal right, dReal left)
{
  double fMax = 100; 

  dJointSetHingeParam(joint[1],  dParamVel, left);
  dJointSetHingeParam(joint[1], dParamFMax, fMax);
  dJointSetHingeParam(joint[3],  dParamVel, right);
  dJointSetHingeParam(joint[3], dParamFMax, fMax);
}

static void turn(dReal speed)
{
  controlWheel(speed, -speed);
}


static void track(dReal phi, dReal speed)
{
  dReal k = 0.5;
  if (phi > 0) controlWheel(    speed,  k * speed);
  else         controlWheel(k * speed,      speed);
}

dReal heading()
{
	const dReal *pb,*pc;

  pb  = dBodyGetPosition(base.body);   
  pc  = dBodyGetPosition(camera.body); 

  return atan2(pc[0]-pb[0],pc[1]-pb[1]);
}

static void simLoop(int pause)
{
	static dReal pose = 0, pose_old = 0; 
	static dReal omega_r = 0, omega_l = 0, omega_r_old = 0, omega_l_old = 0;
	const dReal *pb;
	dReal r, phi; 
	dReal edges;
	dReal step_size = 0.005;

	if (!pause) {
  	dSpaceCollide (space,0,&nearCallback);
  	/*if (vision(&r, &phi, ball.body)) {
    	printf("tracking phi=%.2f \n",phi * 180/M_PI);
    	track(phi,4.0);
  	}
  	else {
    	printf("lost the ball phi=%.2f \n",phi * 180/M_PI);
    	turn(4.0);
  	}
	*/

	dWorldStep(world, step_size);
  	dJointGroupEmpty (contactgroup);

	
	pb  = dBodyGetPosition(base.body);

	
	pose    = heading(); 
	omega_r = dJointGetHingeAngleRate(joint[3]);
	omega_l = dJointGetHingeAngleRate(joint[1]);
	edges   = WH_R1 * (omega_r_old + omega_l_old) * cos(pose_old)/2.0
						+ WH_R1 * (omega_r     + omega_l    ) * cos(pose)/2.0;
	s_y     += 0.5 * step_size * edges;
	//printf("Real y = %f  Dead Reckoning s_y = %f Error = %f\n", pb[1], s_y, pb[1]-s_y);
	omega_r_old = omega_r;
	omega_l_old = omega_l;
	pose_old    = pose;
  }

  drawRobot();
}


static void command(int cmd)
{
  switch (cmd) {
  case 'j': v_r += 0.1; break;     
  case 'f': v_l += 0.1; break;     
  case 'k': v_r -= 0.1; break;     
  case 'd': v_l -= 0.1; break;     
  case 's': v_r = v_l = 0.0;break; 
  default: printf("\nInput j,f,k,d,l,s \n");break;
  }
}



static void start()
{
  float xyz[3] = {  0.0f,   0.0f, 3.0f};
  float hpr[3] = { 90.0f, -90.0f, 0.0f};

  dsSetViewpoint(xyz,hpr);
  dsSetSphereQuality(2);
}

static void setDrawStuff() {
  fn.version = DS_VERSION;
  fn.start   = &start;
  fn.step    = &simLoop;
  fn.command = &command;
  fn.stop = NULL;
  fn.path_to_textures = "./textures"; //"../../drawstuff/textures";
}

//
// constructor declaration
//
RobotController::RobotController()
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
RobotController::RobotController(const std::string &name):Component(name)
{

	
	portSetup();
}

//
// destructor declaration
//

RobotController::~RobotController() {
}

void RobotController::portSetup() {

	// provided service port setup
	
	ProvidedServicePort *pa1;

	pa1=new RobotControlProvided(this);

	addPort("RobotControl",pa1);



	// export variable setup

	EXPORT_VARIABLE("status",m_status);

	EXPORT_VARIABLE("statusInt",m_statusInt);

	EXPORT_VARIABLE("statusFloat",m_statusFloat);
}

// Call back Declaration
ReturnType RobotController::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType RobotController::onStop()
{
		dsStop();
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onExecute()
{

  dInitODE();
  setDrawStuff();

  world        = dWorldCreate();
  space        = dHashSpaceCreate(0);
  contactgroup = dJointGroupCreate(0);
  ground       = dCreatePlane(space,0,0,1,0);

  dWorldSetGravity(world, 0, 0, -9.8);

  makeRobot();

  dsSimulationLoop(NULL,NULL,640,480,&fn);

  dSpaceDestroy(space);
  dWorldDestroy(world);
  dCloseODE();
	
  return OPROS_SUCCESS;
}
	
ReturnType RobotController::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType RobotController::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



void RobotController::goUp()
{
	v_r += 0.1;
	v_l += 0.1;
	controlWheel(v_r, v_l);

	std::cout<<"Up Button Called"<<std::endl;
	// user code here
}
void RobotController::goDown()
{
	v_r -= 0.1;
	v_l -= 0.1;
	controlWheel(v_r, v_l);
	std::cout<<"Down Button Called"<<std::endl;
	// user code here
}
void RobotController::goLeft()
{
	v_r += 0.1;
	v_l -= 0.1;
	controlWheel(v_r, v_l);
	std::cout<<"Left Button Called"<<std::endl;
	// user code here
}
void RobotController::goRight()
{
	v_r -= 0.1;
	v_l += 0.1;
	controlWheel(v_r, v_l);
	std::cout<<"Right Button Called"<<std::endl;
	// user code here
}
void RobotController::Stop()
{
	v_r =0;
	v_l =0;
	controlWheel(v_r, v_l);
	std::cout<<"Stop Button Called"<<std::endl;
	// user code here
}




#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new RobotController();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new RobotController();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

