#ifndef __BLINKY_SYSTEM_HPP__
#define __BLINKY_SYSTEM_HPP__

// This is a coupled model, meaning it has no internal computation, and is
// used to connect atomic models.  So, it is necessary to include coupled.hpp
#include <modeling/devs/coupled.hpp>

#ifdef EMBED
    #include "../../IO_Models/accelerometerInput.hpp"
    #include "../../IO_Models/digitalInput.hpp"
    #include "../../IO_Models/digitalOutput.hpp"
    #include "../../IO_Models/joystickInput.hpp"
    #include "../../IO_Models/lcdOutput.hpp"
    #include "../../IO_Models/lightSensorInput.hpp"
    #include "../../IO_Models/microphoneInput.hpp"
    #include "../../IO_Models/pwmOutput.hpp"
    #include "../../IO_Models/temperatureSensorInput.hpp"
#else
    #include <lib/iestream.hpp>
#endif

// We include any models that are directly contained within this coupled model
#include <blinky.hpp>

namespace cadmium::blinkySystem {
    class blinkySystem : public Coupled {
        public:
        blinkySystem(const std::string& id): Coupled(id){

            // Declare and initialize all controller models (non-input/output)
            auto blinky = addComponent<Blinky>("blinky");

            // Connect any non-input/output models with coupling
            // (NOT APPLICABLE FOR THIS MODEL)

        #ifdef EMBED

            // Declare and initialize all embedded input/output models
            auto digitalInput = addComponent<DigitalInput>("digitalInput",GPIO_PORT_P1,GPIO_PIN1);
            auto digitalOutput = addComponent<DigitalOutput>("digitalOutput",GPIO_PORT_P1,GPIO_PIN0);

            // Connect IO models with coupling to the system
            addCoupling(digitalInput->out,blinky->in);
            addCoupling(blinky->out,digitalOutput->in);

        #else

            // Declare and initialize all simulated input files (these must exist in the file system before compilation)
            auto textInput = addComponent<cadmium::lib::IEStream<bool>>("textInput","input.txt");

            // Connect the input files to the rest of the simulation with coupling
            addCoupling(textInput->out,blinky->in);

        #endif
        }
    };
} // namespace cadmium::blinkySystem

#endif // __BLINKY_SYSTEM_HPP__
