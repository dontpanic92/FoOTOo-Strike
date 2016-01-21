/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/


// NewtonVehicleControllerManager.h: interface for the NewtonVehicleControllerManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef D_CUSTOM_VEHICLE_CONTROLLER_JOINT_H_
#define D_CUSTOM_VEHICLE_CONTROLLER_JOINT_H_

#include <CustomJointLibraryStdAfx.h>
#include <CustomAlloc.h>


class CustomVehicleController;
class CustomVehicleControllerBodyState;

class CustomVehicleControllerJoint: public dComplemtaritySolver::dBilateralJoint
{
	public:
	CUSTOM_JOINTS_API CustomVehicleControllerJoint();
	CUSTOM_JOINTS_API virtual ~CustomVehicleControllerJoint();
	CUSTOM_JOINTS_API virtual void Init(CustomVehicleController* const controller, CustomVehicleControllerBodyState* const state0, CustomVehicleControllerBodyState* const state1);

	protected:
	//CUSTOM_JOINTS_API virtual void JacobianDerivative (dComplemtaritySolver::dParamInfo* const constraintParams); 
	//CUSTOM_JOINTS_API virtual void UpdateSolverForces (const dComplemtaritySolver::dJacobianPair* const jacobians) const; 
	CUSTOM_JOINTS_API virtual void JointAccelerations (dComplemtaritySolver::dJointAccelerationDecriptor* const accelParam);

	CUSTOM_JOINTS_API void InitPointParam (dComplemtaritySolver::dPointDerivativeParam& param, const dVector& pivot) const;
	CUSTOM_JOINTS_API void AddAngularRowJacobian (dComplemtaritySolver::dParamInfo* const constraintParams, const dVector& dir, dFloat jointAngle);
	CUSTOM_JOINTS_API void AddLinearRowJacobian (dComplemtaritySolver::dParamInfo* const constraintParams, const dVector& pivot, const dVector& dir);
	CUSTOM_JOINTS_API void AddAngularRowJacobian (dComplemtaritySolver::dParamInfo* const constraintParams, const dVector& dir0, const dVector& dir1, dFloat ratio);
	CUSTOM_JOINTS_API void CalculatePointDerivative (dComplemtaritySolver::dParamInfo* const constraintParams, const dVector& dir, const dComplemtaritySolver::dPointDerivativeParam& param);

	CustomVehicleController* m_controller; 

	friend class CustomVehicleController;
	friend class CustomVehicleControllerBodyState;
	friend class CustomVehicleControllerTireJoint;
	friend class CustomVehicleControllerBodyStateTire;
	friend class CustomVehicleControllerBodyStateChassis;
	friend class CustomVehicleControllerTireContactJoint;
	friend class CustomVehicleControllerComponentBrake;
	friend class CustomVehicleControllerComponentEngine;
	friend class CustomVehicleControllerComponentSteering;
};


class CustomVehicleControllerTireJoint: public CustomVehicleControllerJoint
{
	public:
	CUSTOM_JOINTS_API virtual void JacobianDerivative (dComplemtaritySolver::dParamInfo* const constraintParams); 
	CUSTOM_JOINTS_API virtual void UpdateSolverForces (const dComplemtaritySolver::dJacobianPair* const jacobians) const; 
};

class CustomVehicleControllerTireContactJoint: public CustomVehicleControllerJoint
{
	public:
	CUSTOM_JOINTS_API CustomVehicleControllerTireContactJoint ();

	CUSTOM_JOINTS_API virtual void SetContacts (int count, const NewtonWorldConvexCastReturnInfo* const contacts);
	CUSTOM_JOINTS_API virtual void UpdateSolverForces (const dComplemtaritySolver::dJacobianPair* const jacobians) const; 
	CUSTOM_JOINTS_API virtual void JacobianDerivative (dComplemtaritySolver::dParamInfo* const constraintParams); 
	CUSTOM_JOINTS_API virtual void JointAccelerations (dComplemtaritySolver::dJointAccelerationDecriptor* const accelParam);

	dVector m_contactsPoint[8];
	dVector m_contactsNormal[8];
	dFloat m_contactFriction[8];
	int m_contactCount;
//	dContact m_contacts[8];
};

class CustomVehicleControllerEngineDifferencialJoint: public CustomVehicleControllerJoint
{
	public:
	CUSTOM_JOINTS_API virtual void JacobianDerivative (dComplemtaritySolver::dParamInfo* const constraintParams); 
	CUSTOM_JOINTS_API virtual void UpdateSolverForces (const dComplemtaritySolver::dJacobianPair* const jacobians) const;
	dFloat m_radio0;
	dFloat m_radio1;
};


#endif 

