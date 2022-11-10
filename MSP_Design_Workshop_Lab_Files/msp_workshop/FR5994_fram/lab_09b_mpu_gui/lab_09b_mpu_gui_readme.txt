lab_09b_mpu_gui  (FR5994 and FR6989 Only)

As explored in Chapter 9, it’s important to protect your executable program 
and read-only data stored in FRAM by using the Memory Protection Unit (MPU). 
The "FRAM – Usage and Best Practices" application note puts it this way: 

	"It is very important to always appropriately configure and enable the MPU before any
	software deployment or production code release to ensure maximum application robustness
	and data integrity. The MPU should be enabled as early as possible after the device starts
	executing code coming from a power-on or reset at the beginning of the C startup routine
	even before the main() routine is entered."
 
The following lab exercise takes you through a couple of different ways you 
can set up the MPU:

- Using the MPU Graphical User Interface (GUI) found in CCSv6
- Using DriverLib code in MPU initialization function called from main()
- Using DriverLib code in MPU initialization function called from _system_pre_init()

You’ll find the GUI method to be quick and easy – thus we recommend that all FRAM (FR5xx/6xx)
users complete this exercise. While the 2nd and 3rd examples are not difficult, 
evaluating their code takes a little bit more time and effort, therefore we’ve 
marked them as “optional”.
