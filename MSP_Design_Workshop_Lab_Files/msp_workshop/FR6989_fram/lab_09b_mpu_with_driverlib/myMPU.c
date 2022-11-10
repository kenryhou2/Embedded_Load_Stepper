// ----------------------------------------------------------------------------
// myMPU.c  (for lab_09b_mpu_with_driverlib project)  ('FR5969 Launchpad)
// ----------------------------------------------------------------------------
#include <driverlib.h>
#include <stdio.h>
#include "myMPU.h"


//***** Extern Global Variables ************************************************
// This symbol is defined by the default 'FR65969 Linker Command File
extern const uint16_t fram_rx_start;               // Starting point for Read/Execute FRAM section


//***** Defines ****************************************************************
#define MAIN_FRAM_START  0x4400                    // Starting address for Main FRAM
#define INFOB_START      0x1900                    // Starting address for INFOB
#define TEST_DATA        0x88


//***** Global Variables *******************************************************
uint16_t *ptr   =   0;
uint16_t Data   =   0;
uint16_t rbData =   0;


//***** Functions **************************************************************
void initMPU(void)
{
    // Configure MPU Segments
    MPU_initTwoSegments( MPU_BASE,
            (uint16_t) &fram_rx_start >> 4,        // Boundary between segments
            MPU_READ | MPU_WRITE | MPU_EXEC,       // All access types to segment 1
            MPU_READ | MPU_EXEC );                 // Execute only access to segment 2

    // Configure what happens when a MPU violation occurs. Either a PUC (Reset) event
    // occurs or an System NMI can be generated (if NMI event is enabled)
    MPU_disablePUCOnViolation( MPU_BASE,                                  // Turning off PUC for all segments; thus
            MPU_FIRST_SEG | MPU_SECOND_SEG | MPU_THIRD_SEG );             // an NMI will be generated for any violation

    //*******************************************************************************************
    // If you choose to enable PUC for some segments but not others, it's recommended
    // that you call both enable & disable functions to properly configure all segments
    //
    // For example, enabling PUC for Segments 2 and 3 (and not for Segment 1)
    //*******************************************************************************************
    //    MPU_enablePUCOnViolation( MPU_BASE,
    //            MPU_SECOND_SEG | MPU_THIRD_SEG );    // Enable PUC for Segments 2 & 3
    //
    //    MPU_disablePUCOnViolation( MPU_BASE,
    //          MPU_FIRST_SEG );                       // Disable PUC for Segment 1
    //*******************************************************************************************

    //Enable NMI on segment access violation                              // Turning on NMI - with PUC "off" and NMI "on" and fault
    MPU_enableNMIevent( MPU_BASE );                                       //   generates an NMI rather than a PUC

    // Lock the MPU configuration (until next reset)
    //MPU_lockMPU( MPU_BASE );

    // Start MPU protection (enables MPU)
    MPU_start( MPU_BASE );
}


// This function currently performs a write to whichever segment location (vLocation) is passed in as an argument
void violateMemory( int16_t vLocation )
{
    switch (vLocation)
    {
    case 0:                                                // Set pointer to InfoB memory
        ptr = (uint16_t) INFOB_START;
    	printf("Writing to INFOB Segment at address 0x%x...\n", ptr);
        break;
    case 1:                                                // Set pointer to Segment 1
        ptr = (uint16_t) MAIN_FRAM_START;
    	printf("Writing to FRAM Segment 1 at address 0x%x...\n", ptr);
        break;
    case 2:                                                // Set pointer to Segment 2
        ptr = (uint16_t) MPUSEGB1 << 4;
    	printf("Writing to FRAM Segment 2 at address 0x%x...\n", ptr);
        break;
    case 3:                                                // Set pointer to Segment 3
        ptr = (uint16_t) MPUSEGB2 << 4;
    	printf("Writing to FRAM Segment 3 at address 0x%x...\n", ptr);
        break;
    case 4:                                                // Set pointer to FRAM control registers
        ptr = (uint16_t *) &MPUCTL0;                       // You can create a violation by writing to registers without a password
    	printf("Writing to MPU Control Register at address 0x%x...\n", ptr);
        break;
    }

    //Data value we'll use to cause a violation (arbitrary value)
    Data = TEST_DATA;

    // Write data to pointer location ... Causes an MPU violation if write permissions are off for that segment
    *ptr = Data;

    // Read back from the same location
    rbData = *ptr;

    printf("... 0x%x was written; 0x%x was read back from address 0x%x", Data, rbData, ptr);
    if ( Data == rbData )
    {
    	printf("   (PASSED)\n\n");
    }
    else
    {
    	printf("   (FAILED)\n\n");
    }
}

