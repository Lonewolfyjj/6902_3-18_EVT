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
#define RT_DFS_ELM_DRIVES 3
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
#define RT_USING_RTC
#define RT_USING_SOFT_RTC
/* RT_USING_ALARM is not set */
/* RT_USING_SDIO is not set */
#define RT_USING_SPI
/* RT_USING_QSPI is not set */
/* RT_USING_SPI_MSD is not set */
/* RT_USING_SFUD is not set */
#define RT_USING_W25QXX
/* RT_USING_GD is not set */
/* RT_USING_ENC28J60 is not set */
/* RT_USING_SPI_WIFI is not set */
#define RT_USING_WDT
#define RT_USING_AUDIO
/* RT_USING_HL_AUDIO is not set */
/* RT_USING_SENSOR is not set */
#define RT_USING_TOUCH

/* Using WiFi */

/* RT_USING_WIFI is not set */

/* Using USB */

/* RT_USING_USB_HOST is not set */
#define RT_USING_USB_DEVICE
#define RT_USBD_THREAD_STACK_SZ 4096
#define USB_VENDOR_ID 0x3547
#define USB_PRODUCT_ID 0x0002
#define RT_USB_DEVICE_COMPOSITE
/* RT_USB_DEVICE_CDC is not set */
#define RT_USB_DEVICE_NONE
#define RT_USB_DEVICE_MSTORAGE
#define RT_USB_DEVICE_HID
#define RT_USB_DEVICE_WINUSB
#define RT_USB_DEVICE_UAC1
/* RT_USB_DEVICE_UVC is not set */
#define RT_WINUSB_GUID "{6860DC3C-C05F-4807-8807-1CA861CC1D66}"
#define RT_USB_MSTORAGE_DISK_NAME "root"
#define RT_USB_MSTORAGE_BUFLEN 4096
#define RT_USB_AUDIO_P_NAME "codecp"
#define RT_USB_AUDIO_C_NAME "pdmc"
/* RT_USB_AUDIO_PLL_COMPENSATION is not set */
/* RT_USB_DEVICE_HID_KEYBOARD is not set */
/* RT_USB_DEVICE_HID_MOUSE is not set */
#define RT_USB_DEVICE_HID_GENERAL
#define RT_USB_DEVICE_HID_GENERAL_OUT_REPORT_LENGTH 63
#define RT_USB_DEVICE_HID_GENERAL_IN_REPORT_LENGTH 63
#define RT_USB_DEVICE_HID_MEDIA

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
#define RT_USING_LITTLEVGL2RTT
/* RT_USING_LITTLEVGL2RTT_V7 is not set */
#define RT_USING_LITTLEVGL2RTT_V8

/* LittlevGL2RTT Component Options */

/* LV memory management config */

/* RT_LV_MEM_STATIC is not set */
#define RT_LV_MEM_DYNAMIC
#define RT_LV_MEM_CUSTOM 1

/* LV graphical config */

/* RT_LV_COLOR_DEPTH_1 is not set */
/* RT_LV_COLOR_DEPTH_8 is not set */
#define RT_LV_COLOR_DEPTH_16
/* RT_LV_COLOR_DEPTH_24 is not set */
#define RT_LV_COLOR_DEPTH 16
#define RT_LV_HOR_RES 126
#define RT_LV_VER_RES 294
#define RT_LV_DPI 160
/* RT_LV_COLOR_16_SWAP is not set */
/* RT_LV_COLOR_SCREEN_TRANSP is not set */
#define RT_LV_INDEXED_CHROMA
#define RT_LV_ANTIALIAS
#define RT_LV_DISP_DEF_REFR_PERIOD 5
#define RT_LV_DISP_SMALL_LIMIT 30
#define RT_LV_DISP_MEDIUM_LIMIT 50
#define RT_LV_DISP_LARGE_LIMIT 70

/* LV Input device config */

#define RT_LV_INDEV_DEF_READ_PERIOD 10
#define RT_LV_INDEV_DEF_DRAG_LIMIT 10
#define RT_LV_INDEV_DEF_DRAG_THROW 10
#define RT_LV_INDEV_DEF_LONG_PRESS_TIME 400
#define RT_LV_INDEV_DEF_LONG_PRESS_REP_TIME 100
#define RT_LV_INDEV_DEF_GESTURE_LIMIT 50
#define RT_LV_INDEV_DEF_GESTURE_MIN_VELOCITY 3

/* LV Feature config */

#define RT_LV_USE_ANIMATION
#define RT_LV_USE_SHADOW
#define RT_LV_USE_BLEND_MODES
#define RT_LV_USE_OPA_SCALE
#define RT_LV_USE_IMG_TRANSFORM
#define RT_LV_USE_GROUP
/* RT_LV_USE_GPU is not set */
/* RT_LV_USE_FILESYSTEM is not set */
/* RT_LV_USE_USER_DATA is not set */
/* RT_LV_USE_PERF_MONITOR is not set */

/* LV OBJECT config */

