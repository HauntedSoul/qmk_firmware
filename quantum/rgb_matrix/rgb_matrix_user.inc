
// Declaring the custom effects
// Effect - RGB_MATRIX_CUSTOM_BREATHING_MULTISPLASH
RGB_MATRIX_EFFECT(BREATHING_MULTISPLASH)


// Defining the custom effects
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool my_effect_runner_reactive_splash(uint8_t start, effect_params_t* params, reactive_splash_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        HSV hsv = rgb_matrix_config.hsv;
        hsv.v   = 0;
        for (uint8_t j = start; j < count; j++) {
            int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
            int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
            uint8_t  dist = sqrt16(dx * dx + dy * dy);
            uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], qadd8(rgb_matrix_config.speed, 1));
            hsv           = effect_func(hsv, dx, dy, dist, tick);
        }
        hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
        
        if (hsv.v > 0) {
            RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }

    return led_max < DRIVER_LED_TOTAL; 
}

static bool BREATHING_MULTISPLASH(effect_params_t* params) {

    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // Hue Breath effect code
    uint8_t  huedelta = 12;
    HSV      hsv      = rgb_matrix_config.hsv;
    uint16_t time     = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    hsv.h             = hsv.h + scale8(abs8(sin8(time) - 128) * 2, huedelta);
    RGB rgb           = hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return my_effect_runner_reactive_splash(0, params, &SOLID_SPLASH_math);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
