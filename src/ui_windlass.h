#ifndef UI_WINDLASS_H
#define UI_WINDLASS_H

#ifdef __cplusplus
extern "C"
{
#endif

    lv_updatable_screen_t windlassScreen;

    static lv_obj_t *some_rode_length;
    static lv_obj_t *some_depth;
    static lv_obj_t *windlass_btnm;

    // static lv_obj_t *motoring_led;

    static const char *windlass_btnm_map[] = {
        "Up", "Stop",
        "Down", "\n",
        "Levar", "Pen", "Fond",
        
        ""
        };

    static lv_btnmatrix_ctrl_t windlass_ctrl_map[] = {
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE
        };



    static void windlass_event_cb(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj = lv_event_get_target(e);
        if (code == LV_EVENT_CLICKED)
        {
            uint32_t id = lv_btnmatrix_get_selected_btn(obj);
            const char *txt = lv_btnmatrix_get_btn_text(obj, id);

            if (txt != NULL)
            {
                if (strcmp("Up", txt) == 0)
                {
                    if (shipDataModel.navigation.anchor.windlass_state.state  != windlass_state_e::WLUP)
                    {
                        Serial.println("Changing windlass state to up");
                        app.onDelay(0, []()
                                    { windlassUp(); });
                    }
                    else
                    {
                        Serial.println("Stopping");
                        app.onDelay(0, []()
                                    { windlassStop(); });
                    }
                }else  if (strcmp("Stop", txt) == 0)
                {
                   
                        Serial.println("AStopping Windlass");
                        app.onDelay(0, []()
                                    { windlassStop(); });
                    
                }
                else  if (strcmp("Down", txt) == 0)
                {
                    if (shipDataModel.navigation.anchor.windlass_state.state  != windlass_state_e::WLDOWN)
                    {
                        Serial.println("Changing windlass state to down");
                        app.onDelay(0, []()
                                    { windlassDown(); });
                    }
                    else
                    {
                        Serial.println("Stopping Windlass");
                        app.onDelay(0, []()
                                    { windlassStop(); });
                    }
                }
                else  if (strcmp("Levar", txt) == 0)
                {
                   
                        Serial.println("Levant alcless");
                        app.onDelay(0, []()
                                    { levar(); });
                    
                }
                else  if (strcmp("Pen", txt) == 0)
                {
                   
                        Serial.println("Going to Pendura");
                        app.onDelay(0, []()
                                    {pendura(); });
                    
                }
                else  if (strcmp("Stop", txt) == 0)
                {
                   
                        Serial.println("Fondeixar");
                        app.onDelay(0, []()
                                    {fondeixar(); });
                    
                }
                
            }
        }
    }

    void windlass_update_ctrl_matrix()
    {
        if (shipDataModel.navigation.anchor.windlass_state.state == windlass_state_e::WLUP)
        {
            windlass_ctrl_map[0] =  LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            windlass_ctrl_map[0] = 0;
        }

        if (shipDataModel.navigation.anchor.windlass_state.state == windlass_state_e::WLOFF)
        {
            windlass_ctrl_map[1] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            windlass_ctrl_map[1] = 0;
        }

        if (shipDataModel.navigation.anchor.windlass_state.state == windlass_state_e::WLDOWN)
        {
            windlass_ctrl_map[2] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            windlass_ctrl_map[2] = 0;
        }

        windlass_ctrl_map[3] = 0;
        windlass_ctrl_map[4] = 0;
        windlass_ctrl_map[5] = 0;

    }
     static void windlass_update_cb()
    {
        
        windlass_update_ctrl_matrix();
        lv_btnmatrix_set_ctrl_map(windlass_btnm, windlass_ctrl_map);
        

        char buffer[10] = "";
        dtostrf(shipDataModel.navigation.anchor.rode_length.m, 0, 0, buffer);
        lv_label_set_text_fmt(some_rode_length, "Ch %s", buffer);

        dtostrf(shipDataModel.environment.depth.below_transducer.m, 0, 0, buffer);
        lv_label_set_text_fmt(some_depth, "Dp %s", buffer);

        //Serial.println("Updating lights data ");

    }

 
   void lv_windlass_buttons(lv_obj_t *parent)
    {
        const int width_l = 120;

        // Check if we are connected. If not connect

        setup_ble();

        some_rode_length = lv_label_create(parent);
        lv_label_set_text_static(some_rode_length, "--- m");
        lv_obj_set_width(some_rode_length, width_l);
        lv_obj_set_style_text_align(some_rode_length, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_align(some_rode_length, LV_ALIGN_TOP_LEFT, 0  , 5);

        #if LV_FONT_MONTSERRAT_40
            lv_obj_set_style_text_font(some_rode_length, &lv_font_montserrat_40, 0);
        #endif

        some_depth = lv_label_create(parent);
        lv_label_set_text_static(some_depth, "--- m");
        lv_obj_set_width(some_depth, width_l);
        lv_obj_set_style_text_align(some_depth, LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_align(some_depth, LV_ALIGN_TOP_RIGHT, 0  , 5);

        #if LV_FONT_MONTSERRAT_40
            lv_obj_set_style_text_font(some_depth, &lv_font_montserrat_40, 0);
        #endif


        windlass_btnm = lv_btnmatrix_create(parent);
        lv_btnmatrix_set_map(windlass_btnm, windlass_btnm_map);
        lv_obj_set_size(windlass_btnm, LV_HOR_RES_MAX, LV_VER_RES_MAX - 70);
        lv_obj_align(windlass_btnm, LV_ALIGN_TOP_LEFT, 0, 70); // Position
        lv_btnmatrix_set_btn_ctrl_all(windlass_btnm, LV_BTNMATRIX_CTRL_CHECKABLE);
        lv_obj_add_event_cb(windlass_btnm, windlass_event_cb, LV_EVENT_CLICKED, NULL);

        

         Serial.println("Screen created ");
  

        windlass_update_cb();
    }


    void init_windlassScreen()
    {
        windlassScreen.screen = lv_obj_create(NULL); // Creates a Screen object
        windlassScreen.init_cb = lv_windlass_buttons;
        windlassScreen.update_cb = windlass_update_cb;
        //lights_update_cb();
    }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