#define RT_LV_USE_ARC
#define RT_LV_USE_BAR
#define RT_LV_USE_BTN
/* RT_LV_USE_BTNMATRIX is not set */
/* RT_LV_USE_CALENDAR is not set */
/* RT_LV_USE_CANVAS is not set */
/* RT_LV_USE_CHECKBOX is not set */
/* RT_LV_USE_CHART is not set */
/* RT_LV_USE_CONT is not set */
/* RT_LV_USE_CPICKER is not set */
/* RT_LV_USE_DROPDOWN is not set */
/* RT_LV_USE_GAUGE is not set */
/* RT_LV_USE_CLOCK is not set */
#define RT_LV_USE_IMG
/* RT_LV_USE_IMGBTN is not set */
/* RT_LV_USE_KEYBOARD is not set */
#define RT_LV_USE_LABEL
/* RT_LV_USE_LED is not set */
/* RT_LV_USE_LINE is not set */
/* RT_LV_USE_LIST is not set */
/* RT_LV_USE_LINEMETER is not set */
#define RT_LV_USE_OBJMASK
/* RT_LV_USE_MSGBOX is not set */
#define RT_LV_USE_PAGE
/* RT_LV_USE_SPINNER is not set */
#define RT_LV_USE_ROLLER
#define RT_LV_USE_SLIDER
/* RT_LV_USE_SPINBOX is not set */
/* RT_LV_USE_SWITCH is not set */
/* RT_LV_USE_TEXTAREA is not set */
/* RT_LV_USE_TABLE is not set */
/* RT_LV_USE_TABVIEW is not set */
/* RT_LV_USE_TILEVIEW is not set */
/* RT_LV_USE_WIN is not set */

/* LV FONTS config */

/* RT_LV_FONT_MONTSERRAT_8 is not set */
/* RT_LV_FONT_MONTSERRAT_10 is not set */
/* RT_LV_FONT_MONTSERRAT_12 is not set */
/* RT_LV_FONT_MONTSERRAT_14 is not set */
/* RT_LV_FONT_MONTSERRAT_16 is not set */
/* RT_LV_FONT_MONTSERRAT_18 is not set */
/* RT_LV_FONT_MONTSERRAT_20 is not set */
/* RT_LV_FONT_MONTSERRAT_22 is not set */
/* RT_LV_FONT_MONTSERRAT_24 is not set */
/* RT_LV_FONT_MONTSERRAT_26 is not set */
/* RT_LV_FONT_MONTSERRAT_28 is not set */
/* RT_LV_FONT_MONTSERRAT_30 is not set */
/* RT_LV_FONT_MONTSERRAT_32 is not set */
/* RT_LV_FONT_MONTSERRAT_34 is not set */
/* RT_LV_FONT_MONTSERRAT_36 is not set */
/* RT_LV_FONT_MONTSERRAT_38 is not set */
/* RT_LV_FONT_MONTSERRAT_40 is not set */
/* RT_LV_FONT_MONTSERRAT_42 is not set */
/* RT_LV_FONT_MONTSERRAT_44 is not set */
/* RT_LV_FONT_MONTSERRAT_46 is not set */
/* RT_LV_FONT_MONTSERRAT_48 is not set */

/* LV_THEME_DEFAULT VALUEs config */

#define RT_LV_THEME_DEFAULT_COLOR_PRIMARY 0x01a2b1
#define RT_LV_THEME_DEFAULT_COLOR_SECONDARY 0x44d1b6
#define RT_LV_THEME_DEFAULT_FONT_SMALL_NULL
#define RT_LV_THEME_DEFAULT_FONT_NORMAL_NULL
#define RT_LV_THEME_DEFAULT_FONT_SUBTITLE_NULL
#define RT_LV_THEME_DEFAULT_FONT_TITLE_NULL

/* LittlevGL2RTT display buffer Options */

#define RT_LV_DISP_USE_HEAP_BUF
/* RT_LV_DISP_USE_LARGE_BUF is not set */
/* RT_LV_DISP_DOUBLE_BUF is not set */
#define RT_LV_DISP_BUF_LINES 40
/* RT_LV_USING_DEMO is not set */

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
#define RT_USING_OTA

/* OTA Component Options */

/* RT_USING_OTA_RECOVERY is not set */
#define RT_USING_OTA_FROM_LOCAL
/* RT_USING_OTA_FROM_HTTP is not set */
/* RT_SUPPORT_ROOT_AB is not set */
/* RT_USING_AUPIPE is not set */

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
#define RT_USING_PKGS

/* RT-Thread online packages */

/* IoT - internet of things */

/* PKG_USING_LORAWAN_DRIVER is not set */
/* PKG_USING_PAHOMQTT is not set */
/* PKG_USING_UMQTT is not set */
/* PKG_USING_WEBCLIENT is not set */
/* PKG_USING_WEBNET is not set */
/* PKG_USING_MONGOOSE is not set */
/* PKG_USING_MYMQTT is not set */
/* PKG_USING_KAWAII_MQTT is not set */
/* PKG_USING_BC28_MQTT is not set */
/* PKG_USING_WEBTERMINAL is not set */
/* PKG_USING_LIBMODBUS is not set */
/* PKG_USING_FREEMODBUS is not set */
/* PKG_USING_NANOPB is not set */

/* Wi-Fi */

/* Marvell WiFi */

/* PKG_USING_WLANMARVELL is not set */

/* Wiced WiFi */

/* PKG_USING_WLAN_WICED is not set */
/* PKG_USING_RW007 is not set */
/* PKG_USING_COAP is not set */
/* PKG_USING_NOPOLL is not set */
/* PKG_USING_NETUTILS is not set */
/* PKG_USING_CMUX is not set */
/* PKG_USING_PPP_DEVICE is not set */
/* PKG_USING_AT_DEVICE is not set */
/* PKG_USING_ATSRV_SOCKET is not set */
/* PKG_USING_WIZNET is not set */
/* PKG_USING_ZB_COORDINATOR is not set */

