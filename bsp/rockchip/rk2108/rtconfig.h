#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

#define RKMCU_RK2108
#define PM_HAS_CUSTOM_CONFIG

/* RT-Thread Kernel */

#define RT_USING_CORE_RTTHREAD
/* RT_USING_CORE_FREERTOS is not set */
#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
/* RT_THREAD_PRIORITY_8 is not set */
#define RT_THREAD_PRIORITY_32
/* RT_THREAD_PRIORITY_256 is not set */
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDEL_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 512
/* RT_USING_TIMER_SOFT is not set */
#define RT_DEBUG
/* RT_DEBUG_COLOR is not set */
/* RT_DEBUG_INIT_CONFIG is not set */
#define RT_DEBUG_USING_IO
/* RT_DEBUG_USING_GPIO is not set */
/* RT_DEBUG_THREAD_CONFIG is not set */
/* RT_DEBUG_SCHEDULER_CONFIG is not set */
/* RT_DEBUG_IPC_CONFIG is not set */
/* RT_DEBUG_TIMER_CONFIG is not set */
/* RT_DEBUG_IRQ_CONFIG is not set */
/* RT_DEBUG_MEM_CONFIG is not set */
/* RT_DEBUG_SLAB_CONFIG is not set */
/* RT_DEBUG_MEMHEAP_CONFIG is not set */
/* RT_DEBUG_MODULE_CONFIG is not set */

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE
/* RT_USING_SIGNALS is not set */

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
/* RT_USING_NOHEAP is not set */
#define RT_USING_SMALL_MEM
/* RT_USING_SLAB is not set */
/* RT_USING_MEMHEAP_AS_HEAP is not set */
/* RT_USING_MEMTRACE is not set */
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_DEVICE_OPS
/* RT_USING_INTERRUPT_INFO is not set */
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart0"
#define RT_VER_NUM 0x30103
#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4
/* ARCH_CPU_STACK_GROWS_UPWARD is not set */

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
/* RT_USING_USER_MAIN is not set */

/* C++ features */

/* RT_USING_CPLUSPLUS is not set */

/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
/* FINSH_ECHO_DISABLE_DEFAULT is not set */
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
/* FINSH_USING_AUTH is not set */
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_USING_MSH_ONLY
#define FINSH_ARG_MAX 20

/* Device virtual file system */

#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define DFS_FILESYSTEMS_MAX 4
#define DFS_FILESYSTEM_TYPES_MAX 2
#define DFS_FD_MAX 16
#define RT_USING_DFS_MNTTABLE
#define RT_USING_DFS_ELMFAT

/* elm-chan's FatFs, Generic FAT Filesystem Module */

