#pragma once

#include <frc/geometry/Translation2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Pose2d.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <units/velocity.h>

namespace pathplanner {
    class PathPoint {
    public:
        frc::Translation2d m_position;
        frc::Rotation2d m_heading;
        frc::Rotation2d m_holonomicRotation;
        units::meters_per_second_t m_velocityOverride;

        constexpr PathPoint(frc::Translation2d position, frc::Rotation2d heading, frc::Rotation2d holonomicRotation, units::meters_per_second_t velocityOverride) : 
            m_position(position),
            m_heading(heading),
            m_holonomicRotation(holonomicRotation),
            m_velocityOverride(velocityOverride) {}

        constexpr PathPoint(frc::Translation2d position, frc::Rotation2d heading, frc::Rotation2d holonomicRotation) : 
            PathPoint(position, heading, holonomicRotation, -1_mps) {}

        constexpr PathPoint(frc::Translation2d position, frc::Rotation2d heading, units::meters_per_second_t velocityOverride) : 
            PathPoint(position, heading, frc::Rotation2d(), velocityOverride) {}

        constexpr PathPoint(frc::Translation2d position, frc::Rotation2d heading) : 
            PathPoint(position, heading, frc::Rotation2d(), -1_mps) {}

        static PathPoint fromCurrentHolonomicState(frc::Pose2d currentPose, frc::ChassisSpeeds currentSpeeds){
            units::meters_per_second_t linearVel = units::math::sqrt((currentSpeeds.vx * currentSpeeds.vx) + (currentSpeeds.vy * currentSpeeds.vy));
            frc::Rotation2d heading(units::math::atan2(currentSpeeds.vy, currentSpeeds.vx));
            return PathPoint(currentPose.Translation(), heading, currentPose.Rotation(), linearVel);
        }

        constexpr static PathPoint fromCurrentDifferentialState(frc::Pose2d currentPose, frc::ChassisSpeeds currentSpeeds){
            return PathPoint(currentPose.Translation(), currentPose.Rotation(), currentSpeeds.vx);
        }

        constexpr PathPoint(PathPoint&&) = default; // explicit default move constructor
        constexpr PathPoint& operator=(PathPoint&&) = default; // explicit default move assignment operator
        constexpr PathPoint(PathPoint const&) = default; // explicit default copy constructor
        constexpr PathPoint& operator=(PathPoint const&) = default; // explicit default copy assignment operator
    };
}