/* IoT Cloud */

/* PKG_USING_ONENET is not set */
/* PKG_USING_GAGENT_CLOUD is not set */
/* PKG_USING_ALI_IOTKIT is not set */
/* PKG_USING_AZURE is not set */
/* PKG_USING_TENCENT_IOT_EXPLORER is not set */
/* PKG_USING_JIOT-C-SDK is not set */
/* PKG_USING_UCLOUD_IOT_SDK is not set */
/* PKG_USING_JOYLINK is not set */
/* PKG_USING_EZ_IOT_OS is not set */
/* PKG_USING_IOTSHARP_SDK is not set */
/* PKG_USING_NIMBLE is not set */
/* PKG_USING_LLSYNC_SDK_ADAPTER is not set */
/* PKG_USING_OTA_DOWNLOADER is not set */
/* PKG_USING_IPMSG is not set */
/* PKG_USING_LSSDP is not set */
/* PKG_USING_AIRKISS_OPEN is not set */
/* PKG_USING_LIBRWS is not set */
/* PKG_USING_TCPSERVER is not set */
/* PKG_USING_PROTOBUF_C is not set */
/* PKG_USING_DLT645 is not set */
/* PKG_USING_QXWZ is not set */
/* PKG_USING_SMTP_CLIENT is not set */
/* PKG_USING_ABUP_FOTA is not set */
/* PKG_USING_LIBCURL2RTT is not set */
/* PKG_USING_CAPNP is not set */
/* PKG_USING_AGILE_TELNET is not set */
/* PKG_USING_NMEALIB is not set */
/* PKG_USING_PDULIB is not set */
/* PKG_USING_BTSTACK is not set */
/* PKG_USING_LORAWAN_ED_STACK is not set */
/* PKG_USING_WAYZ_IOTKIT is not set */
/* PKG_USING_MAVLINK is not set */
/* PKG_USING_BSAL is not set */
/* PKG_USING_AGILE_MODBUS is not set */
/* PKG_USING_AGILE_FTP is not set */
/* PKG_USING_EMBEDDEDPROTO is not set */
/* PKG_USING_RT_LINK_HW is not set */
/* PKG_USING_LORA_PKT_FWD is not set */
/* PKG_USING_LORA_GW_DRIVER_LIB is not set */
/* PKG_USING_LORA_PKT_SNIFFER is not set */
/* PKG_USING_HM is not set */
/* PKG_USING_SMALL_MODBUS is not set */
/* PKG_USING_NET_SERVER is not set */
/* PKG_USING_ZFTP is not set */

/* security packages */

/* PKG_USING_MBEDTLS is not set */
/* PKG_USING_LIBSODIUM is not set */
/* PKG_USING_LIBHYDROGEN is not set */
/* PKG_USING_TINYCRYPT is not set */
/* PKG_USING_TFM is not set */
/* PKG_USING_YD_CRYPTO is not set */

/* language packages */

/* JSON: JavaScript Object Notation, a lightweight data-interchange format */

/* PKG_USING_CJSON is not set */
/* PKG_USING_LJSON is not set */
/* PKG_USING_RT_CJSON_TOOLS is not set */
/* PKG_USING_RAPIDJSON is not set */
/* PKG_USING_JSMN is not set */
/* PKG_USING_AGILE_JSMN is not set */
/* PKG_USING_PARSON is not set */

/* XML: Extensible Markup Language */

/* PKG_USING_SIMPLE_XML is not set */
/* PKG_USING_EZXML is not set */
/* PKG_USING_LUATOS_SOC is not set */
/* PKG_USING_LUA is not set */
/* PKG_USING_JERRYSCRIPT is not set */
/* PKG_USING_MICROPYTHON is not set */
/* PKG_USING_PIKASCRIPT is not set */
/* PKG_USING_RTT_RUST is not set */

/* multimedia packages */

/* LVGL: powerful and easy-to-use embedded GUI library */

/* PKG_USING_LVGL is not set */
/* PKG_USING_LITTLEVGL2RTT is not set */
/* PKG_USING_LV_MUSIC_DEMO is not set */
/* PKG_USING_GUI_GUIDER_DEMO is not set */

/* u8g2: a monochrome graphic library */

/* PKG_USING_U8G2 is not set */
/* PKG_USING_OPENMV is not set */
/* PKG_USING_MUPDF is not set */
/* PKG_USING_STEMWIN is not set */
/* PKG_USING_WAVPLAYER is not set */
/* PKG_USING_TJPGD is not set */
/* PKG_USING_PDFGEN is not set */
/* PKG_USING_HELIX is not set */
/* PKG_USING_AZUREGUIX is not set */
/* PKG_USING_TOUCHGFX2RTT is not set */
/* PKG_USING_NUEMWIN is not set */
/* PKG_USING_MP3PLAYER is not set */
/* PKG_USING_TINYJPEG is not set */
/* PKG_USING_UGUI is not set */

/* PainterEngine: A cross-platform graphics application framework written in C language */

/* PKG_USING_PAINTERENGINE is not set */
/* PKG_USING_PAINTERENGINE_AUX is not set */
/* PKG_USING_MCURSES is not set */
/* PKG_USING_VT100 is not set */
/* PKG_USING_QRCODE is not set */
/* PKG_USING_GUIENGINE is not set */
/* PKG_USING_PERSIMMON is not set */

/* tools packages */