#define RT_DFS_ELM_CODE_PAGE 437
#define RT_DFS_ELM_WORD_ACCESS
/* RT_DFS_ELM_USE_LFN_0 is not set */
/* RT_DFS_ELM_USE_LFN_1 is not set */
/* RT_DFS_ELM_USE_LFN_2 is not set */
#define RT_DFS_ELM_USE_LFN_3
#define RT_DFS_ELM_USE_LFN 3
#define RT_DFS_ELM_LFN_UNICODE_0
/* RT_DFS_ELM_LFN_UNICODE_1 is not set */
/* RT_DFS_ELM_LFN_UNICODE_2 is not set */
/* RT_DFS_ELM_LFN_UNICODE_3 is not set */
#define RT_DFS_ELM_LFN_UNICODE 0
#define RT_DFS_ELM_MAX_LFN 255
#define RT_DFS_ELM_DRIVES 2
#define RT_DFS_ELM_MAX_SECTOR_SIZE 4096
/* RT_DFS_ELM_USE_ERASE is not set */
#define RT_DFS_ELM_REENTRANT
/* RT_DFS_ELM_ENABLE_DISK_BUFFER is not set */
#define RT_USING_DFS_DEVFS
/* RT_USING_DFS_ROMFS is not set */
/* RT_USING_DFS_RAMFS is not set */
/* RT_USING_DFS_UFFS is not set */
/* RT_USING_DFS_JFFS2 is not set */
/* RT_USING_LITTLEFS is not set */

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
/* RT_USING_CAN is not set */
/* RT_USING_HWTIMER is not set */
/* RT_USING_CPUTIME is not set */
#define RT_USING_I2C
/* RT_USING_I2C_BITOPS is not set */
#define RT_USING_PIN
/* RT_USING_ADC is not set */
#define RT_USING_PWM
#define RT_USING_MTD_NOR
/* RT_USING_MTD_NAND is not set */
/* RT_USING_MTD is not set */
#define RT_USING_PM
/* RT_USING_PM_FG is not set */
/* RT_USING_PM_CHARGER is not set */
/* RT_USING_RTC is not set */
#define RT_USING_SDIO
#define RT_SDIO_STACK_SIZE 512
#define RT_SDIO_THREAD_PRIORITY 15
#define RT_MMCSD_STACK_SIZE 1536
#define RT_MMCSD_THREAD_PREORITY 22
#define RT_MMCSD_MAX_PARTITION 16
#define RT_SDCARD_MOUNT_POINT "/mnt/sdcard"
#define RT_SDIO_DEBUG
#define RT_USING_SPI
/* RT_USING_QSPI is not set */
/* RT_USING_SPI_MSD is not set */
/* RT_USING_SFUD is not set */
#define RT_USING_W25QXX
/* RT_USING_GD is not set */
/* RT_USING_ENC28J60 is not set */
/* RT_USING_SPI_WIFI is not set */
/* RT_USING_WDT is not set */
#define RT_USING_AUDIO
#define RT_USING_HL_AUDIO
/* RT_USING_SENSOR is not set */
#define RT_USING_TOUCH

/* Using WiFi */

/* RT_USING_WIFI is not set */

/* Using USB */

/* RT_USING_USB_HOST is not set */
/* RT_USING_USB_DEVICE is not set */

/* POSIX layer and C standard library */

#define RT_USING_LIBC
/* RT_USING_PTHREADS is not set */
#define RT_USING_POSIX
/* RT_USING_POSIX_MMAP is not set */
/* RT_USING_POSIX_TERMIOS is not set */
/* RT_USING_POSIX_AIO is not set */
/* RT_USING_MODULE is not set */

/* Network */

/* Socket abstraction layer */

/* RT_USING_SAL is not set */

/* light weight TCP/IP stack */

/* RT_USING_LWIP is not set */

/* Modbus master and slave stack */

/* RT_USING_MODBUS is not set */

/* AT commands */

/* RT_USING_AT is not set */

/* System */

#define RT_USING_LITTLEVGL2RTT

/* LVGL configuration */

/* LV_CONF_SKIP is not set */
/* LV_CONF_MINIMAL is not set */

/* Color settings */

/* LV_COLOR_DEPTH_32 is not set */
#define LV_COLOR_DEPTH_16
/* LV_COLOR_DEPTH_8 is not set */
/* LV_COLOR_DEPTH_1 is not set */
#define LV_COLOR_DEPTH 16
/* LV_COLOR_16_SWAP is not set */
/* LV_COLOR_SCREEN_TRANSP is not set */
#define LV_COLOR_MIX_ROUND_OFS 128
#define LV_COLOR_CHROMA_KEY_HEX 0x00FF00

/* Memory settings */

/* LV_MEM_CUSTOM is not set */
#define LV_MEM_SIZE_KILOBYTES 32
#define LV_MEM_ADDR 0x0
#define LV_MEM_BUF_MAX_NUM 16
/* LV_MEMCPY_MEMSET_STD is not set */

/* HAL Settings */

#define LV_DISP_DEF_REFR_PERIOD 30
#define LV_INDEV_DEF_READ_PERIOD 30
/* LV_TICK_CUSTOM is not set */
#define LV_DPI_DEF 130

/* Feature configuration */

/* Drawing */

