################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./functiongenerator.obj" \
"./lightsensor.obj" \
"./main.obj" \
"./driverlib/MSP430FR2xx_4xx/adc.obj" \
"./driverlib/MSP430FR2xx_4xx/crc.obj" \
"./driverlib/MSP430FR2xx_4xx/cs.obj" \
"./driverlib/MSP430FR2xx_4xx/ecomp.obj" \
"./driverlib/MSP430FR2xx_4xx/eusci_a_spi.obj" \
"./driverlib/MSP430FR2xx_4xx/eusci_a_uart.obj" \
"./driverlib/MSP430FR2xx_4xx/eusci_b_i2c.obj" \
"./driverlib/MSP430FR2xx_4xx/eusci_b_spi.obj" \
"./driverlib/MSP430FR2xx_4xx/framctl.obj" \
"./driverlib/MSP430FR2xx_4xx/gpio.obj" \
"./driverlib/MSP430FR2xx_4xx/icc.obj" \
"./driverlib/MSP430FR2xx_4xx/lcd_e.obj" \
"./driverlib/MSP430FR2xx_4xx/mpy32.obj" \
"./driverlib/MSP430FR2xx_4xx/pmm.obj" \
"./driverlib/MSP430FR2xx_4xx/rtc.obj" \
"./driverlib/MSP430FR2xx_4xx/sac.obj" \
"./driverlib/MSP430FR2xx_4xx/sfr.obj" \
"./driverlib/MSP430FR2xx_4xx/sysctl.obj" \
"./driverlib/MSP430FR2xx_4xx/tia.obj" \
"./driverlib/MSP430FR2xx_4xx/timer_a.obj" \
"./driverlib/MSP430FR2xx_4xx/timer_b.obj" \
"./driverlib/MSP430FR2xx_4xx/tlv.obj" \
"./driverlib/MSP430FR2xx_4xx/wdt_a.obj" \
"./jsmn/jsmn.obj" \
"../lnk_msp430fr2355.cmd" \
$(GEN_CMDS__FLAG) \
-lfrwp_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include driverlib/MSP430FR2xx_4xx/subdir_vars.mk
-include jsmn/subdir_vars.mk
-include subdir_rules.mk
-include driverlib/MSP430FR2xx_4xx/subdir_rules.mk
-include jsmn/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
function_generator_MSP-EXP430FR2356.out \

EXE_OUTPUTS__QUOTED += \
"function_generator_MSP-EXP430FR2356.out" \

BIN_OUTPUTS += \
function_generator_MSP-EXP430FR2356.txt \

BIN_OUTPUTS__QUOTED += \
"function_generator_MSP-EXP430FR2356.txt" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "function_generator_MSP-EXP430FR2356.out" secondary-outputs

# Tool invocations
function_generator_MSP-EXP430FR2356.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --code_model=large --data_model=large --use_hw_mpy=F5 --advice:power="all" --advice:hw_config=all --define=__MSP430FR2355__ --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"function_generator_MSP-EXP430FR2356.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccs1200/ccs/ccs_base/msp430/include" -i"C:/ti/ccs1200/ccs/ccs_base/msp430/lib/FR2xx" -i"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/lib" -i"C:/ti/ccs1200/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --priority --reread_libs --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="function_generator_MSP-EXP430FR2356_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "function_generator_MSP-EXP430FR2356.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

function_generator_MSP-EXP430FR2356.txt: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/hex430" --memwidth=8 --romwidth=8 --diag_wrap=off --ti_txt -o "function_generator_MSP-EXP430FR2356.txt" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "functiongenerator.obj" "lightsensor.obj" "main.obj" "driverlib\MSP430FR2xx_4xx\adc.obj" "driverlib\MSP430FR2xx_4xx\crc.obj" "driverlib\MSP430FR2xx_4xx\cs.obj" "driverlib\MSP430FR2xx_4xx\ecomp.obj" "driverlib\MSP430FR2xx_4xx\eusci_a_spi.obj" "driverlib\MSP430FR2xx_4xx\eusci_a_uart.obj" "driverlib\MSP430FR2xx_4xx\eusci_b_i2c.obj" "driverlib\MSP430FR2xx_4xx\eusci_b_spi.obj" "driverlib\MSP430FR2xx_4xx\framctl.obj" "driverlib\MSP430FR2xx_4xx\gpio.obj" "driverlib\MSP430FR2xx_4xx\icc.obj" "driverlib\MSP430FR2xx_4xx\lcd_e.obj" "driverlib\MSP430FR2xx_4xx\mpy32.obj" "driverlib\MSP430FR2xx_4xx\pmm.obj" "driverlib\MSP430FR2xx_4xx\rtc.obj" "driverlib\MSP430FR2xx_4xx\sac.obj" "driverlib\MSP430FR2xx_4xx\sfr.obj" "driverlib\MSP430FR2xx_4xx\sysctl.obj" "driverlib\MSP430FR2xx_4xx\tia.obj" "driverlib\MSP430FR2xx_4xx\timer_a.obj" "driverlib\MSP430FR2xx_4xx\timer_b.obj" "driverlib\MSP430FR2xx_4xx\tlv.obj" "driverlib\MSP430FR2xx_4xx\wdt_a.obj" "jsmn\jsmn.obj" 
	-$(RM) "functiongenerator.d" "lightsensor.d" "main.d" "driverlib\MSP430FR2xx_4xx\adc.d" "driverlib\MSP430FR2xx_4xx\crc.d" "driverlib\MSP430FR2xx_4xx\cs.d" "driverlib\MSP430FR2xx_4xx\ecomp.d" "driverlib\MSP430FR2xx_4xx\eusci_a_spi.d" "driverlib\MSP430FR2xx_4xx\eusci_a_uart.d" "driverlib\MSP430FR2xx_4xx\eusci_b_i2c.d" "driverlib\MSP430FR2xx_4xx\eusci_b_spi.d" "driverlib\MSP430FR2xx_4xx\framctl.d" "driverlib\MSP430FR2xx_4xx\gpio.d" "driverlib\MSP430FR2xx_4xx\icc.d" "driverlib\MSP430FR2xx_4xx\lcd_e.d" "driverlib\MSP430FR2xx_4xx\mpy32.d" "driverlib\MSP430FR2xx_4xx\pmm.d" "driverlib\MSP430FR2xx_4xx\rtc.d" "driverlib\MSP430FR2xx_4xx\sac.d" "driverlib\MSP430FR2xx_4xx\sfr.d" "driverlib\MSP430FR2xx_4xx\sysctl.d" "driverlib\MSP430FR2xx_4xx\tia.d" "driverlib\MSP430FR2xx_4xx\timer_a.d" "driverlib\MSP430FR2xx_4xx\timer_b.d" "driverlib\MSP430FR2xx_4xx\tlv.d" "driverlib\MSP430FR2xx_4xx\wdt_a.d" "jsmn\jsmn.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

