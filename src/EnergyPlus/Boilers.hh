// EnergyPlus, Copyright (c) 1996-2018, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef Boilers_hh_INCLUDED
#define Boilers_hh_INCLUDED

// ObjexxFCL Headers
#include <ObjexxFCL/Array1D.hh>

// EnergyPlus Headers
#include <DataGlobals.hh>
#include <EnergyPlus.hh>
#include <PlantComponent.hh>

namespace EnergyPlus {

namespace Boilers {

    // Using/Aliasing

    // Data
    // MODULE PARAMETER DEFINITIONS

    // Boiler normalized efficiency curve types
    enum class EfficiencyCurveType {
        NotSet,
        Linear,
        BiLinear,
        Quadratic,
        BiQuadratic,
        Cubic,
        QuadraticLinear,
        BiCubic,
        Default=NotSet,
    };

    // water temperature evaluation method
    enum class TemperatureEvaluationModeType {
        NotSet,
        Entering,
        Leaving,
        Default=NotSet,
    };

    // Boiler flow modes
    enum class FlowModeType {
        NotSet,
        Constant,
        NotModulated,
        LeavingSetPointModulated,
        Default=NotSet,
    };

    // DERIVED TYPE DEFINITIONS

    // MODULE VARIABLE DECLARATIONS:
    extern int NumBoilers;            // Number of boilers

    // SUBROUTINE SPECIFICATIONS FOR MODULE Boilers

    // Types
    class BoilerObject : public PlantComponent
    {
        // Members
        public:
            virtual ~BoilerObject()
            {
            }

            std::string Name;             // user identifier
            
            // Operational fault parameters
            bool FaultyBoilerFoulingFlag;     // True if the boiler has fouling fault
            int FaultyBoilerFoulingIndex;     // Index of the fault object corresponding to the boiler
            Real64 FaultyBoilerFoulingFactor; // Boiler fouling factor

                                              // member functions
            static PlantComponent *factory(std::string objectName);

            void simulate(const PlantLocation &calledFromLocation, bool const FirstHVACIteration, Real64 &CurLoad, bool const RunFlag, int const EquipFlowCtrl) override;

            void getDesignCapacities(const PlantLocation &EP_UNUSED(calledFromLocation), Real64 &MaxLoad, Real64 &MinLoad, Real64 &OptLoad) override;

            void onInitLoopEquip(const PlantLocation &EP_UNUSED(calledFromLocation)) override;

            void getSizingFactor(Real64 &SizFac) override;

            void doSizing();

            // public getters/setters
            Real64 getDesignNominalCapacity();
            Real64 getDesignVolumeFlowRate();

            void setDesignNominalCapacity(Real64 const capacity);
            void setDesignVolumeFlowRate(Real64 const flowRate);

            // set the sizing factor for the boiler, if sizingFactor <= 0.0, it is set to 1.0
            void setDesignSizingFactor(Real64 const sizingFactor);
            void setLoopNumber(int const loopNumber);
            void setDesignOutletTemperature(Real64 const temperature);

            // set the outlet limit temperature, if temperature <= 0.0, set it to 99.9
            void setDesignOutletTemperatureLimit(Real64 const temperature);

            // Default Constructor
            BoilerObject()
                : FuelType(0), TypeNum(0), LoopNum(0), LoopSideNum(0), BranchNum(0), CompNum(0), designNominalCapacity_(0.0),
                  designNominalCapacityWasAutoSized_(false), designEfficiency_(0.0), designOutletTemperature_(0.0),
                  designFlowMode_(FlowModeType::Default), ModulatedFlowSetToLoop(false), ModulatedFlowErrDone(false), designVolumeFlowRate_(0.0),
                  designVolumeFlowRateWasAutoSized_(false), designMassFlowRate_(0.0), designSizingFactor_(0.0), nodeHotWaterInletIndex_(0),
                  nodeHotWaterOutletIndex_(0), designMinPartLoadRatio_(0.0), designMaxPartLoadRatio_(0.0), designOptimalPartLoadRatio_(0.0),
                  efficiencyCurveTemperatureMode_(TemperatureEvaluationModeType::Default), curveEfficiencyIndex_(0),
                  efficiencyCurveType_(EfficiencyCurveType::Default), designOutletTemperatureLimit_(0.0), designParasiticElectricalLoad_(0.0),
                  EffCurveOutputError(0), EffCurveOutputIndex(0), CalculatedEffError(0), CalculatedEffIndex(0), FaultyBoilerFoulingFlag(false),
                  FaultyBoilerFoulingIndex(0), FaultyBoilerFoulingFactor(1.0), doOneTimeInitialisation(true), doEnvironmentInitialisation(true),
                  operatingLoad_(0.0), operatingHeatingEnergy_(0.0), operatingParasiticElectricalPower_(0.0),
                  operatingParasiticElectricalConsumption_(0.0), operatingMassFlowRate_(0.0), operatingInletTemperature_(0.0),
                  operatingPartLoadRatio_(0.0), operatingFuelUseRate_(0.0), operatingFuelUse_(0.0)
            {
            }