#define LV_DRAW_COMPLEX
#define LV_SHADOW_CACHE_SIZE 0
#define LV_CIRCLE_CACHE_SIZE 4
#define LV_LAYER_SIMPLE_BUF_SIZE 24576
#define LV_IMG_CACHE_DEF_SIZE 0
#define LV_GRADIENT_MAX_STOPS 2
#define LV_GRAD_CACHE_DEF_SIZE 0
/* LV_DITHER_GRADIENT is not set */
#define LV_DISP_ROT_MAX_BUF 10240

/* GPU */

/* LV_USE_GPU_ARM2D is not set */
/* LV_USE_GPU_STM32_DMA2D is not set */
/* LV_USE_GPU_SWM341_DMA2D is not set */
/* LV_USE_GPU_NXP_PXP is not set */
/* LV_USE_GPU_NXP_VG_LITE is not set */
/* LV_USE_GPU_SDL is not set */

/* Logging */

/* LV_USE_LOG is not set */

/* Asserts */

#define LV_USE_ASSERT_NULL
#define LV_USE_ASSERT_MALLOC
/* LV_USE_ASSERT_STYLE is not set */
/* LV_USE_ASSERT_MEM_INTEGRITY is not set */
/* LV_USE_ASSERT_OBJ is not set */
#define LV_ASSERT_HANDLER_INCLUDE "assert.h"

/* Others */

/* LV_USE_PERF_MONITOR is not set */
/* LV_USE_MEM_MONITOR is not set */
/* LV_USE_REFR_DEBUG is not set */
/* LV_SPRINTF_CUSTOM is not set */
/* LV_SPRINTF_USE_FLOAT is not set */
#define LV_USE_USER_DATA
/* LV_ENABLE_GC is not set */

/* Compiler settings */

/* LV_BIG_ENDIAN_SYSTEM is not set */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1
/* LV_ATTRIBUTE_FAST_MEM_USE_IRAM is not set */
/* LV_USE_LARGE_COORD is not set */

/* Font usage */

/* Enable built-in fonts */

/* LV_FONT_MONTSERRAT_8 is not set */
/* LV_FONT_MONTSERRAT_10 is not set */
/* LV_FONT_MONTSERRAT_12 is not set */
#define LV_FONT_MONTSERRAT_14
/* LV_FONT_MONTSERRAT_16 is not set */
/* LV_FONT_MONTSERRAT_18 is not set */
/* LV_FONT_MONTSERRAT_20 is not set */
/* LV_FONT_MONTSERRAT_22 is not set */
/* LV_FONT_MONTSERRAT_24 is not set */
/* LV_FONT_MONTSERRAT_26 is not set */
/* LV_FONT_MONTSERRAT_28 is not set */
/* LV_FONT_MONTSERRAT_30 is not set */
/* LV_FONT_MONTSERRAT_32 is not set */
/* LV_FONT_MONTSERRAT_34 is not set */
/* LV_FONT_MONTSERRAT_36 is not set */
/* LV_FONT_MONTSERRAT_38 is not set */
/* LV_FONT_MONTSERRAT_40 is not set */
/* LV_FONT_MONTSERRAT_42 is not set */
/* LV_FONT_MONTSERRAT_44 is not set */
/* LV_FONT_MONTSERRAT_46 is not set */
/* LV_FONT_MONTSERRAT_48 is not set */
/* LV_FONT_MONTSERRAT_12_SUBPX is not set */
/* LV_FONT_MONTSERRAT_28_COMPRESSED is not set */
/* LV_FONT_DEJAVU_16_PERSIAN_HEBREW is not set */
/* LV_FONT_SIMSUN_16_CJK is not set */
/* LV_FONT_UNSCII_8 is not set */
/* LV_FONT_UNSCII_16 is not set */
/* LV_FONT_CUSTOM is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_8 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_12 is not set */
#define LV_FONT_DEFAULT_MONTSERRAT_14
/* LV_FONT_DEFAULT_MONTSERRAT_16 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_18 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_20 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_22 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_24 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_26 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_28 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_30 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_32 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_34 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_36 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_38 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_40 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_42 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_44 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_46 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_48 is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_12_SUBPX is not set */
/* LV_FONT_DEFAULT_MONTSERRAT_28_COMPRESSED is not set */
/* LV_FONT_DEFAULT_DEJAVU_16_PERSIAN_HEBREW is not set */
/* LV_FONT_DEFAULT_SIMSUN_16_CJK is not set */
/* LV_FONT_DEFAULT_UNSCII_8 is not set */
/* LV_FONT_DEFAULT_UNSCII_16 is not set */
/* LV_FONT_FMT_TXT_LARGE is not set */
/* LV_USE_FONT_COMPRESSED is not set */
/* LV_USE_FONT_SUBPX is not set */
#define LV_USE_FONT_PLACEHOLDER

