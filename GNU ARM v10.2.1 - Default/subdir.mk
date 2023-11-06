################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app.c \
../main.c 

OBJS += \
./app.o \
./main.o 

C_DEPS += \
./app.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
app.o: ../app.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DBGM220SC22WGA=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/BGM22/Include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/common/util/app_assert" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/common/util/app_log" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//protocol/bluetooth/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/bootloader" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/bootloader/api" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/button/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_cryptoacc_library/include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/common/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/host/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/gpiointerrupt/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/hfxo_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/iostream/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/leddrv/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_mbedtls_support/config" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/mbedtls/include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/mbedtls/library" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_mbedtls_support/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/mpu/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/nvm3/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/bluetooth/common/ota_dfu" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/power_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_psa_driver/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/pwm/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/common" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/ble" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/ieee802154" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/zwave" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/plugin/pa-conversions" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/plugin/pa-conversions/efr32xg22" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/se_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/se_manager/src" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/silicon_labs/silabs_core/memory_manager" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_protocol_crypto/src" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/udelay/inc" -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain/autogen" -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain/config" -Os -Wall -Wextra -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fmessage-length=0 -MMD -MP -MF"app.d" -MT"app.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DBGM220SC22WGA=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/BGM22/Include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/common/util/app_assert" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/common/util/app_log" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//protocol/bluetooth/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/bootloader" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/bootloader/api" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/button/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_cryptoacc_library/include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/common/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/host/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/gpiointerrupt/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/hfxo_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/iostream/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/leddrv/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_mbedtls_support/config" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/mbedtls/include" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/mbedtls/library" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_mbedtls_support/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/mpu/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/nvm3/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//app/bluetooth/common/ota_dfu" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/power_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_psa_driver/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/driver/pwm/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/common" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/ble" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/ieee802154" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/protocol/zwave" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/plugin/pa-conversions" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/radio/rail_lib/plugin/pa-conversions/efr32xg22" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/se_manager/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/se_manager/src" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/silicon_labs/silabs_core/memory_manager" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//util/third_party/crypto/sl_component/sl_protocol_crypto/src" -I"/Users/admin/SimplicityStudio/SDKs/gecko_sdk//platform/service/udelay/inc" -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain/autogen" -I"/Users/admin/SimplicityStudio/v5_workspace/soc_DorceaProMain/config" -Os -Wall -Wextra -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -c -fmessage-length=0 -MMD -MP -MF"main.d" -MT"main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


