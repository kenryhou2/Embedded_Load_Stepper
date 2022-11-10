lab_07a_energyTrace

This lab exercise introduces us to many of the techniques used for measuring and reducing power dissipation in a MSP430 based design.
We begin by learning how to use EnergyTrace to measure energy consumption in  programs. Using this (or more crudely, using a multi-meter) we can now judge the affects our low-power optimizations have on our system.

This lab doesn't involve writing much code; rather, we re-explore the Lab4a and Lab4c solutions to discover how to measure energy which will help us determine if our power optimiation efforts are taking us in the right direction.

General Procedure
- Do you have EnergyTrace hardware available?
  1. MSP-EXP430FR5969 Launchpad
  2. MSP-FET that lets you connect to any MSP430 target
  3. If not either of these, you'll have to use a multi-meter
     (we don't provide multi-meter use directions, but reference
      the Launchpad User's Guide which describes the process)
- Enable EnergyTrace in CCS
- Run with EnergyTrace to create and save an energy profile
- Use the "Free Run" capability of CCS to minimize emulation energy requirements
- Compare energy profiles
- For 'FR5969 users, view the CPU & Peripheral "States" featuer of EnergyTrace++

