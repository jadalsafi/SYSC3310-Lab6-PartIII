#ifndef __BLINKY_HPP__
#define __BLINKY_HPP__

// This is an atomic model, meaning it has its' own internal logic/computation
// So, it is necessary to include atomic.hpp
#include <modeling/devs/atomic.hpp>

#if !defined NO_LOGGING || !defined EMBED
    #include <iostream>
#endif

namespace cadmium::blinkySystem {
    // A class to represent the state of this specific model
    // All atomic models will have their own state
    struct BlinkyState {

        // sigma is a mandatory variable, used to advance the time of the simulation
        double sigma;

        // Declare model-specific variables
        bool lightOn;
        bool fastToggle;

        // Set the default values for the state constructor for this specific model
        BlinkyState(): sigma(0), lightOn(false), fastToggle(false)  {}
    };

#if !defined NO_LOGGING || !defined EMBED
    /**
     * This function is used to output the current state of the model to the .csv output
     * file after each internal transition.  This is used to verify the accuracy of the
     * simulation.
     *
     * In this model, each time the internal transition function is invoked the current
     * output from the out port is listed, followed by the model's current state for
     * state.lightOn, and state.sigma.
     *
     * Note that state.sigma is NOT mandatory to include here, but it is listed due to
     * the desired program logic.
     *
     * @param out output stream.
     * @param s state to be represented in the output stream.
     * @return output stream with sigma and lightOn already inserted.
     */
    std::ostream& operator<<(std::ostream &out, const BlinkyState& state) {
        out << "{lightOn: " << state.lightOn << ", sigma: " << state.sigma << "}";
        return out;
    }
#endif

    // Atomic model of Blinky
    class Blinky : public Atomic<BlinkyState> {
     private:

     public:

        // Declare ports for the model

        // Input ports
        Port<bool> in;

        // Output ports
        Port<bool> out;

        // Declare variables for the model's behaviour
        double slowToggleTime;
        double fastToggleTime;

        /**
         * Constructor function for this atomic model, and its respective state object.
         *
         * For this model, both a Blinky object and a BlinkyState object
         * are created, using the same id.
         *
         * @param id ID of the new Blinky model object, will be used to identify results on the output file
         */
        Blinky(const std::string& id): Atomic<BlinkyState>(id, BlinkyState()) {

            // Initialize ports for the model

            // Input Ports
            in  = addInPort<bool>("in");

            // Output Ports
            out = addOutPort<bool>("out");

            // Initialize variables for the model's behaviour
            slowToggleTime = 2.0;
            fastToggleTime = 0.5;


            // Set a value for sigma (so it is not 0), this determines how often the
            // internal transition occurs
            state.sigma = slowToggleTime;
        }

        /**
         * The transition function is invoked each time the value of
         * state.sigma reaches 0.
         *
         * In this model, the value of state.lightOn is toggled.
         *
         * @param state reference to the current state of the model.
         */
        void internalTransition(BlinkyState& state) const override {

            state.lightOn = !state.lightOn;
#ifndef NO_LOGGING
            std::cout << "running internal transition: lightOn="
                      << std::boolalpha << state.lightOn << '\n';
#endif

        }

        /**
         * The external transition function is invoked each time external data
         * is sent to an input port for this model.
         *
         * In this model, the value of state.fastToggle is toggled each time the
         * button connected to the "in" port is pressed.
         *
         * The value of state.sigma is then updated depending on the value of
         * state.fastToggle.  Sigma is not required to be updated in this function,
         * but we are changing it based on our desired logic for the program.
         *
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         */
        void externalTransition(BlinkyState& state, double e) const override {

            // First check if there are un-handled inputs for the "in" port
            #ifndef NO_LOGGING
            std::cout << "running external transition: lightOn="
                      << std::boolalpha << state.lightOn << '\n';
#endif
            if(!in->empty()){

                // The variable x is created to handle the external input values in sequence.
                // The getBag() function is used to get the next input value.
                for( const auto x : in->getBag()){
                    if (x==0)
                        state.fastToggle = !state.fastToggle; // if the button was pressed, we change the speed of toggling
                }

                if(state.fastToggle)
                    state.sigma = fastToggleTime;
                else
                    state.sigma = slowToggleTime;
            }

        }

        /**
         * This function outputs any desired state values to their associated ports.
         *
         * In this model, the value of state.lightOn is sent via the out port.  Once
         * the value of state.ligthOn reaches the I/O model, that model will update
         * the status of the LED.
         *
         * @param state reference to the current model state.
         */
        void output(const BlinkyState& state) const override {

            #ifndef NO_LOGGING
            std::cout << "output function running: lightOn="
                      << std::boolalpha << state.lightOn << '\n';
#endif
            out->addMessage(state.lightOn);

        }

        /**
         * It returns the value of state.sigma for this model.
         *
         * This function is the same for all models, and does not need to be changed.
         *
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const BlinkyState& state) const override {

            return state.sigma;

        }
    };
} // namespace cadmium::blinkySystem

#endif // __BLINKY_HPP__
