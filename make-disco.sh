#! /bin/bash
export RTT_EXEC_PATH="d:/Program Files/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_EABI/bin"
make
#openocd -f openocd.cfg -c "flash_image"
st-link_cli -c SWD UR LPM -p gcc/net.bin 0x08000000 -Rst -Run
#openocd -f openocd.cfg -c "flash_param"
