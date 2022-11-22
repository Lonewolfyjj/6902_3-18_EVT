#define A6902_TX_HL 0
#define A6902_RX_HL_CH 1
#define A6902_RX_HL_EN 0
#define A6902_TX_JF 0
#define A6902_RX_JF_CH 0
#define A6902_RX_JF_EN 0
// 用于指示型号配置结束，注意该文件不可以加入文件头部注释，除了管理员外不可以在该行自行添加任何注释，否则编译会出问题
// indicate end of config, do not add any comment in this line except the maintainer

#if A6902_TX_HL

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "CN"

#elif A6902_TX_JF

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "CN"

#elif A6902_RX_HL_CH

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "CN"

#elif A6902_RX_JF_CH

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "CN"

#elif A6902_RX_HL_EN

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "EN"

#elif A6902_RX_JF_EN

#define A6902_VERSION "V0.0.0.2"
#define A6902_LANGUAGE "EN"

#endif

// RX返回0 TX返回1
#define HL_IS_TX_DEVICE() (A6902_TX_HL | A6902_TX_JF)

// HL返回0 飓风返回1
#define HL_GET_LOGO_TYPE() (A6902_TX_JF | A6902_RX_JF_CH | A6902_RX_JF_EN)