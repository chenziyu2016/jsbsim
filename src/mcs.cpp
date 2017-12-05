#include "initialization/FGTrim.h"
#include "FGFDMExec.h"
#include <iostream>

JSBSim::FGFDMExec* FDMExec;
string aircraftName = "quad";
string resetName = "quad_lab_init";
string logDirectivePath = "scripts/quad_output.xml";
int simulationRate = 10000;

int main() {
    
    // create jsbsim object
    FDMExec = new JSBSim::FGFDMExec();
    
    // set paths for jsbsim
    //FDMExec->SetRootDir(rootDir);
    FDMExec->SetAircraftPath("definitions/aircraft");
    FDMExec->SetEnginePath("definitions/engines");
    FDMExec->SetSystemsPath("definitions/systems");
    
    // disable color highlighting
    FDMExec->disableHighLighting();
    
    // load aircraft model
    FDMExec->LoadModel(aircraftName);
    
    // load initial conditions
    FDMExec->GetIC()->Load(resetName);
    
    // set output directives
    FDMExec->SetOutputDirectives(logDirectivePath);
    
    // run initial conditions to set up the model
    FDMExec->RunIC();
    
    // set simulation rate
    FDMExec->Setdt(1.0/simulationRate);
    
	bool result = true;
    double throttle = 0.0;
    double time = 0.0;
    int i = 0;
    
	while (result) {

        // get simulation time 
        time = FDMExec->GetSimTime();
        
        // break simulation after 10 seconds
        if (time > 10.0) break;

        // obtain simulation inputs
        double pos_x = FDMExec->GetPropertyValue("position/distance-from-start-lon-mt");
        double pos_y = FDMExec->GetPropertyValue("position/distance-from-start-lat-mt");
        double pos_z = FDMExec->GetPropertyValue("position/h-agl-ft");
        
        if (i > 5000) {
            i = 0;
            std::cout << time << "\t" << pos_x << "\t" << pos_y << "\t" << pos_z << "\n";
            std::cout << time << "\t" << FDMExec->GetPropertyValue("/engines/engine[0]/rpm") << "\n";
        }
        
        i++;
        
        if (time > 1.0) {
            throttle = 0.9;
        
            // stimulate simulation inputs
            FDMExec->SetPropertyValue("fcs/throttle-cmd-norm[0]", throttle);
            FDMExec->SetPropertyValue("fcs/throttle-cmd-norm[1]", throttle);
            FDMExec->SetPropertyValue("fcs/throttle-cmd-norm[2]", throttle);
            FDMExec->SetPropertyValue("fcs/throttle-cmd-norm[3]", throttle);
        }

        if (time > 5.0)
            throttle = 0.1;
        
        
        // run one simulation frame
        result = FDMExec->Run();
	}
	std::cout << "----------------------------\nSIMULATION FINISHED\n";
	return 0;
}