/* Text Settings */

#define LV_TXT_ENC_UTF8
/* LV_TXT_ENC_ASCII is not set */
#define LV_TXT_BREAK_CHARS " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_COLOR_CMD "#"
/* LV_USE_BIDI is not set */
/* LV_USE_ARABIC_PERSIAN_CHARS is not set */

/* Widget usage */

#define LV_USE_ARC
#define LV_USE_BAR
#define LV_USE_BTN
#define LV_USE_BTNMATRIX
#define LV_USE_CANVAS
#define LV_USE_CHECKBOX
#define LV_USE_DROPDOWN
#define LV_USE_IMG
#define LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION
#define LV_LABEL_LONG_TXT_HINT
#define LV_USE_LINE
#define LV_USE_ROLLER
#define LV_ROLLER_INF_PAGES 7
#define LV_USE_SLIDER
#define LV_USE_SWITCH
#define LV_USE_TEXTAREA
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500
#define LV_USE_TABLE

/* Extra Widgets */

#define LV_USE_ANIMIMG
#define LV_USE_CALENDAR
/* LV_CALENDAR_WEEK_STARTS_MONDAY is not set */
#define LV_USE_CALENDAR_HEADER_ARROW
#define LV_USE_CALENDAR_HEADER_DROPDOWN
#define LV_USE_CHART
#define LV_USE_COLORWHEEL
#define LV_USE_IMGBTN
#define LV_USE_KEYBOARD
#define LV_USE_LED
#define LV_USE_LIST
#define LV_USE_MENU
#define LV_USE_METER
#define LV_USE_MSGBOX
#define LV_USE_SPAN
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#define LV_USE_SPINBOX
#define LV_USE_SPINNER
#define LV_USE_TABVIEW
#define LV_USE_TILEVIEW
#define LV_USE_WIN

/* Themes */

#define LV_USE_THEME_DEFAULT
/* LV_THEME_DEFAULT_DARK is not set */
#define LV_THEME_DEFAULT_GROW
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#define LV_USE_THEME_BASIC
/* LV_USE_THEME_MONO is not set */

/* Layouts */

#define LV_USE_FLEX
#define LV_USE_GRID

/* 3rd Party Libraries */

/* LV_USE_FS_STDIO is not set */
/* LV_USE_FS_POSIX is not set */
/* LV_USE_FS_WIN32 is not set */
/* LV_USE_FS_FATFS is not set */
/* LV_USE_PNG is not set */
/* LV_USE_BMP is not set */
/* LV_USE_SJPG is not set */
/* LV_USE_GIF is not set */
/* LV_USE_QRCODE is not set */
/* LV_USE_FREETYPE is not set */
/* LV_USE_RLOTTIE is not set */
/* LV_USE_FFMPEG is not set */

/* Others */

#define LV_USE_SNAPSHOT
/* LV_USE_MONKEY is not set */
/* LV_USE_GRIDNAV is not set */
/* LV_USE_FRAGMENT is not set */
/* LV_USE_IMGFONT is not set */
/* LV_USE_MSG is not set */
/* LV_USE_IME_PINYIN is not set */

/* Examples */

#define LV_BUILD_EXAMPLES

/* Demos */