/* PKG_USING_CMBACKTRACE is not set */
/* PKG_USING_EASYFLASH is not set */
/* PKG_USING_EASYLOGGER is not set */
/* PKG_USING_SYSTEMVIEW is not set */
/* PKG_USING_SEGGER_RTT is not set */
/* PKG_USING_RDB is not set */
/* PKG_USING_ULOG_EASYFLASH is not set */
/* PKG_USING_ULOG_FILE is not set */
/* PKG_USING_LOGMGR is not set */
/* PKG_USING_ADBD is not set */
/* PKG_USING_COREMARK is not set */
/* PKG_USING_DHRYSTONE is not set */
/* PKG_USING_MEMORYPERF is not set */
/* PKG_USING_NR_MICRO_SHELL is not set */
/* PKG_USING_CHINESE_FONT_LIBRARY is not set */
/* PKG_USING_LUNAR_CALENDAR is not set */
/* PKG_USING_BS8116A is not set */
/* PKG_USING_GPS_RMC is not set */
/* PKG_USING_URLENCODE is not set */
/* PKG_USING_UMCN is not set */
/* PKG_USING_LWRB2RTT is not set */
/* PKG_USING_CPU_USAGE is not set */
/* PKG_USING_GBK2UTF8 is not set */
/* PKG_USING_VCONSOLE is not set */
/* PKG_USING_KDB is not set */
/* PKG_USING_WAMR is not set */
/* PKG_USING_MICRO_XRCE_DDS_CLIENT is not set */
/* PKG_USING_LWLOG is not set */
/* PKG_USING_ANV_TRACE is not set */
/* PKG_USING_ANV_MEMLEAK is not set */
/* PKG_USING_ANV_TESTSUIT is not set */
/* PKG_USING_ANV_BENCH is not set */
/* PKG_USING_DEVMEM is not set */
/* PKG_USING_REGEX is not set */
/* PKG_USING_MEM_SANDBOX is not set */
/* PKG_USING_SOLAR_TERMS is not set */
/* PKG_USING_GAN_ZHI is not set */
/* PKG_USING_FDT is not set */
/* PKG_USING_CBOX is not set */
/* PKG_USING_SNOWFLAKE is not set */
/* PKG_USING_HASH_MATCH is not set */
/* PKG_USING_ARMV7M_DWT_TOOL is not set */
/* PKG_USING_VOFA_PLUS is not set */

/* system packages */

/* enhanced kernel services */

/* PKG_USING_RT_MEMCPY_CM is not set */
/* PKG_USING_RT_KPRINTF_THREADSAFE is not set */
/* PKG_USING_RT_VSNPRINTF_FULL is not set */

/* acceleration: Assembly language or algorithmic acceleration packages */

/* PKG_USING_QFPLIB_M0_FULL is not set */
/* PKG_USING_QFPLIB_M0_TINY is not set */
/* PKG_USING_QFPLIB_M3 is not set */

/* CMSIS: ARM Cortex-M Microcontroller Software Interface Standard */

/* PKG_USING_CMSIS_5 is not set */
/* PKG_USING_CMSIS_RTOS1 is not set */
/* PKG_USING_CMSIS_RTOS2 is not set */

/* Micrium: Micrium software products porting for RT-Thread */

/* PKG_USING_UCOSIII_WRAPPER is not set */
/* PKG_USING_UCOSII_WRAPPER is not set */
/* PKG_USING_UC_CRC is not set */
/* PKG_USING_UC_CLK is not set */
/* PKG_USING_UC_COMMON is not set */
/* PKG_USING_UC_MODBUS is not set */
/* PKG_USING_FREERTOS_WRAPPER is not set */
/* PKG_USING_CAIRO is not set */
/* PKG_USING_PIXMAN is not set */
/* PKG_USING_PARTITION is not set */
/* PKG_USING_PERF_COUNTER is not set */
/* PKG_USING_FAL is not set */
/* PKG_USING_FLASHDB is not set */
/* PKG_USING_SQLITE is not set */
/* PKG_USING_RTI is not set */
/* PKG_USING_DFS_YAFFS is not set */
/* PKG_USING_LITTLEFS is not set */
/* PKG_USING_DFS_JFFS2 is not set */
/* PKG_USING_DFS_UFFS is not set */
/* PKG_USING_LWEXT4 is not set */
/* PKG_USING_THREAD_POOL is not set */
/* PKG_USING_ROBOTS is not set */
/* PKG_USING_EV is not set */
/* PKG_USING_SYSWATCH is not set */
/* PKG_USING_SYS_LOAD_MONITOR is not set */
/* PKG_USING_PLCCORE is not set */
/* PKG_USING_RAMDISK is not set */
/* PKG_USING_MININI is not set */
/* PKG_USING_QBOOT is not set */
/* PKG_USING_PPOOL is not set */
/* PKG_USING_OPENAMP is not set */
/* PKG_USING_LPM is not set */
/* PKG_USING_TLSF is not set */
/* PKG_USING_EVENT_RECORDER is not set */
/* PKG_USING_ARM_2D is not set */
/* PKG_USING_MCUBOOT is not set */
/* PKG_USING_TINYUSB is not set */
/* PKG_USING_CHERRYUSB is not set */
/* PKG_USING_KMULTI_RTIMER is not set */
/* PKG_USING_TFDB is not set */
/* PKG_USING_QPC is not set */
/* PKG_USING_AGILE_UPGRADE is not set */

/* peripheral libraries and drivers */

/* sensors drivers */

