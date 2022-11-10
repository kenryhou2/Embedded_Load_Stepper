[Version]
4.60 (beta)


[Workshop]
MSP Design Workshop


[URL]
Please refer to the MSP Workshop Wiki page for more information. To contact 
the author, please use the email address found on this wiki page.
http://processors.wiki.ti.com/index.php/MSP_Design_Workshop

For access to the Value-Line training (or currently missing chapters), 
please refer to: http://processors.wiki.ti.com/index.php/Getting_Started_with_the_MSP430_LaunchPad_Workshop_v220
(or find a link for "Archived Versions" on the MSP Workshop wiki page)


[Abstract]
The The MSP Design Workshop is an in-depth introduction into MSP430 basics. 
The LaunchPad is an easy-to-use development tool intended for beginners and experienced 
users alike for creating microcontroller-based applications.


[Agenda]
In this workshop you will learn:

1.  Introduction - An introduction to the MSP430 Family of Products
2.  Code Composer Studio (CCS) - Introduction to Code Composer Studio IDE 
3.  Using MSP430ware with GPIO - Use MSP430ware's DriverLib to utilize GPIO 
4.  Clocking & Init - Configure the various clocks on the MSP430 
5.  Interrupts - Working with interrupts on the MSP430
6.  Timers - Exploring Timer_A (and _B) 
7.  Low Power Optimization (LPM) - Applying ULP Concepts using EnergyTrace and ULP Advisor
8.  Using the Real Time Clock (RTC)
9.  Non-Volatile Memory (NVM) - Using FRAM and Flash
10. USB Communications - Using the MSP430's USB module and API Stack ('F5529 Launchpad Only)
11. Energia (Arduino) - Using Energia (Arduino for the MSP430 and MSP432)  
12. Liquid Crystal Displays - Using Segmented LCD's ('FR4311 and 'FR6989 Launchpads Only)


[Future Plans]
The following additional content is on the wishlist for this workshop:
- Textual narrative for chapters:   
  -- 2nd half of the USB chapter (and cleanup of lab solution)
  -- LCD Chapter (for now, see PPT notes for slide-by-slide narrative)
- Video recording for LCD chapter
- Video recording for introductory chapter
- Updates to all the chapters to include MSP432 specific information
- Additional Future Chapters:
  -- CCS Cloud
  -- Introduction to Embedded System Software
  -- Using the Analog Converter
  -- Serial Communications on the MSP
  -- USB memory interface (MSC, FatFS) (USB Launchpad only)
  -- DMA
  -- BSL


[History]
Change History for this Workshop:

Version 4.60 (2016.05.09)  (Beta version)
-------------------------
- Updated to MSPWare_3_30_00_18 (which includes CMSIS updates for MSP432)
- Added FR5994 solutions to lab exercises
- Cleaned up 'FR6989 and MSP432 lab exercises


Version 4.51 (2015.08.28)
-------------------------
- This release only updates the Lab Files:
  -- Uses latest version of MSPWare (MSPWare_2_30_00_49)
    (although there were no changes to DriverLib versus MSPWare_2_21_00_39)
  -- Converted MSP432 DriverLib calls to "MAP_" function calls - which means
     the ROM'd version of the function will be used, if it exists
  -- Although Energia lab write-up has not been updated, the solutions now
     have been verified for all LaunchPads supported by this workshop
  -- Updates to Lab 6c HAL files to include MSP432 and FR6989
  -- Errata fixes for lab files


