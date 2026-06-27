#ifndef CPU_H
#define CPU_H

#include <cstdint>

namespace baremetal {

class Cpu {
 public:
  // Default constructor.
  //
  // Usage:
  //
  // Cpu cpu;
  //
  Cpu() = default;

  // Default destructor.
  //
  // Usage:
  //
  // {
  //   Cpu cpu;
  // }
  // // Destructor called here automatically.
  //
  ~Cpu() = default;

  // Copy constructor.
  //
  // This would allow:
  //
  // Cpu cpu1;
  // Cpu cpu2(cpu1);
  //
  // or:
  //
  // Cpu cpu2 = cpu1;
  //
  // We forbid this because there is only one CPU.
  //
  Cpu(const Cpu&) = delete;

  // Copy assignment operator.
  //
  // This would allow:
  //
  // Cpu cpu1;
  // Cpu cpu2;
  //
  // cpu2 = cpu1;
  //
  // We forbid assigning one CPU to another.
  //
  Cpu& operator=(const Cpu&) = delete;

  // Move constructor.
  //
  // This would allow:
  //
  // Cpu cpu1;
  //
  // Cpu cpu2(std::move(cpu1));
  //
  // or:
  //
  // Cpu cpu2 = std::move(cpu1);
  //
  // Moving a CPU does not make sense because CPU models
  // unique hardware.
  //
  Cpu(Cpu&&) = delete;

  // Move assignment operator.
  //
  // This would allow:
  //
  // Cpu cpu1;
  // Cpu cpu2;
  //
  // cpu2 = std::move(cpu1);
  //
  // Again, CPUs are not movable.
  //
  Cpu& operator=(Cpu&&) = delete;

  void Reset(std::uint32_t initial_sp,
             std::uint32_t reset_pc);

  [[nodiscard]]
  std::uint32_t pc() const;

  [[nodiscard]]
  std::uint32_t sp() const;

 private:
  std::uint32_t pc_ = 0;

  std::uint32_t sp_ = 0;
};

}  // namespace baremetal

#endif // CPU_H