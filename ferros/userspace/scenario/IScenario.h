#ifndef FERROS_ISCENARIO_H
#define FERROS_ISCENARIO_H

#include <string>

namespace ferros {
namespace simulation {

class IScenario {
public:
    virtual ~IScenario() = default;

    // Execute the bounded scenario
    virtual void execute() = 0;

    // Get the name of the scenario
    virtual std::string getName() const = 0;

    // Get the type of the scenario (e.g., "attack" or "normal")
    virtual std::string getType() const = 0;

    // Get the configured duration in seconds
    virtual int getDurationSec() const = 0;

    // Get the number of CPU threads used (if applicable, else 0)
    virtual int getCpuThreads() const = 0;
};

} // namespace simulation
} // namespace ferros

#endif // FERROS_ISCENARIO_H
