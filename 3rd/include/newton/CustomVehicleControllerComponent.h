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

#ifndef D_CUSTOM_VEHICLE_CONTROLLER_COMPONENT_H_
#define D_CUSTOM_VEHICLE_CONTROLLER_COMPONENT_H_

#include <CustomJointLibraryStdAfx.h>
#include <CustomVehicleControllerJoint.h>
#include <CustomAlloc.h>

class dBilateralJoint;
class CustomVehicleController;
class CustomVehicleControllerBodyState;
class CustomVehicleControllerBodyStateTire;

class CustomVehicleControllerComponent: public CustomAlloc  
{
	public:

	class dInterpolationCurve
	{
		public:
		class dNot
		{
			public:
			dFloat m_param;
			dFloat m_value;
		};

		dInterpolationCurve ()
		{	
			m_count = 0;
		}

		~dInterpolationCurve ()
		{
		}

		CUSTOM_JOINTS_API void InitalizeCurve (int points, const dFloat* const steps, const dFloat* const values);
		CUSTOM_JOINTS_API dFloat GetValue (dFloat param) const;

		dNot m_nodes[6];
		int m_count;
	};

	void SetParam(dFloat param)
	{
		m_paramMemory = m_param;
		m_param = param;
	}

	dFloat GetParam() const 
	{
		return m_param;
	}

	protected:
	CustomVehicleControllerComponent (CustomVehicleController* const controller)
		:m_controller(controller)
		,m_param(0.0f)
		,m_paramMemory(0.0f)
		,m_timer(60)
	{
	}

	virtual ~CustomVehicleControllerComponent()
	{
	}

	bool ParamChanged()
	{
		m_timer --;
		if (dAbs (m_paramMemory - m_param) > 1.e-3f) {
			m_timer = 60;
		}
		return m_timer > 0;
	}

	virtual void Update (dFloat timestep) = 0;

	CustomVehicleController* m_controller; 
	dFloat m_param;
	dFloat m_paramMemory;
	int m_timer;

	friend class CustomVehicleController;
};

class CustomVehicleControllerComponentEngine: public CustomVehicleControllerComponent
{
	public:
	class dDifferential: public CustomAlloc  
	{
		public:
		CUSTOM_JOINTS_API dDifferential (CustomVehicleController* const controller);
		CUSTOM_JOINTS_API virtual ~dDifferential (){}

		CUSTOM_JOINTS_API dFloat GetShaftOmega() const;
		CUSTOM_JOINTS_API void ApplyTireTorque (dFloat shaftTorque, dFloat shaftOmega) const;
		
		CUSTOM_JOINTS_API virtual int GetAxelCount () const = 0;
		CUSTOM_JOINTS_API virtual int GetGainArray (dFloat * const gains) const = 0;
		CUSTOM_JOINTS_API virtual int GetTireArray(CustomVehicleControllerBodyStateTire** const array) const = 0;
		CUSTOM_JOINTS_API virtual int GetDifferentialJoints (dComplemtaritySolver::dBilateralJoint** const buffer) = 0;
	};

	class dSingleAxelDifferential: public dDifferential
	{
		public:
		CUSTOM_JOINTS_API dSingleAxelDifferential (CustomVehicleController* const controller, CustomVehicleControllerBodyStateTire* const leftTire, CustomVehicleControllerBodyStateTire* const rightTire);
		CUSTOM_JOINTS_API virtual ~dSingleAxelDifferential (){}

		CUSTOM_JOINTS_API virtual int GetAxelCount () const;
		CUSTOM_JOINTS_API virtual int GetGainArray (dFloat * const gains) const;
		CUSTOM_JOINTS_API virtual int GetTireArray(CustomVehicleControllerBodyStateTire** const array) const;
		CUSTOM_JOINTS_API virtual int GetDifferentialJoints (dComplemtaritySolver::dBilateralJoint** const buffer);

		protected:
		dFloat m_gain0;
		dFloat m_gain1;
		CustomVehicleControllerBodyStateTire* m_tire0;
		CustomVehicleControllerBodyStateTire* m_tire1;
		CustomVehicleControllerEngineDifferencialJoint m_differentialJoint;
		friend class dMultiAxelDifferential;
	};

	class dMultiAxelDifferential: public dDifferential
	{
		public:
		class dGainAxelPair
		{
			public:
			dGainAxelPair ()
				:m_gain (0.0f)
				,m_axel(NULL)
			{
			}

			dGainAxelPair (const dGainAxelPair& copy)
			{
			}

			~dGainAxelPair()
			{
				if (m_axel) {
					delete m_axel;
				}
			}

			dFloat m_gain;
			dSingleAxelDifferential* m_axel;
		};

		CUSTOM_JOINTS_API dMultiAxelDifferential (CustomVehicleController* const controller, int count, dSingleAxelDifferential** const dDifferencialArray);
		CUSTOM_JOINTS_API virtual ~dMultiAxelDifferential();
		
