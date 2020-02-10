/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
 
#include "commands/DriveWithJoystick.h"
#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"
#include <frc/smartdashboard/SmartDashboard.h>

namespace frc4783 {

DriveWithJoystick::DriveWithJoystick(DriveTrain* subsystem, RobotContainer* contained) : drivetrain(subsystem), container(contained){
        AddRequirements(subsystem);
    }

void DriveWithJoystick::Execute(){
    //float m_speed = (0.95 * Robot::robotcontainer->driveStick2->GetRawAxis(RobotContainer::LEFT_Y_AXIS_E));
    float m_speed = (0.95 * container->ps4.GetRawAxis(RobotContainer::LEFT_Y_AXIS_E));
    //float m_turn = (1.0 * Robot::robotcontainer->driveStick2->GetRawAxis(RobotContainer::RIGHT_X_AXIS_E));
    float m_turn = (1.0 * container->ps4.GetRawAxis(RobotContainer::RIGHT_X_AXIS_E));
    if (container->ps4.GetRawButton(2)) {
        nt::NetworkTableEntry angleEntry;
        nt::NetworkTableEntry distanceEntry;
        
        auto inst = nt::NetworkTableInstance::GetDefault();
        auto table = inst.GetTable("Vision"); 
        angleEntry = table->GetEntry("angle");
        distanceEntry = table->GetEntry("angle");//m_turn represents the speed of the turn 
        m_turn = angleEntry.GetDouble(0)/(53.5/2); //field of view is of 53.5
        m_speed = distanceEntry.GetDouble(0)* 0.5;
        //float pp_speed = (1 - (1/distanceEntry.GetDouble(0)))*0.4;
        //printf("m_speed is of %f \n", pp_speed);
    }
    
    drivetrain->ArcadeDrive(m_speed,m_turn);


} 

bool DriveWithJoystick::IsFinished(){
    return false;
}

void DriveWithJoystick::End(){
    drivetrain->Stop();
}

void DriveWithJoystick::Interrupted(){
    End();
}

void DriveWithJoystick::Initialize() {
    
}

} /* namespace frc4783 */