/* LV_USE_DEMO_WIDGETS is not set */
/* LV_USE_DEMO_KEYPAD_AND_ENCODER is not set */
/* LV_USE_DEMO_BENCHMARK is not set */
/* LV_USE_DEMO_STRESS is not set */
/* LV_USE_DEMO_MUSIC is not set */

/* VBUS(Virtual Software BUS) */

/* RT_USING_VBUS is not set */

/* Utilities */

#define RT_USING_CMBACKTRACE
/* PKG_CMBACKTRACE_PLATFORM_M0_M0PLUS is not set */
/* PKG_CMBACKTRACE_PLATFORM_M3 is not set */
#define PKG_CMBACKTRACE_PLATFORM_M4
/* PKG_CMBACKTRACE_PLATFORM_M7 is not set */
/* PKG_CMBACKTRACE_PLATFORM_NOT_SELECTED is not set */
#define PKG_CMBACKTRACE_DUMP_STACK
#define PKG_CMBACKTRACE_PRINT_ENGLISH
/* PKG_CMBACKTRACE_PRINT_CHINESE is not set */
/* PKG_CMBACKTRACE_FAULT_REBOOT is not set */
/* PKG_CMBACKTRACE_FAULT_DUMP_TO_FLASH is not set */
/* RT_USING_LOGTRACE is not set */
/* RT_USING_RYM is not set */
/* RT_USING_RZM is not set */
/* RT_USING_ULOG is not set */
/* RT_USING_UTEST is not set */

/* ARM CMSIS */

/* RT_USING_CMSIS_OS is not set */
/* RT_USING_RTT_CMSIS is not set */

/* RT-Thread Benchmarks */

/* RT_USING_BENCHMARK is not set */
/* RT_USING_NET_HTTP is not set */
/* RT_USING_OTA is not set */

/* RT-Thread third party package */

/* PKG_CMSIS_RTOS2 is not set */
/* RT_USING_WLAN_WICED is not set */
/* RT_USING_NETUTILS is not set */

/* Bluetooth */

/* BT_ENABLED is not set */

/* examples bluetooth */

/* Bluetooth examlpes */

/* Example 'BT API TEST' Config */

/* BT_API_TEST is not set */

/* Example 'BT DISCOVERY' Config */

/* BT_DISCOVERY_EXAMPLES is not set */

/* Example 'A2DP SINK' Config */

/* A2DP_SINK_EXAMPLE is not set */

/* Example 'A2DP SOURCE' Config  */

/* A2DP_SOURCE_EXAMPLE is not set */

/* Example 'HFP CLIENT' Config */

/* HFP_CLIENT_EXAMPLE is not set */
/* RT_USING_CYPRESS is not set */
#define RT_USING_AUDIO_SERVER
#define AUDIO_USING_STATIC_LIBRARY
/* AUDIO_USING_SOURCE_CODE is not set */
/* RT_USING_EXT_CODEC is not set */
/* AUDIO_ENABLE_PLAYER_TEST is not set */
/* AUDIO_ENABLE_RECORDER_TEST is not set */
/* RT_USING_PKGS is not set */

/* RT-Thread board config */

#define RT_BOARD_NAME "audio_hollyland_a6902_tx"
#define RK_SRAM_END 0x20100000
/* M4_JTAG_ENABLE is not set */
/* DSP_JTAG_ENABLE is not set */

/* RT-Thread rockchip common drivers */

