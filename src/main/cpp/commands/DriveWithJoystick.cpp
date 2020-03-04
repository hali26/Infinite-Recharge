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

    if (container->ps4.GetRawButton(1)) { // (X) button on Xbox; Square button on PS4
        // Power port alignment

    }

    if (container->ps4.GetRawButton(4)) { // (Y) button on Xbox; Triangle button on PS4
        // Power cell alignment
        
        // Declares variables from network tables
        nt::NetworkTableEntry angleEntry;
        nt::NetworkTableEntry distanceEntry;
        nt::NetworkTableEntry H_errorEntry;
        
        // Specifies which table data is being collected from
        auto inst = nt::NetworkTableInstance::GetDefault();
        auto table = inst.GetTable("Vision"); 

        // Gets variable values from network tables
        H_errorEntry = table->GetEntry("H_error");
        angleEntry = table->GetEntry("angle");
        distanceEntry = table->GetEntry("H_dist");

        // Casts variables to doubles
        double H_error = H_errorEntry.GetDouble(0); // Horizontal error from power cell (angle)
        double distance = distanceEntry.GetDouble(0); // Distance from bumper to power cell

        m_speed = distance * -0.015 - 0.23; // Adjusts speed proportional to distance of power cell from bumper

        // Aligns robot to power cell with angle error
        if(H_error > 0 && H_error < 165 ) {
            m_turn = (H_error*0.0020) + 0.45;
        } else if (H_error < 0 && H_error > -165) {
            m_turn = (H_error * 0.0020) - 0.45;
        }
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
