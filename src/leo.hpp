#pragma once

#include <cadmium/modeling/devs/atomic.hpp>
#include <limits>
#include <ostream>

namespace leo {

inline constexpr double infinity = std::numeric_limits<double>::infinity();

struct SatelliteState {
    bool illuminate = false;
};

inline std::ostream& operator<<(std::ostream& out, const SatelliteState& s) {
    return out << (s.illuminate ? "illuminate" : "passive");
}

class Satellite : public cadmium::Atomic<SatelliteState> {
public:
    cadmium::Port<bool> beamOut;

    explicit Satellite(const std::string& id)
        : Atomic(id, SatelliteState{}), beamOut(addOutPort<bool>("beam_out")) {}

    void internalTransition(SatelliteState& s) const override {
        s.illuminate = !s.illuminate;
    }

    void externalTransition(SatelliteState&, double) const override {}

    void output(const SatelliteState& s) const override {
        beamOut->addMessage(!s.illuminate);
    }

    double timeAdvance(const SatelliteState& s) const override {
        return s.illuminate ? 2.0 : 3.0;
    }
};

enum class LockState { passive, acquiring, transmitting, releasing };

struct OutdoorUnitState {
    LockState phase = LockState::passive;
    double sigma = infinity;
};

inline std::ostream& operator<<(std::ostream& out, const OutdoorUnitState& s) {
    switch (s.phase) {
    case LockState::passive: return out << "passive";
    case LockState::acquiring: return out << "acquire_lock";
    case LockState::transmitting: return out << "tx_rx";
    case LockState::releasing: return out << "release_lock";
    }
    return out;
}

class OutdoorUnit : public cadmium::Atomic<OutdoorUnitState> {
public:
    cadmium::Port<bool> beamIn;
    cadmium::Port<bool> signalOut;

    explicit OutdoorUnit(const std::string& id)
        : Atomic(id, OutdoorUnitState{}),
          beamIn(addInPort<bool>("beam_in")),
          signalOut(addOutPort<bool>("signal_out")) {}

    void internalTransition(OutdoorUnitState& s) const override {
        s.phase = s.phase == LockState::acquiring
                      ? LockState::transmitting : LockState::passive;
        s.sigma = infinity;
    }

    void externalTransition(OutdoorUnitState& s, double elapsed) const override {
        s.sigma -= elapsed;
        for (const bool beam : beamIn->getBag()) {
            switch (s.phase) {
            case LockState::passive:
                if (beam) s = {LockState::acquiring, 2.0};
                break;
            case LockState::acquiring:
                if (!beam) s = {LockState::passive, infinity};
                break;
            case LockState::transmitting:
                if (!beam) s = {LockState::releasing, 1.0};
                break;
            case LockState::releasing:
                if (beam) s = {LockState::transmitting, infinity};
                break;
            }
        }
    }

    void output(const OutdoorUnitState& s) const override {
        signalOut->addMessage(s.phase == LockState::acquiring);
    }

    double timeAdvance(const OutdoorUnitState& s) const override {
        return s.sigma;
    }
};

} // namespace leo