/* HAL_DBG_ON is not set */
/* RT_USING_SND_GLB_RST is not set */
#define RT_USING_RESET
/* RT_USING_CRU_DUMP is not set */
/* RT_USING_REGULATOR_DUMP is not set */
/* MMC_FORCE_INTERNAL_BUF is not set */
/* RT_USING_PROF_IRQ is not set */
/* RT_USING_PMIC is not set */
/* RT_USING_PWM_REGULATOR is not set */
#define RT_USING_CACHE
#define RT_USING_UNCACHE_HEAP
#define RT_UNCACHE_HEAP_ORDER 0x0F
#define RT_USING_LARGE_HEAP
#define RT_LARGE_MALLOC_THRRESH 512
#define RT_LARGE_HEAP_SIZE 524288
/* RT_USING_DTCM_HEAP is not set */
/* RT_USING_PSRAM_HEAP is not set */
#define RT_USING_PANEL_FPGA_LVDS
/* RT_USING_PANEL_SS is not set */
/* RT_USING_PANEL_S6E3FC2X01 is not set */
/* RT_USING_PANEL_S6E3HC2_X1 is not set */
/* RT_USING_PANEL_S6E3HC2_X4 is not set */
/* RT_USING_PANEL_ST7703_DS is not set */
/* RT_USING_PANEL_ST7796H2 is not set */
/* RT_USING_PANEL_AM018RT90211 is not set */
/* RT_USING_PANEL_AM014RT90327V0 is not set */
/* RT_USING_PANEL_H245QBN02 is not set */
/* RT_USING_BACKLIGHT is not set */

/* Enable Fault Dump Hook */

/* RT_USNING_FAULT_DUMP_HOOK is not set */
#define RT_USING_SNOR
#define RT_SNOR_SPEED 80000000
#define RT_SNOR_DUAL_IO
#define RT_USING_SNOR_FSPI_HOST
/* RT_USING_SNOR_SFC_HOST is not set */
/* RT_USING_SNOR_SPI_HOST is not set */

/* RT-Thread rockchip jpeg enc driver */

/* RT_USING_JPEG_ENC is not set */

/* RT-Thread rockchip pm drivers */

#define RT_USING_PM_DVFS
#define RT_USING_PM_REQ_PWR
/* RT_USING_PM_REQ_CLK is not set */
/* RT_USING_DVFS_DBG is not set */
/* RT_USING_PM_RUNTIME is not set */
/* RT_USING_PM_TIMER is not set */

/* RT-Thread rockchip mipi-dphy driver */

/* RT_USING_MIPI_DPHY is not set */

/* RT-Thread rockchip isp driver */

/* RT_USING_ISP is not set */

/* RT-Thread rockchip vcm driver */

/* RT_USING_VCM is not set */

/* RT-Thread rockchip vicap driver */

/* RT_USING_VICAP is not set */

/* RT-Thread rockchip camera driver */

/* RT_USING_CAMERA is not set */

/* RT-Thread rockchip vicap_lite driver */

/* RT_USING_VICAP_LITE is not set */

/* RT-Thread rockchip csi2host driver */

/* RT_USING_CSI2HOST is not set */

/* RT-Thread rockchip buffer_manage driver */

/* RT_USING_BUFFER_MANAGE is not set */
/* RT_USING_TOUCH_DRIVERS is not set */
/* RT_USING_SENSOR_HUB is not set */
/* RT_USING_QPIPSRAM is not set */

/* RT-Thread rockchip coredump driver */

/* RT_USING_COREDUMP is not set */
/* RT_USING_SPINAND is not set */

/* Enable PSTORE */

/* RT_USING_PSTORE is not set */

/* RT-Thread rockchip rk2108 drivers */

/* Enable Audio */

#define RT_USING_CODEC
#define RT_USING_CODEC_NAU88L25B
#define RT_USING_I2STDM
#define RT_USING_I2STDM1
/* RT_USING_DRIVER_AUDIO_PCM_PLUGIN is not set */

/* Audio Cards */

#define RT_USING_AUDIO_CARD
/* RT_USING_AUDIO_CARD_ACDCDIG is not set */
/* RT_USING_AUDIO_CARD_AUDIOPWM is not set */
/* RT_USING_AUDIO_CARD_I2S_MIC is not set */
/* RT_USING_AUDIO_CARD_PDM_MIC is not set */
/* RT_USING_AUDIO_CARD_PCM_BT is not set */
/* RT_USING_AUDIO_CARD_AW8896 is not set */
/* RT_USING_AUDIO_CARD_ES7243 is not set */
/* RT_USING_AUDIO_CARD_ES8156 is not set */
/* RT_USING_AUDIO_CARD_ES8311 is not set */
/* RT_USING_AUDIO_CARD_ES8388 is not set */
/* RT_USING_AUDIO_CARD_ADC_ES8311_ECHO is not set */
/* RT_USING_AUDIO_CARD_PDM_ES8388_ECHO is not set */
#define RT_USING_AUDIO_CARD_NAU88L25B