/* PKG_USING_FINGERPRINT is not set */
/* PKG_USING_LSM6DSM is not set */
/* PKG_USING_LSM6DSL is not set */
/* PKG_USING_LPS22HB is not set */
/* PKG_USING_HTS221 is not set */
/* PKG_USING_LSM303AGR is not set */
/* PKG_USING_BME280 is not set */
/* PKG_USING_BME680 is not set */
/* PKG_USING_BMA400 is not set */
/* PKG_USING_BMI160_BMX160 is not set */
/* PKG_USING_SPL0601 is not set */
/* PKG_USING_MS5805 is not set */
/* PKG_USING_DA270 is not set */
/* PKG_USING_DF220 is not set */
/* PKG_USING_HSHCAL001 is not set */
/* PKG_USING_BH1750 is not set */
/* PKG_USING_MPU6XXX is not set */
/* PKG_USING_AHT10 is not set */
/* PKG_USING_AP3216C is not set */
/* PKG_USING_TSL4531 is not set */
/* PKG_USING_DS18B20 is not set */
/* PKG_USING_DHT11 is not set */
/* PKG_USING_DHTXX is not set */
/* PKG_USING_GY271 is not set */
/* PKG_USING_GP2Y10 is not set */
/* PKG_USING_SGP30 is not set */
/* PKG_USING_HDC1000 is not set */
/* PKG_USING_BMP180 is not set */
/* PKG_USING_BMP280 is not set */
/* PKG_USING_SHTC1 is not set */
/* PKG_USING_BMI088 is not set */
/* PKG_USING_HMC5883 is not set */
/* PKG_USING_MAX6675 is not set */
/* PKG_USING_TMP1075 is not set */
/* PKG_USING_SR04 is not set */
/* PKG_USING_CCS811 is not set */
/* PKG_USING_PMSXX is not set */
/* PKG_USING_RT3020 is not set */
/* PKG_USING_MLX90632 is not set */
/* PKG_USING_MLX90393 is not set */
/* PKG_USING_MLX90392 is not set */
/* PKG_USING_MLX90397 is not set */
/* PKG_USING_MS5611 is not set */
/* PKG_USING_MAX31865 is not set */
/* PKG_USING_VL53L0X is not set */
/* PKG_USING_INA260 is not set */
/* PKG_USING_MAX30102 is not set */
/* PKG_USING_INA226 is not set */
/* PKG_USING_LIS2DH12 is not set */
/* PKG_USING_HS300X is not set */
/* PKG_USING_ZMOD4410 is not set */
/* PKG_USING_ISL29035 is not set */
/* PKG_USING_MMC3680KJ is not set */
/* PKG_USING_QMP6989 is not set */
/* PKG_USING_BALANCE is not set */
/* PKG_USING_SHT2X is not set */
/* PKG_USING_SHT3X is not set */
/* PKG_USING_AD7746 is not set */
/* PKG_USING_ADT74XX is not set */
/* PKG_USING_MAX17048 is not set */

/* touch drivers */

/* PKG_USING_GT9147 is not set */
/* PKG_USING_GT1151 is not set */
/* PKG_USING_GT917S is not set */
/* PKG_USING_GT911 is not set */
/* PKG_USING_FT6206 is not set */
/* PKG_USING_FT5426 is not set */
/* PKG_USING_FT6236 is not set */
/* PKG_USING_XPT2046_TOUCH is not set */
/* PKG_USING_REALTEK_AMEBA is not set */
/* PKG_USING_AS7341 is not set */
/* PKG_USING_STM32_SDIO is not set */
/* PKG_USING_ESP_IDF is not set */
/* PKG_USING_ICM20608 is not set */
/* PKG_USING_BUTTON is not set */
/* PKG_USING_PCF8574 is not set */
/* PKG_USING_SX12XX is not set */
/* PKG_USING_SIGNAL_LED is not set */
/* PKG_USING_LEDBLINK is not set */
/* PKG_USING_LITTLED is not set */
/* PKG_USING_LKDGUI is not set */
/* PKG_USING_NRF5X_SDK is not set */
/* PKG_USING_NRFX is not set */
/* PKG_USING_WM_LIBRARIES is not set */

/* Kendryte SDK */

