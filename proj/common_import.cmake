set(REPO_PATH $ENV{REPO_DIR})

###################################################################
####### Debug probe: J-Link
###################################################################

add_custom_target(reset
   COMMAND openocd -f interface/jlink.cfg 
                   -c "transport select swd" 
                   -c "adapter speed 6000" 
                   -f target/rp2040.cfg 
                   -f ${REPO_PATH}/scripts/openocd_proc.cfg
                   -c "target_reset"
)

add_custom_target(program_flash
   COMMAND openocd -f interface/jlink.cfg 
                   -c "transport select swd" 
                   -c "adapter speed 6000" 
                   -f target/rp2040.cfg 
                   -f ${REPO_PATH}/scripts/openocd_proc.cfg
                   -c "target_program_flash ${PROJECT_NAME}.elf"
   DEPENDS ${PROJECT_NAME}
)

###################################################################
####### Debug probe: Raspberry Pi
###################################################################

add_custom_target(reset_rpi
   COMMAND openocd -f interface/cmsis-dap.cfg
                   -c "adapter speed 5000"
                   -f target/rp2040.cfg
                   -f ${REPO_PATH}/scripts/openocd_proc.cfg
                   -c "target_reset"
)

add_custom_target(program_flash_rpi
   COMMAND openocd -f interface/cmsis-dap.cfg
                   -c "adapter speed 5000"
                   -f target/rp2040.cfg
                   -f ${REPO_PATH}/scripts/openocd_proc.cfg
                   -c "target_program_flash ${PROJECT_NAME}.elf"
   DEPENDS ${PROJECT_NAME}
)
