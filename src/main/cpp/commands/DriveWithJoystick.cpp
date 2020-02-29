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


frc4783::DriveWithJoystick::DriveWithJoystick(frc4783::rrDriveTrain* subsystem, RobotContainer* contained) : drivetrain(subsystem), container(contained){
        AddRequirements(subsystem);
        printf("DriveWithJoystick init completed");
    }

void frc4783::DriveWithJoystick::Execute(){
    float m_speed = (0.95 * container->ps4.GetRawAxis(RobotContainer::LEFT_Y_AXIS_E));
    float m_turn = (1.0 * container->ps4.GetRawAxis(RobotContainer::RIGHT_X_AXIS_E));

    //float m_speed = (0.95 * container->xbox.GetRawAxis(RobotContainer::LEFT_Y_AXIS));
    //float m_turn = (1.0 * container->xbox.GetRawAxis(RobotContainer::RIGHT_X_AXIS));

    if (container->ps4.GetRawButton(2)) {
        
        nt::NetworkTableEntry angleEntry;
        nt::NetworkTableEntry distanceEntry;
        nt::NetworkTableEntry H_errorEntry;
        
        auto inst = nt::NetworkTableInstance::GetDefault();
        auto table = inst.GetTable("Vision"); 
        H_errorEntry = table->GetEntry("H_error");
        double H_error = H_errorEntry.GetDouble(0);
        angleEntry = table->GetEntry("angle");
        distanceEntry = table->GetEntry("H_dist"); //m_turn represents the speed of the turn
        double distance = distanceEntry.GetDouble(0);

        //m_turn = angleEntry.GetDouble(0)*0.03; //field of view is of 53.5
        if(H_error > 0 && H_error < 165 ) {
            m_turn = (H_error*0.0020) + 0.45;
        } else if (H_error < 0 && H_error > -165) {
            m_turn = (H_error * 0.0020) - 0.45;
        }

        printf("dist error:  %f \n", distance);
        if(distance > 80) {
            m_speed = -0.4;
        } else {
            m_speed = distance * -0.015 - 0.23;
        }

        //m_turn = H_errorEntry.GetDouble(0)*-0.0045; //field of view is of 53.5
        //m_speed = 0.3 +(1/distanceEntry.GetDouble(0)*-0.8);    
        //printf("m_speed is of : %f \n m_turn is of : %f\n",m_speed, m_turn);
        //float pp_speed = (1 - (1/distanceEntry.GetDouble(0)))*0.4;
        //printf("m_speed is of %f \n", pp_speed);
    }
    
    drivetrain->ArcadeDrive(m_speed,m_turn);
} 

bool frc4783::DriveWithJoystick::IsFinished(){
    return false;
}

void frc4783::DriveWithJoystick::End(){
    drivetrain->Stop();
}

void frc4783::DriveWithJoystick::Interrupted(){
    End();
}

void frc4783::DriveWithJoystick::Initialize() {
    
}

} /* namespace frc4783 */