/* PKG_USING_K210_SDK is not set */
/* PKG_USING_KENDRYTE_SDK is not set */
/* PKG_USING_INFRARED is not set */
/* PKG_USING_MULTI_INFRARED is not set */
/* PKG_USING_AGILE_BUTTON is not set */
/* PKG_USING_AGILE_LED is not set */
/* PKG_USING_AT24CXX is not set */
/* PKG_USING_MOTIONDRIVER2RTT is not set */
/* PKG_USING_PCA9685 is not set */
/* PKG_USING_I2C_TOOLS is not set */
/* PKG_USING_NRF24L01 is not set */
/* PKG_USING_RPLIDAR is not set */
/* PKG_USING_AS608 is not set */
/* PKG_USING_RC522 is not set */
/* PKG_USING_WS2812B is not set */
/* PKG_USING_EMBARC_BSP is not set */
/* PKG_USING_EXTERN_RTC_DRIVERS is not set */
/* PKG_USING_MULTI_RTIMER is not set */
/* PKG_USING_MAX7219 is not set */
/* PKG_USING_BEEP is not set */
/* PKG_USING_EASYBLINK is not set */
/* PKG_USING_PMS_SERIES is not set */
/* PKG_USING_CAN_YMODEM is not set */
/* PKG_USING_LORA_RADIO_DRIVER is not set */
/* PKG_USING_QLED is not set */
/* PKG_USING_PAJ7620 is not set */
/* PKG_USING_AGILE_CONSOLE is not set */
/* PKG_USING_LD3320 is not set */
/* PKG_USING_WK2124 is not set */
/* PKG_USING_LY68L6400 is not set */
/* PKG_USING_DM9051 is not set */
/* PKG_USING_SSD1306 is not set */
/* PKG_USING_QKEY is not set */
/* PKG_USING_RS485 is not set */
/* PKG_USING_RS232 is not set */
/* PKG_USING_NES is not set */
/* PKG_USING_VIRTUAL_SENSOR is not set */
/* PKG_USING_VDEVICE is not set */
/* PKG_USING_SGM706 is not set */
/* PKG_USING_STM32WB55_SDK is not set */
/* PKG_USING_RDA58XX is not set */
/* PKG_USING_LIBNFC is not set */
/* PKG_USING_MFOC is not set */
/* PKG_USING_TMC51XX is not set */
/* PKG_USING_TCA9534 is not set */
/* PKG_USING_KOBUKI is not set */
/* PKG_USING_ROSSERIAL is not set */
/* PKG_USING_MICRO_ROS is not set */
/* PKG_USING_MCP23008 is not set */
/* PKG_USING_BLUETRUM_SDK is not set */
/* PKG_USING_MISAKA_AT24CXX is not set */
/* PKG_USING_MISAKA_RGB_BLING is not set */
/* PKG_USING_LORA_MODEM_DRIVER is not set */
/* PKG_USING_BL_MCU_SDK is not set */
/* PKG_USING_SOFT_SERIAL is not set */
/* PKG_USING_MB85RS16 is not set */
/* PKG_USING_CW2015 is not set */
/* PKG_USING_RFM300 is not set */
/* PKG_USING_IO_INPUT_FILTER is not set */
/* PKG_USING_RASPBERRYPI_PICO_SDK is not set */
/* PKG_USING_LRF_NV7LIDAR is not set */

/* AI packages */

/* PKG_USING_LIBANN is not set */
/* PKG_USING_NNOM is not set */
/* PKG_USING_ONNX_BACKEND is not set */
/* PKG_USING_ONNX_PARSER is not set */
/* PKG_USING_TENSORFLOWLITEMICRO is not set */
/* PKG_USING_ELAPACK is not set */
/* PKG_USING_ULAPACK is not set */
/* PKG_USING_QUEST is not set */
/* PKG_USING_NAXOS is not set */

/* Signal Processing and Control Algorithm Packages */

/* PKG_USING_FIRE_PID_CURVE is not set */
/* PKG_USING_UKAL is not set */

/* miscellaneous packages */

/* project laboratory */

/* samples: kernel and components samples */

/* PKG_USING_KERNEL_SAMPLES is not set */
/* PKG_USING_FILESYSTEM_SAMPLES is not set */
/* PKG_USING_NETWORK_SAMPLES is not set */
/* PKG_USING_PERIPHERAL_SAMPLES is not set */

/* entertainment: terminal games and other interesting software packages */

/* PKG_USING_CMATRIX is not set */
/* PKG_USING_SL is not set */
/* PKG_USING_CAL is not set */
/* PKG_USING_ACLOCK is not set */
/* PKG_USING_THREES is not set */
/* PKG_USING_2048 is not set */
/* PKG_USING_SNAKE is not set */
/* PKG_USING_TETRIS is not set */
/* PKG_USING_DONUT is not set */
/* PKG_USING_COWSAY is not set */
/* PKG_USING_LIBCSV is not set */
/* PKG_USING_OPTPARSE is not set */
/* PKG_USING_FASTLZ is not set */
/* PKG_USING_MINILZO is not set */
/* PKG_USING_QUICKLZ is not set */
/* PKG_USING_LZMA is not set */
/* PKG_USING_MULTIBUTTON is not set */
/* PKG_USING_FLEXIBLE_BUTTON is not set */
/* PKG_USING_CANFESTIVAL is not set */
/* PKG_USING_ZLIB is not set */
/* PKG_USING_MINIZIP is not set */
/* PKG_USING_HEATSHRINK is not set */
/* PKG_USING_DSTR is not set */
/* PKG_USING_TINYFRAME is not set */
/* PKG_USING_KENDRYTE_DEMO is not set */
/* PKG_USING_DIGITALCTRL is not set */
/* PKG_USING_UPACKER is not set */
/* PKG_USING_UPARAM is not set */
/* PKG_USING_HELLO is not set */
/* PKG_USING_VI is not set */
/* PKG_USING_ARMv7M_DWT is not set */
/* PKG_USING_CRCLIB is not set */
/* PKG_USING_LWGPS is not set */
/* PKG_USING_STATE_MACHINE is not set */
/* PKG_USING_DESIGN_PATTERN is not set */
/* PKG_USING_CONTROLLER is not set */
/* PKG_USING_PHASE_LOCKED_LOOP is not set */
/* PKG_USING_MFBD is not set */
/* PKG_USING_SLCAN2RTT is not set */
/* PKG_USING_SOEM is not set */
/* PKG_USING_QPARAM is not set */

/* Arduino libraries */

/* PKG_USING_RTDUINO is not set */

/* Projects */

/* PKG_USING_ARDUINO_ULTRASOUND_RADAR is not set */
/* PKG_USING_ARDUINO_SENSOR_KIT is not set */
/* PKG_USING_ARDUINO_MATLAB_SUPPORT is not set */

