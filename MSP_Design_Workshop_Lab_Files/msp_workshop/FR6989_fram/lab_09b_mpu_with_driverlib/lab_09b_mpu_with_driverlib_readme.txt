lab_09b_mpu_with_driverlib  (FR5969 and FR6989 only)

While this lab configures the 'FR5xx/6xx Memory Protection Unit (MPU) for the FRAM
non-volatile memory using the MSPware Driver Library, there are a number of
other items implemented in this program.

This program explores the following items:
- Builds on lab_09a_info_fram (that flashes the LED the number of times the
  program has been reset or power-cycled)
- Uses the _system_pre_init() function to configure WDT and MPU before reaching main()
- Initalizes the MPU:
  -- Using 2 segments (with Read/Execute defined by linker command file)
  -- Setting up violation on write to Segment 3 (Location of code)
  -- System NMI is generated on violation (as opposed to PUC)
  -- MPU is started, but not locked
- A function in the program is used to test the MPU's configuration by
  writing to the various segments trying to create violations; the results are
  reported back via printf()
- An example of FR5xx/6xx reset handlers are provided; including a function that
  tests for why the program was last reset 
- A simple example for creating SYSTEM event flags is provided. This can be
  used to flag reset/interrupt events so that your main program can respond
  to them (if needed). Also, we used PERSISTENT storage for these flags.
- "_DEBUG" symbol was added to "Debug" build configuration of the project; 
  this allows the use of if{} statements to exclude some code, such as 
  printf() statements, if project is built in other build configurations.  
 
Files included in the project:
- hello.c : Carried over from the previous lab, but quite a bit has been
            added to it.
- myMPU.c : Provides the function that initializes the MPU; as well as the
            function which causes memory violations
- system_isr_routines.c : Includes the interrupt handlers for Reset, System NMI,
                          and User NMI events. Additionally, it contains our
                          _system_pre_init() function call.

The system_isr_routines.c files provides a good template for handling MSP430 System 
Reset Events. For more information about this, check out the following wiki page:
  http://processors.wiki.ti.com/index.php/Handling_MSP430_System_Reset_Events


Basic Lab Exercise Steps:
- Open and build the project
- Run the program and examine the console output
- Import "watch_expressions.txt" into Expressions window
- Suspend the program and put a breakpoint at the start of _system_pre_init()
- Reset the CPU and single-step through the initMPU to see how these functions
  work - watch how the MPU registers get modified
- Similarly, set breakpoints in the interrupt routines to see them working
- Try changing the 'enablePUC' and 'enableNMI' options, then rebuilding the
  program to see how these items change the results of the violation tests
  