		CUSTOM_JOINTS_API virtual int GetAxelCount () const;
		CUSTOM_JOINTS_API virtual int GetGainArray (dFloat * const gains) const;
		CUSTOM_JOINTS_API virtual int GetTireArray(CustomVehicleControllerBodyStateTire** const array) const;
		CUSTOM_JOINTS_API virtual int GetDifferentialJoints (dComplemtaritySolver::dBilateralJoint** const buffer);

		protected:
		dList<dGainAxelPair> m_list;
	};

	class dTracksSkidDifferential: public dDifferential
	{
		public:
		CUSTOM_JOINTS_API dTracksSkidDifferential (CustomVehicleController* const controller, CustomVehicleControllerBodyStateTire* const leftTire, CustomVehicleControllerBodyStateTire* const rightTire);
		CUSTOM_JOINTS_API virtual ~dTracksSkidDifferential (){}

		CUSTOM_JOINTS_API virtual int GetAxelCount () const;
		CUSTOM_JOINTS_API virtual int GetGainArray (dFloat * const gains) const;
		CUSTOM_JOINTS_API virtual int GetTireArray(CustomVehicleControllerBodyStateTire** const array) const;
		CUSTOM_JOINTS_API virtual int GetDifferentialJoints (dComplemtaritySolver::dBilateralJoint** const buffer);

		protected:
		dFloat m_gain0;
		dFloat m_gain1;
		CustomVehicleControllerBodyStateTire* m_tire0;
		CustomVehicleControllerBodyStateTire* m_tire1;
	};

	class dGearBox: public CustomAlloc
	{
	public:
		enum dGearID
		{
			m_reverseGear = 0,
			m_newtralGear,
			m_firstGear,
			m_maxGears = 16
		};

		class dGearState: public CustomAlloc  
		{
		public:
			dGearState (dFloat ratio, dFloat shiftUp, dFloat shiftDown, dGearID id) 
				:m_ratio(ratio)
				,m_shiftUp (shiftUp)
				,m_shiftDown (shiftDown)
				,m_next(NULL)
				,m_prev(NULL)
				,m_id(id)
			{
			}

			virtual ~dGearState()
			{
			}
			virtual dGearState* Update(CustomVehicleController* const vehicle);

			dFloat m_ratio;
			dFloat m_shiftUp;
			dFloat m_shiftDown;
			dGearState* m_next;
			dGearState* m_prev;
			dGearState* m_neutral;
			dGearState* m_reverse;
			dGearID m_id;
		};

		class dReverseGearState: public dGearState
		{
		public:
			dReverseGearState (dFloat ratio)
				:dGearState(-dAbs(ratio), 1000.0f, -1000.0f, m_reverseGear)
			{
			}
			virtual dGearState* Update(CustomVehicleController* const vehicle);
		};

		class dNeutralGearState: public dGearState
		{
		public:
			dNeutralGearState (dGearState* const first, dGearState* const reverse)
				:dGearState(0.0f, 1000.0f, -1000.0f, m_newtralGear)
			{
				m_next = first;
				m_prev = reverse;
			}
			dGearState* Update(CustomVehicleController* const vehicle);
		};

		CUSTOM_JOINTS_API dGearBox (CustomVehicleController* const controller, dFloat reverseGearRatio, int gearCount, const dFloat* const gearBoxRatios);
		CUSTOM_JOINTS_API ~dGearBox ();
		CUSTOM_JOINTS_API void Update (dFloat timestep);
		CUSTOM_JOINTS_API dFloat GetGearRatio(int gear) const;

		CUSTOM_JOINTS_API int GetGear() const;
		CUSTOM_JOINTS_API void SetGear (int gear);
		CUSTOM_JOINTS_API int GetGearCount() const;

		bool GetTransmissionMode () const;
		void SetTransmissionMode (bool mode);
		void SetOptimalShiftLimits (dFloat minShift, dFloat maxShift);

		dGearState* m_gears[m_maxGears];
		dGearState* m_currentGear;
		CustomVehicleController* m_controller;
		dFloat m_shiftTimer; 
		int m_gearsCount;

		bool m_automatic;
	};

	CUSTOM_JOINTS_API CustomVehicleControllerComponentEngine (CustomVehicleController* const controller, dGearBox* const gearBox, dDifferential* const differencial);
	CUSTOM_JOINTS_API ~CustomVehicleControllerComponentEngine();

	CUSTOM_JOINTS_API virtual void Update (dFloat timestep);

	CUSTOM_JOINTS_API void InitEngineTorqueCurve (dFloat vehicleSpeedInKilometerPerHours,
		dFloat idleTorqueInPoundFoot, dFloat revolutionsPerMinutesAtIdleTorque, 
		dFloat peakTorqueInPoundFoot, dFloat revolutionsPerMinutesAtPeakTorque, 
		dFloat peakHorsePower, dFloat revolutionsPerMinutesAtPeakHorsePower, 
		dFloat torqueArRedLineInPoundFoot, dFloat revolutionsPerMinutesAtRedLineTorque);