/* Sensors */

/* PKG_USING_ARDUINO_CAPACITIVESENSOR is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ADXL375 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_VL53L0X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SENSOR is not set */
/* PKG_USING_ADAFRUIT_MAX31855 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MAX31865 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MAX31856 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MLX90614 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LSM9DS1 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AHTX0 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LSM9DS0 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP280 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ADT7410 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP085 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BME680 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP9808 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP4728 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_INA219 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LTR390 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ADXL345 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_DHT is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP9600 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LSM6DS is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BNO055 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MAX1704X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MMC56X3 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MLX90393 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MLX90395 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ICM20X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_DPS310 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_HTS221 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SHT4X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SHT31 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ADXL343 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BME280 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AS726X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AMG88XX is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AM2320 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AM2315 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LTR329_LTR303 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP085_UNIFIED is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP183 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP183_UNIFIED is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BMP3XX is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MS8607 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LIS3MDL is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MLX90640 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MMA8451 is not set */
/* PKG_USING_ADAFRUIT_MSA301 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MPL115A2 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BNO08X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BNO08X_RVC is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LIS2MDL is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LSM303DLH_MAG is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LC709203F is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_CAP1188 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_CCS811 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_NAU7802 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LIS331 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LPS2X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LPS35HW is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LSM303_ACCEL is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_LIS3DH is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_PCF8591 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MPL3115A2 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MPR121 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MPRLS is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MPU6050 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_PCT2075 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_PM25AQI is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_EMC2101 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_FXAS21002C is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SCD30 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_FXOS8700 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_HMC5883_UNIFIED is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SGP30 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TMP006 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TLA202X is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TCS34725 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SI7021 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SI1145 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SGP40 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SHTC3 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_HDC1000 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_HTU21DF is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AS7341 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_HTU31D is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SENSORLAB is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_INA260 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TMP007_LIBRARY is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_L3GD20 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TMP117 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TSC2007 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TSL2561 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TSL2591_LIBRARY is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_VCNL4040 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_VEML6070 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_VEML6075 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_VEML7700 is not set */
/* PKG_USING_ARDUINO_SEEED_LIS3DHTR is not set */
/* PKG_USING_ARDUINO_SEEED_DHT is not set */
/* PKG_USING_ARDUINO_SEEED_ADXL335 is not set */
/* PKG_USING_ARDUINO_SEEED_ADXL345 is not set */
/* PKG_USING_ARDUINO_SEEED_BME280 is not set */
/* PKG_USING_ARDUINO_SEEED_BMP280 is not set */
/* PKG_USING_ARDUINO_SEEED_H3LIS331DL is not set */
/* PKG_USING_ARDUINO_SEEED_MMA7660 is not set */
/* PKG_USING_ARDUINO_SEEED_TSL2561 is not set */
/* PKG_USING_ARDUINO_SEEED_PAJ7620 is not set */
/* PKG_USING_ARDUINO_SEEED_VL53L0X is not set */
/* PKG_USING_SEEED_ITG3200 is not set */
/* PKG_USING_ARDUINO_SEEED_SHT31 is not set */
/* PKG_USING_ARDUINO_SEEED_HP20X is not set */
/* PKG_USING_ARDUINO_SEEED_DRV2605L is not set */
/* PKG_USING_ARDUINO_SEEED_BBM150 is not set */
/* PKG_USING_ARDUINO_SEEED_HMC5883L is not set */
/* PKG_USING_ARDUINO_SEEED_LSM303DLH is not set */
/* PKG_USING_ARDUINO_SEEED_TCS3414CS is not set */
/* PKG_USING_SEEED_MP503 is not set */
/* PKG_USING_ARDUINO_SEEED_BMP085 is not set */
/* PKG_USING_ARDUINO_SEEED_HIGHTEMP is not set */
/* PKG_USING_ARDUINO_SEEED_VEML6070 is not set */
/* PKG_USING_ARDUINO_SEEED_SI1145 is not set */
/* PKG_USING_ARDUINO_SEEED_SHT35 is not set */
/* PKG_USING_ARDUINO_SEEED_AT42QT1070 is not set */
/* PKG_USING_ARDUINO_SEEED_LSM6DS3 is not set */
/* PKG_USING_ARDUINO_SEEED_HDC1000 is not set */
/* PKG_USING_ARDUINO_SEEED_HM3301 is not set */
/* PKG_USING_ARDUINO_SEEED_MCP9600 is not set */
/* PKG_USING_ARDUINO_SEEED_LTC2941 is not set */

/* Display */

/* PKG_USING_ARDUINO_U8G2 is not set */
/* PKG_USING_SEEED_TM1637 is not set */

/* Timing */

/* PKG_USING_ARDUINO_MSTIMER2 is not set */

/* Data Processing */

/* PKG_USING_ARDUINO_KALMANFILTER is not set */
/* PKG_USING_ARDUINO_ARDUINOJSON is not set */

/* Data Storage */

/* Communication */

/* PKG_USING_ARDUINO_ADAFRUIT_PN532 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_SI4713 is not set */

/* Device Control */

/* PKG_USING_ARDUINO_ADAFRUIT_PCF8574 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_PCA9685 is not set */
/* PKG_USING_ARDUINO_SEEED_PCF85063TP is not set */

/* Other */

/* Signal IO */

/* PKG_USING_ARDUINO_ADAFRUIT_BUSIO is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_TCA8418 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP23017 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_ADS1X15 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_AW9523 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP3008 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_MCP4725 is not set */
/* PKG_USING_ARDUINO_ADAFRUIT_BD3491FS is not set */

