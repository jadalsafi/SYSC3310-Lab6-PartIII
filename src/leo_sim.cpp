#include "leo.hpp"
#include <cadmium/lib/iestream.hpp>
#include <cadmium/simulation/logger/csv.hpp>
#include <cadmium/simulation/root_coordinator.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: leo_sim satellite|odu|coupled input-file output-csv\n";
        return 1;
    }
    const std::string mode = argv[1];
    auto model = std::make_shared<cadmium::Coupled>(mode);
    if (mode == "satellite") {
        model->addComponent<leo::Satellite>("satellite");
    } else if (mode == "odu") {
        const auto input = model->addComponent<cadmium::lib::IEStream<bool>>("beam_input", argv[2]);
        const auto odu = model->addComponent<leo::OutdoorUnit>("odu");
        model->addCoupling(input->out, odu->beamIn);
    } else if (mode == "coupled") {
        const auto sat = model->addComponent<leo::Satellite>("satellite");
        const auto odu = model->addComponent<leo::OutdoorUnit>("odu");
        model->addCoupling(sat->beamOut, odu->beamIn);
    } else {
        std::cerr << "Unknown mode: " << mode << '\n';
        return 1;
    }

    cadmium::RootCoordinator simulator(model);
    simulator.setLogger<cadmium::CSVLogger>(argv[3], ";");
    simulator.start();
    simulator.simulate(20.0);
    simulator.stop();
}
