################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1200/ccs/ccs_base/msp430/include" --include_path="C:/Users/a0496455/TI Drive/2021-2022/TI/Auto_PDS_rotation_2/projects/Embedded_load _stepper/MSP430_software/MSP_Design_Workshop_Lab_Files/msp_workshop/FR6989_fram/lab_03b_button" --include_path="C:/Users/a0496455/TI Drive/2021-2022/TI/Auto_PDS_rotation_2/projects/Embedded_load _stepper/MSP430_software/MSP_Design_Workshop_Lab_Files/msp_workshop/FR6989_fram/lab_03b_button/driverlib/MSP430FR5xx_6xx" --include_path="C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR6989__ --define=_MPU_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