        private:
            int FuelType;                                                       // resource type assignment
            int TypeNum;                                                        // plant loop type identifier
            int LoopNum;                                                        // plant loop connection
            int LoopSideNum;                                                    // plant loop side connection
            int BranchNum;                                                      // plant loop branch connection
            int CompNum;                                                        // plant loop component connection
            Real64 designNominalCapacity_;                                      // W - design nominal capacity of Boiler
            bool designNominalCapacityWasAutoSized_;                            // true if previous was set to autosize input
            Real64 designEfficiency_;                                           // boiler efficiency at design conditions
            Real64 designOutletTemperature_;                                    // C - Boiler design outlet temperature
            FlowModeType designFlowMode_;                                       // one of 3 modes for componet flow during operation
            bool ModulatedFlowSetToLoop;                                        // True if the setpoint is missing at the outlet node
            bool ModulatedFlowErrDone;                                          // true if setpoint warning issued
            Real64 designVolumeFlowRate_;                                       // m3/s - Boiler water design volumetric flow rate
            bool designVolumeFlowRateWasAutoSized_;                             // true if previous was set to autosize input
            Real64 designMassFlowRate_;                                         // kg/s - Boiler water design mass flow rate
            Real64 designSizingFactor_;                                         // sizing factor
            int nodeHotWaterInletIndex_;                                        // Node number at the boiler inlet
            int nodeHotWaterOutletIndex_;                                       // Node number at the boiler outlet
            Real64 designMinPartLoadRatio_;                                     // Minimum allowed operating part load ratio
            Real64 designMaxPartLoadRatio_;                                     // Maximum allowed operating part load ratio
            Real64 designOptimalPartLoadRatio_;                                 // Optimal operating part load ratio
            TemperatureEvaluationModeType efficiencyCurveTemperatureMode_;      // water temp to use in curve, switch between entering and leaving
            int curveEfficiencyIndex_;                                          // Index to efficiency curve
            EfficiencyCurveType efficiencyCurveType_;                           // Type of efficiency curve
            Real64 designOutletTemperatureLimit_;                               // C - Boiler outlet maximum temperature limit
            Real64 designParasiticElectricalLoad_;                              // W - Parasitic electric power (e.g. forced draft fan)
            int EffCurveOutputError;                                            // efficiency curve output <=0 recurring warning error counter
            int EffCurveOutputIndex;                                            // efficiency curve output <=0 recurring warning error message index
            int CalculatedEffError;                                             // calculated efficiency >1.1 recurring warning error counter
            int CalculatedEffIndex;                                             // calculated efficiency >1.1 recurring warning error message index
            std::string EndUseSubcategory;                                      // identifier use for the end use subcategory
            bool doOneTimeInitialisation;                                       // do the one time initialisation, i.e. locate on plantloops etc.
            bool doEnvironmentInitialisation;                                   // do the environment initialisation, i.e. get inlet conditions etc.
            Real64 operatingLoad_;                                              // W - Boiler Load
            Real64 operatingHeatingEnergy_;                                     // J - Boiler energy integrated over time
            Real64 operatingParasiticElectricalPower_;                          // W - Parasitic electrical power (e.g. forced draft fan)
            Real64 operatingParasiticElectricalConsumption_;                    // J - Parasitic Electrical Consumption (e.g. forced draft fan)
            Real64 operatingMassFlowRate_;                                      // kg/s - Boiler mass flow rate
            Real64 operatingInletTemperature_;                                  // C - Boiler inlet temperature
            Real64 operatingOutletTemperature_;                                 // C - Boiler outlet temperature
            Real64 operatingPartLoadRatio_;                                     // Boiler operating part-load ratio
            Real64 operatingFuelUseRate_;                                       // W - Boiler fuel used
            Real64 operatingFuelUse_;                                           // J - Boiler Fuel consumed integrated over time

            // static member functions
            static void GetBoilerInput();

            void initialise(); // number of the current boiler being simulated

            void calculate(Real64 const MyLoad,    // W - hot water demand to be met by boiler
                           bool const RunFlag,     // TRUE if boiler operating
                           int const EquipFlowCtrl // Flow control mode for the equipment
            );

            bool hasTwoVariableEfficiencyCurve();

            void clearOperatingVariables();

            // transfer data from the boiler to outlet nodes
            void update();

            // update all reporting variables
            void report();
    };

    // Object Data
    extern Array1D<BoilerObject> Boiler;      // boiler data - dimension to number of machines

    void clear_state();

} // namespace Boilers

} // namespace EnergyPlus

#endif
