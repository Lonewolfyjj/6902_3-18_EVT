#include "hl_mod_page_common.h"

#if (!HL_IS_TX_DEVICE())
#include "hl_mod_display.h"
#include "lvgl.h"

#include "hl_mod_page.h"
#include "lv_port_indev.h"
#include "page_test.h"
#include "page_menu.h"
#include "hl_mod_input.h"




static void hl_mod_page_setup(void)
{

}

static void hl_mod_page_exit(void)
{
   
}

static void hl_mod_page_loop(void)
{
    uint8_t key_event;

    key_event  = hl_mod_get_knob_okkey_val();
    
    if (key_event == HL_KEY_EVENT_SHORT) {
    }
  
}

PAGE_DEC(PAGE_RECORD_PROTECT)
{
    bool result;

    result     = PageManager_PageRegister(PAGE_RECORD_PROTECT, hl_mod_page_setup, hl_mod_page_loop, hl_mod_page_exit,
                                      NULL);

    if (result == false) {
        LV_LOG_USER("page_record_protect init fail\n");
    }
}

#endif
/*
 * EOF
 */



