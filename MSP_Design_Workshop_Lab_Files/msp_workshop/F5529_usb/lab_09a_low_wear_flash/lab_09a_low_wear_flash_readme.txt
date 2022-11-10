lab_09a_info_low_wear_flash

Flash only -- FRAM parts don't really need to worry about wear issues due to their 
              ultra high endurance

This example modifies lab_09a_info_flash by using the entire infoB segment. In original 
exercise, we wrote count to the first location in Info B. On the next power-cycle
we erased the entire Info B segement and wrote one location; we did this again-and-again
on every power-cycle.

This solution provides a simple method of minimizing FLASH wear. Rather than erasing the
entire flash on each power-cycle, we use the consecutive location in flash. We keep doing
this until we reach the end of Info B; only when we reach the end do we erase the entire
segment and start over again.

While there are probably better algorithms to handle these types of flash wear issues, 
this is a simple example solution to the problem.