/* Audio Interfaces */

#define RT_USING_PCM
/* RT_USING_VAD is not set */
#define RT_USING_CRU
#define RT_USING_DMA
#define RT_USING_DMA_PL330
#define RT_USING_PMU
#define RT_USING_DISPLAY
#define RT_USING_VOP
/* RT_USING_SPI_SCREEN is not set */
/* RT_USING_VOP_MCU is not set */
#define RT_USING_DSI
/* RT_USING_KEYCTRL is not set */
#define RT_USING_PWM0

/* Enable DSP */

#define RT_USING_DSP
#define RT_USING_DSPFW_LOADER
#define RT_USING_DSPFW_BUILTIN
/* RT_USING_DSPFW_FILE is not set */
#define RT_DSPFW_FILE_NAME "rkdsp_fw.c"
/* RT_USING_DSP_TRACE is not set */
#define RT_DSP_UART_PORT 0

/* Enable UART */

#define RT_USING_UART
#define RT_USING_UART0
#define RT_USING_UART1
#define RT_USING_UART2

/* Enable I2C */

#define RT_USING_I2C0
#define RT_USING_I2C1
#define RT_USING_I2C2

/* Enable SPI */

#define RT_USING_SPI2APB
#define RT_USING_SPI1
#define RT_USING_SPI2

/* Enable SDIO/SDMMC/eMMC */

#define RT_USING_SDIO0
/* RT_USING_SYSTICK is not set */

/* RT-Thread application */

/* RT_USING_LVGL_APP_DEMO is not set */
/* RT_USING_ROCKCHIP_DEMO is not set */
/* RT_USING_RK_IOT_APP is not set */
/* ENABLE_DUAL_TRACKING is not set */

/* RT-Thread bsp test case */

/* RT_USING_PRIVATE_TEST is not set */

/* RT-Thread Common Test case */

/* RT_USING_TC is not set */
/* RT_USING_FILE_TEST is not set */
#define RT_USING_COMMON_TEST
/* RT_USING_AUTO_TEST is not set */
/* RT_USING_COMMON_TEST_SUSPEND is not set */
#define RT_USING_COMMON_TEST_AUDIO
/* RT_USING_COMMON_TEST_CODEC_ES8388 is not set */
/* RT_USING_COMMON_TEST_CPU_USAGE is not set */
/* RT_USING_COMMON_TEST_DCACHE is not set */
/* RT_USING_COMMON_TEST_ICACHE is not set */
/* RT_USING_COMMON_TEST_DSP is not set */
/* RT_USING_DSP_OPUS_CODEC_TEST is not set */
/* RT_USING_COMMON_TEST_GPIO is not set */
/* RT_USING_COMMON_TEST_I2C is not set */
/* RT_USING_COMMON_TEST_INTC is not set */
/* RT_USING_COMMON_TEST_MBOX is not set */
/* RT_USING_COMMON_MEMTESTER is not set */
/* RT_USING_COMMON_TEST_MPU is not set */
/* RT_USING_COMMON_TEST_PM is not set */
/* RT_USING_COMMON_TEST_SNOR is not set */
/* RT_USING_COMMON_TEST_SPI is not set */
/* RT_USING_COMMON_TEST_SPI2APB is not set */
/* RT_USING_COMMON_STRESS_TEST is not set */
/* RT_USING_COMMON_TEST_PERFORMANCE is not set */
/* RT_USING_COMMON_TEST_FWANALYSIS is not set */
/* RT_USING_COMMON_TEST_TIMER is not set */
#define RT_USING_FWANALYSIS

#endif
