#ifndef UI_LIGHTS_H
#define UI_LIGHTS_H

#ifdef __cplusplus
extern "C"
{
#endif

    lv_updatable_screen_t lightsScreen;

    static lv_obj_t *lights_btnm;
    static lv_obj_t *compass_slider;
    // static lv_obj_t *motoring_led;

    static const char *lights_btnm_map[] = {
        "Nav", "Motor",
        "Anchor", "\n",
        "Deck", "Inst", ""};

    static lv_btnmatrix_ctrl_t lights_ctrl_map[] = {
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE,
        LV_BTNMATRIX_CTRL_CHECKABLE};

        static void lights_slider_event_cb(lv_event_t *e){
            lv_event_code_t code = lv_event_get_code(e);
            lv_obj_t *obj = lv_event_get_target(e);
            if (code == LV_EVENT_RELEASED){
                
                int i = lv_slider_get_value(compass_slider);
                float f = (1.0 * i) / 100.0;
                Serial.print("Setting intesity value: "); Serial.println(f);
                app.onDelay(0, [f]()
                                    { turnSwitch("electrical/lights/1/compass.state", f); });
            }

        }

    static void lights_event_cb(lv_event_t *e)
    {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t *obj = lv_event_get_target(e);
        if (code == LV_EVENT_CLICKED)
        {
            uint32_t id = lv_btnmatrix_get_selected_btn(obj);
            const char *txt = lv_btnmatrix_get_btn_text(obj, id);

            if (txt != NULL)
            {
                if (strcmp("Nav", txt) == 0)
                {
                    if (shipDataModel.navigation.lights.bow_red_green.state.st == on_off_e::ON)
                    {
                        Serial.println("Actual State = 1; Changing to 0");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/navigation/state", 0.0); });
                    }
                    else
                    {
                        Serial.println("Actual State = 0; Changing to 1");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/navigation/state", 1.0); });
                    }
                }else  if (strcmp("Motor", txt) == 0)
                {
                    if (shipDataModel.navigation.lights.motoring.state.st == on_off_e::ON)
                    {
                        Serial.println("Actual State = 1; Changing to 0");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/motoring/state", 0.0); });
                    }
                    else
                    {
                        Serial.println("Actual State = 0; Changing to 1");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/motoring/state", 1.0); });
                    }
                }
                else  if (strcmp("Anchor", txt) == 0)
                {
                    if (shipDataModel.navigation.lights.anchor.state.st == on_off_e::ON)
                    {
                        Serial.println("Actual State = 1; Changing to 0");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/anchor/state", 0.0); });
                    }
                    else
                    {
                        Serial.println("Actual State = 0; Changing to 1");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/anchor/state", 1.0); });
                    }
                }
                 else  if (strcmp("Deck", txt) == 0)
                {
                    if (shipDataModel.navigation.lights.deck.state.st == on_off_e::ON)
                    {
                        Serial.println("Actual State = 1; Changing to 0");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/deck/state", 0.0); });
                    }
                    else
                    {
                        Serial.println("Actual State = 0; Changing to 1");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/deck/state", 1.0); });
                    }
                } if (strcmp("Inst", txt) == 0)
                {
                    if (shipDataModel.navigation.lights.instruments.state.st == on_off_e::ON)
                    {
                        Serial.println("Actual State = 1; Changing to 0");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/instruments/state", 0.0); });
                    }
                    else
                    {
                        Serial.println("Actual State = 0; Changing to 1");
                        app.onDelay(0, []()
                                    { turnSwitch("navigation/lights/instruments/state", 1.0); });
                    }
                }
            }
        }
    }

    void lights_update_ctrl_matrix()
    {
        if (shipDataModel.navigation.lights.bow_red_green.state.st == on_off_e::ON)
        {
            lights_ctrl_map[0] =  LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            lights_ctrl_map[0] = 0;
        }

        if (shipDataModel.navigation.lights.motoring.state.st == on_off_e::ON)
        {
            lights_ctrl_map[1] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            lights_ctrl_map[1] = 0;
        }

        if (shipDataModel.navigation.lights.anchor.state.st == on_off_e::ON)
        {
            lights_ctrl_map[2] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            lights_ctrl_map[2] = 0;
        }

        if (shipDataModel.navigation.lights.deck.state.st == on_off_e::ON)
        {
            lights_ctrl_map[3] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            lights_ctrl_map[3] = 0;
        }

        if (shipDataModel.navigation.lights.instruments.state.st == on_off_e::ON)
        {
            lights_ctrl_map[4] = LV_BTNMATRIX_CTRL_CHECKED;
        }
        else
        {
            lights_ctrl_map[4] = 0;
        }

    }
     static void lights_update_cb()
    {
        lights_update_ctrl_matrix();
        lv_btnmatrix_set_ctrl_map(lights_btnm, lights_ctrl_map);
        int i = floor(shipDataModel.navigation.lights.compass.intensity * 100.0);
        if(!lv_slider_is_dragged(compass_slider)){
            lv_slider_set_value(compass_slider, i, LV_ANIM_OFF);
        }

        //Serial.println("Updating lights data ");

    }

   void lv_lights_buttons(lv_obj_t *parent)
    {

        lights_btnm = lv_btnmatrix_create(parent);
        lv_btnmatrix_set_map(lights_btnm, lights_btnm_map);
        lv_obj_set_size(lights_btnm, LV_HOR_RES_MAX - 50, LV_VER_RES_MAX);
        lv_obj_align(lights_btnm, LV_ALIGN_TOP_LEFT, 0, 0); // Position
        lv_btnmatrix_set_btn_ctrl_all(lights_btnm, LV_BTNMATRIX_CTRL_CHECKABLE);
        lv_obj_add_event_cb(lights_btnm, lights_event_cb, LV_EVENT_CLICKED, NULL);

        compass_slider = lv_slider_create(parent);
        lv_slider_set_range(compass_slider, 0, 100);
        lv_obj_set_size(compass_slider, 25, LV_VER_RES_MAX-50);
        lv_obj_set_pos(compass_slider, LV_HOR_RES_MAX - 30 , 25);
        
        lv_obj_add_event_cb(compass_slider, lights_slider_event_cb, LV_EVENT_RELEASED, NULL);

         Serial.println("Screen created ");
  

        lights_update_cb();
    }


    void init_lightsScreen()
    {
        lightsScreen.screen = lv_obj_create(NULL); // Creates a Screen object
        lightsScreen.init_cb = lv_lights_buttons;
        lightsScreen.update_cb = lights_update_cb;
        //lights_update_cb();
    }

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