	CUSTOM_JOINTS_API bool GetKey() const;
	CUSTOM_JOINTS_API void SetKey (bool key);
	CUSTOM_JOINTS_API int GetGear () const;
	CUSTOM_JOINTS_API void SetGear (int gear);
	CUSTOM_JOINTS_API dFloat GetRPM () const;
	CUSTOM_JOINTS_API dFloat GetTopRPM () const;
	CUSTOM_JOINTS_API dFloat GetRedLineRPM () const;
	CUSTOM_JOINTS_API dFloat GetSpeed () const;
	CUSTOM_JOINTS_API dFloat GetTopSpeed () const;
	CUSTOM_JOINTS_API bool GetTransmissionMode () const;
	CUSTOM_JOINTS_API void SetTransmissionMode (bool mode);

	dGearBox* GetGearBox() const;
	dFloat GetTorque (dFloat radianPerSeconds) const;

	void SetTopSpeed (dFloat topSpeedMeterPerSecunds, dFloat rpsAtPeckPower);

	int AddDifferentialJoints (dComplemtaritySolver::dBilateralJoint** const buffer );

	protected:
	void ConvertToMetricSystem (dFloat& vehicleSpeedInKilometerPerHours,
			dFloat& idleTorqueInPoundFoot, dFloat& revolutionsPerMinutesAtIdleTorque, 
			dFloat& peakTorqueInPoundFoot, dFloat& revolutionsPerMinutesAtPeakTorque, 
			dFloat& peakHorsePower, dFloat& revolutionsPerMinutesAtPeakHorsePower, 
			dFloat& torqueArRedLineInPoundFoot, dFloat& revolutionsPerMinutesAtRedLineTorque) const;

	dGearBox* m_gearBox;
	dDifferential* m_differencial;
	dInterpolationCurve m_torqueCurve;

	dFloat m_speedMPS;
	dFloat m_engineRPS;
	dFloat m_topSpeedMPS;
	dFloat m_engineToque;
	dFloat m_crownGearRatio;
	dFloat m_engineIdleInvInertia;
	dFloat m_engineIdleResistance1;
	dFloat m_engineIdleResistance2;
	dFloat m_engineInternalFriction;
	dFloat m_clutchTorqueCouplingTime;
	dFloat m_radiansPerSecundsAtRedLine;
	dFloat m_radiansPerSecundsAtPeakPower;
	dFloat m_radiansPerSecundsAtIdleTorque;
	bool m_engineSwitch;

	friend class CustomVehicleControllerBodyStateTire;
	friend class CustomVehicleControllerBodyStateChassis;
};

class CustomVehicleControllerComponentBrake: public CustomVehicleControllerComponent
{
	public:
	CUSTOM_JOINTS_API CustomVehicleControllerComponentBrake (CustomVehicleController* const controller, dFloat maxBrakeTorque);
	CUSTOM_JOINTS_API void AddBrakeTire (CustomVehicleControllerBodyStateTire* const tire);
	CUSTOM_JOINTS_API virtual void Update (dFloat timestep);

	dFloat m_maxBrakeTorque;
	dList<dList<CustomVehicleControllerBodyStateTire>::dListNode*> m_brakeTires;

	friend class CustomVehicleControllerBodyStateTire;
	friend class CustomVehicleControllerBodyStateChassis;
};

class CustomVehicleControllerComponentSteering: public CustomVehicleControllerComponent
{
	public:
	CUSTOM_JOINTS_API CustomVehicleControllerComponentSteering (CustomVehicleController* const controller, dFloat maxAngleInRadians);
	CUSTOM_JOINTS_API void AddSteeringTire (CustomVehicleControllerBodyStateTire* const tire);

	CUSTOM_JOINTS_API dFloat GetMaxSteeringAngle () const;
	CUSTOM_JOINTS_API virtual void Update (dFloat timestep);
	CUSTOM_JOINTS_API void CalculateAkermanParameters (const CustomVehicleControllerBodyStateTire* const rearLeftTire, const CustomVehicleControllerBodyStateTire* const rearRightTire, 
													   const CustomVehicleControllerBodyStateTire* const frontLeftTire, const CustomVehicleControllerBodyStateTire* const frontRightTire); 


	dList<CustomVehicleControllerBodyStateTire*> m_steeringTires;
	dFloat m_maxAngle;
	dFloat m_akermanWheelBaseWidth;
	dFloat m_akermanAxelSeparation;

	friend class CustomVehicleControllerBodyStateTire;
	friend class CustomVehicleControllerBodyStateChassis;
};

class CustomVehicleControllerComponentTrackSkidSteering: public CustomVehicleControllerComponentSteering
{
	public:
	CUSTOM_JOINTS_API CustomVehicleControllerComponentTrackSkidSteering (CustomVehicleController* const controller, dFloat steeringRPM, dFloat teeringTorque);

	protected:
	CUSTOM_JOINTS_API virtual void Update (dFloat timestep);

	dFloat m_steeringRPM;
	dFloat m_steeringTorque;
	dFloat m_differencialTurnRate;
};

#endif 