Version 4.50 (2015.08.01)
-------------------------
- Uses latest version of MSPWare (MSPWare_2_21_00_39) and CCS
- 'FR6989 LaunchPad is now supported in these workshop materials
- Minor improvements to the MSP432 labs exercises and solutions (doesn't include lab write-ups, yet)
- Improvements to the Segmented LCD examples


Version 4.32 (2015.03.24)
-------------------------
- Errata fixes to lab exercises
- Lab Solutions for MSP432 Launchpad
- Unfortunately, this update does not include:
  -- Updated installation guide for CCSv6.1 and MSPWare_2_00_00_40
  -- Updated lab instructions which include steps for the MSP432 Launchpad
  -- Updated discussion materials which include MSP432 content
  -- Updated videos which include MSP432 content
     

Version 4.01 (2015.02.08)
-------------------------
- Errata fixes
- Narrative/script for LCD Chapter 12
- Cleaned up Lab 12 and provided full worksheet solutions


Version 4.00 (2015.01.10)
-------------------------
- Added support for the 'FR4133 Launchpad (discussions and lab solutions)
- Added LCD chapter showing how to use LCD controller ('FR4311)
  -- Includes two new LCD lab exercises
- Updated labs to MSP430ware v1.97 - this includes API changes that bring
  it into the new common API standards across TI embedded processors


Version 3.21 (2014.07.24)
-------------------------
- (Ch 7) Clarified information for connecting MSP-FET to F5529 Launchpad
- (Ch 9) Non-Volatile Memory (FRAM/Flash)
  -- Add #pragma Persistent to FRAM discussiong
  -- Added new MPU GUI lab for FRAM devices
  -- Reworked lab exercises to take into account changes in default
     linker command files, as well as Persistent pragma
  -- Changed calls from MPU driverlib module due to newly deprecated functions
  -- Added code to lab_09b_mpu_with_driverlib showing how to use
     system reset interrupts (SNMI, UNMI, SYSRST)
  -- Added discussion and code showning use of _system_pre_init() function


Version 3.20 (2014.07.07)
-------------------------
- New "Low Power Optimization" chapter that includes "Energy Trace" and "ULP Advisor"
  -- EnergyTrace excercise uses the 'FR5969 Launchpad or the MSP-FET along with the 'F5529 LP
  -- Added directions for hooking new MSP-FET to 'F5529 Launchpad
- New "Real-Time Clock" (RTC) chapter (no lab exercise, yet)
- Reordered chapters 7-11 (caused by inserting two new chapters as 7 and 8)
- Added hints to many of the lab & worksheet questions
- Tweaked clock rates in Lab 4 (better clock rates for labs in later chapters)
- Re-worked much of the Timer lab
- Added code to set PxIES register
- Workshop fully ported for CCSv6
- Renamed lab directory from "FR5969_wolverine" to "FR5969_fram"
- Updated for MSP430ware_1_90_00_30
- Upgraded to MSP430Flasher_1.3.1


Version 3.10b (2014.05.12)
--------------------------
- Textual narrative for NVM chapter and half of the USB chapter (the rest are already complete)
- Various errata fixes (esp the path error in Lab 3a)
- Fixed broken link in Installation Guide
- Lab files have not changed


Version 3.10a (2014.05.01)
--------------------------
- Installation Guide was updated to reflect changes due to the production release
  of Code Composer Studio version 6 (CCSv6).
- Installation directions for CCSv5.5 were removed.
- MSP430ware is now installed as part of the new CCSv6 App Center.
- Lab files have not changed


Version 3.10 (2014.03.07)
-------------------------
- Added 'FR5969 Launchpad (Wolverine) solutions to the workshop
- Added Chapter 7 on Non-Volatile Memory (Flash & FRAM)
- Moved USB to Chapter 8 and Energia to Chapter 9
- Changed USB (Lab 8d) to use HID rather than CDC
- Added optional lab exercises:
  -- lab_06b_upTimerB_solution
  -- lab_06c_timerHal_solution
- Tools:
  -- Updated for MSP430ware_1_70_00_28 -- this version has API changes, so you must use
     this version for the solutions to work
  -- Workshop supports either CCSv5.5 or CCSv6


Files Affected:  (All files were changed in this release)
- MSP430_Workshop_Installation_Guide_v3_10.pdf
- MSP430_Workshop_v3_10.pdf
- MSP430_Workshop_Lab_Guide_v3_10.pdf
- MSP430_Workshop_setup_v3_10.exe
- msp430_workshop_v3_10_ppt.ZIP
- msp430_workshop_checksums.md5


Version 3.02a (2014.02.21)
--------------------------
- Missing pages from Chapter 5 were fixed.

Files Affected:  (All files were changed in this release)
- MSP430_Workshop_v3_02a.pdf


Version 3.02 (2014.01.27)
-------------------------
- Added narrative text to Chapters 2, 3, 4, 5, 6. (Chapter 7 is the only one left to do.)
- Updated solution for lab02c_oob 
  -- It currently had the beta version of the F5529 Out-of-box User Experience lab
  -- The lab now uses the 16K limited version of the project, in case students need the 
     more limited form to work with the free license of CCS
- Typo and minor errata correction to chapters 2-6.

Files Affected:  (All files were changed in this release)
- MSP430_Workshop_Installation_Guide_v3_02.pdf
- MSP430_Workshop_v3_02.pdf
- MSP430_Workshop_Lab_Guide_v3_02.pdf
- MSP430_Workshop_setup_v3_02.exe
- msp430_workshop_v3_02_ppt.ZIP
- msp430_workshop_checksums.md5


Version 3.01 (2013.11.07)
-------------------------
Chapter 1
- Streamlined chapter 1 to cut down on its length
- Added narrative text to chapter 1 in the Student Notes (plan on doing this for all chapters)

Chapter 7
- Minor errata fixes
- Added slide to 'Final Thoughts' area

Installer (updated 2013.12.19)
- Now allows you to choose a different "intall-to" location during setup

Files Affected:
- MSP430_Workshop_Installation_Guide_v3.01.pdf
- MSP430_Workshop_v3_01.pdf
- MSP430_Workshop_Lab_Guide_v3_01.pdf
- msp430_workshop_v3_01_ppt.zip
- MSP430_Launchpad_Workshop_Readme.txt
- MSP430_Workshop_setup_v3_01.exe


Version 3.00 (2013.10.31)
-------------------------
Brand new workshop

Based on 
- 'F5529 USB Launchpad
- CCSv5.5
- MSP430ware driverlib
- USB Developers Package

Hardware Required
- 'F5529 Launchpad
- One Jumper Wire

All files affected

Changes in Content
- All chapters and labs have been modified (except Energia)
  -- Introduction
  -- GPIO
  -- Clocking & Init
  -- Interrupts
  -- Timers

- New Chapters
  -- USB
  -- MSP430ware

- Some chapters have been removed (but will be reinstated in the future)
  -- Low Power Optimization
  -- Serial Communications
  -- Cap Touch
  -- ADC
  -- FRAM
  -- GRACE






Version 2.2.2 (2013.07.10)
--------------------------
1. Re-added the Energia chapter (no changes)


Version 2.2.1 (2013.06.06)
--------------------------
1. (Chapter 10) Updated marketing slide
2. (Chapter 11) Energia chapter temporarily removed

Files affected:
- MSP430_10_Capacitive_Touch.pdf
- MSP430_11_Energia.pdf
- MSP430_LaunchPad_Workshop_v2.21.pdf
- MSP430_Launchpad_Workshop_v2.21.ppt.zip


Version 2.2.0 (2013.05.31)
--------------------------
1. Updated Capacitive Touch chapter
   - Chapter completely re-written
   - Lab updated to us new TouchPro GUI tool
   - Lab updated to use new CapTouch Power Designer tool
   - Lab10d now uses Grace

2. Officially adding Energia chapter/lab to the workshop

3. PowerPoint files for Chapters 10 and 11 (CapTouch and Energia) are being released independently 
   from the rest of the chapters. If you have to have the PPT's, make sure you download all 3 files.

4. Install document (and Lab 1) updated to reflect changes to the release (i.e. new/updated labs)

5. Changing how solutions will be included with the workshop. You might notice for Lab10 that we
   have included archive (zip) files for each solution. These were created by using the CCS export
   feature to create an archive of the working lab files

6. Moving to CCSv5.4 - at this point, though, only Lab10 has been officially verified

7. Depricating support for earlier version of the MSP430 Value-line Launchpad
   From now on, we are only supporting v1.5 of the Launchpad; the board using the MSP430G2553
   device. While most lab exercises will probably continue to run on the older (less capable)
   device, we won't be testing our solutions (or writing our lab steps) with the older board.
     
   If you must continue to use the older Launchpad, we recommend that you use v2.1 of the
   workshop materials.