/* Uncategorized */

/* RT-Thread board config */

#define RT_BOARD_NAME "audio_hollyland_a6902_rx"
#define RK_SRAM_END 0x200C0000
/* M4_JTAG_ENABLE is not set */
/* DSP_JTAG_ENABLE is not set */

/* RT-Thread rockchip common drivers */

/* HAL_DBG_ON is not set */
/* RT_USING_SND_GLB_RST is not set */
#define RT_USING_RESET
#define RT_USING_CRU_DUMP
/* RT_USING_REGULATOR_DUMP is not set */
/* RT_USING_PROF_IRQ is not set */
/* RT_USING_PMIC is not set */
/* RT_USING_PWM_REGULATOR is not set */
#define RT_USING_CACHE
#define RT_USING_UNCACHE_HEAP
#define RT_UNCACHE_HEAP_ORDER 0x0F
#define RT_USING_LARGE_HEAP
#define RT_LARGE_MALLOC_THRRESH 512
#define RT_LARGE_HEAP_SIZE 262144
/* RT_USING_DTCM_HEAP is not set */
/* RT_USING_PSRAM_HEAP is not set */
/* RT_USING_PANEL_FPGA_LVDS is not set */
/* RT_USING_PANEL_SS is not set */
/* RT_USING_PANEL_S6E3FC2X01 is not set */
/* RT_USING_PANEL_S6E3HC2_X1 is not set */
/* RT_USING_PANEL_S6E3HC2_X4 is not set */
/* RT_USING_PANEL_ST7703_DS is not set */
/* RT_USING_PANEL_ST7796H2 is not set */
/* RT_USING_PANEL_AM018RT90211 is not set */
/* RT_USING_PANEL_AM014RT90327V0 is not set */
/* RT_USING_PANEL_H245QBN02 is not set */
#define RT_USING_PANEL_HL_RM690A0
/* RT_USING_BACKLIGHT is not set */

/* Enable Fault Dump Hook */

/* RT_USNING_FAULT_DUMP_HOOK is not set */
#define RT_USING_SNOR
#define RT_SNOR_SPEED 80000000
#define RT_SNOR_DUAL_IO
/* RT_ROOT_DEF_PART_OFFSET is not set */
/* RT_ROOT_DEF_PART_SIZE is not set */
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
/* RT_USING_ISP3 is not set */

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
#define RT_USING_TOUCH_DRIVERS
/* RT_USING_TOUCH_GTXX is not set */
/* RT_USING_TOUCH_S3706 is not set */
/* RT_USING_CSTXX is not set */
/* RT_USING_CYTTSP5 is not set */
/* RT_USING_IT7259 is not set */
/* RT_USING_FT3169 is not set */
#define RT_USING_ZTW523A
#define RT_TOUCH_X_RANGE 126
#define RT_TOUCH_Y_RANGE 294
/* RT_USING_SENSOR_HUB is not set */
/* RT_USING_WDT_DUMP is not set */
/* RT_USING_QPIPSRAM is not set */

/* RT-Thread rockchip coredump driver */

/* RT_USING_COREDUMP is not set */
/* RT_USING_SPINAND is not set */

/* Enable PSTORE */

/* RT_USING_PSTORE is not set */

/* RT-Thread rockchip AMP driver */

/* RT-Thread rockchip rk2108 drivers */

/* Enable Audio */

#define RT_USING_CODEC
#define RT_USING_CODEC_NAU88L25B
#define RT_USING_CODEC_NAU88L25B_HP
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
#define RT_USING_AUDIO_CARD_NAU88L25B_HP

/* Audio Interfaces */

#define RT_USING_PCM
/* RT_USING_VAD is not set */
#define RT_USING_CRU
#define RT_USING_DMA
#define RT_USING_DMA_PL330
#define RT_USING_DMA0
#define RT_USING_PMU
#define RT_USING_DISPLAY
#define RT_USING_VOP
/* RT_USING_SPI_SCREEN is not set */
/* RT_USING_VOP_MCU is not set */
#define RT_USING_DSI
/* RT_USING_KEYCTRL is not set */
/* RT_USING_PWM0 is not set */

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
/* RT_USING_SPI1 is not set */
/* RT_USING_SPI2 is not set */
/* RT_USING_SYSTICK is not set */

/* RT-Thread application */

/* RT_USING_LVGL_APP_DEMO is not set */
/* RT_USING_ROCKCHIP_DEMO is not set */
/* RT_USING_USBAPP is not set */
/* RT_USING_RK_BATTERY_IPC is not set */
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
/* RT_USING_COMMON_TEST_AUDIO is not set */
/* RT_USING_COMMON_TEST_CODEC_ES8388 is not set */
/* RT_USING_COMMON_TEST_CPU_USAGE is not set */
/* RT_USING_COMMON_TEST_DCACHE is not set */
/* RT_USING_COMMON_TEST_ICACHE is not set */
/* RT_USING_COMMON_TEST_DISPLAY is not set */
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
/* RT_USING_COMMON_TEST_WDT is not set */
/* RT_USING_COMMON_TEST_PERFORMANCE is not set */
/* RT_USING_COMMON_TEST_FWANALYSIS is not set */
/* RT_USING_COMMON_TEST_OTA is not set */
/* RT_USING_COMMON_TEST_TIMER is not set */
#define RT_USING_FWANALYSIS

#endif
