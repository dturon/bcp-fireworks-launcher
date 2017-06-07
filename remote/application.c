#include <application.h>

#define UPDATE_INTERVAL 5000

static bc_led_t led;

static bc_module_relay_t relay_0_0;
static bc_module_relay_t relay_0_1;

static void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param);
static void radio_event_handler(bc_radio_event_t event, void *event_param);
static void temperature_tag_event_handler(bc_tag_temperature_t *self, bc_tag_temperature_event_t event, void *event_param);
static void humidity_tag_event_handler(bc_tag_humidity_t *self, bc_tag_humidity_event_t event, void *event_param);
static void lux_meter_event_handler(bc_tag_lux_meter_t *self, bc_tag_lux_meter_event_t event, void *event_param);
static void barometer_tag_event_handler(bc_tag_barometer_t *self, bc_tag_barometer_event_t event, void *event_param);

void application_init(void)
{
    bc_led_init(&led, BC_GPIO_LED, false, false);

    static bc_button_t button;
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize radio
    bc_radio_init();
    bc_radio_set_event_handler(radio_event_handler, NULL);
    bc_radio_listen();

    bc_module_relay_init(&relay_0_0, BC_MODULE_RELAY_I2C_ADDRESS_DEFAULT);
    bc_module_relay_init(&relay_0_1, BC_MODULE_RELAY_I2C_ADDRESS_ALTERNATE);

    bc_module_relay_set_state(&relay_0_0, BC_MODULE_RELAY_STATE_FALSE);
    bc_module_relay_set_state(&relay_0_1, BC_MODULE_RELAY_STATE_FALSE);

    bc_module_power_init();

    static bc_tag_temperature_t temperature_tag_0_48;
    bc_tag_temperature_init(&temperature_tag_0_48, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_DEFAULT);
    bc_tag_temperature_set_update_interval(&temperature_tag_0_48, UPDATE_INTERVAL);
    static uint8_t temperature_tag_0_48_i2c = (BC_I2C_I2C0 << 7) | BC_TAG_TEMPERATURE_I2C_ADDRESS_DEFAULT;
    bc_tag_temperature_set_event_handler(&temperature_tag_0_48, temperature_tag_event_handler, &temperature_tag_0_48_i2c);

    static bc_tag_temperature_t temperature_tag_0_49;
    bc_tag_temperature_init(&temperature_tag_0_49, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_ALTERNATE);
    bc_tag_temperature_set_update_interval(&temperature_tag_0_49, UPDATE_INTERVAL);
    static uint8_t temperature_tag_0_49_i2c = (BC_I2C_I2C0 << 7) | BC_TAG_TEMPERATURE_I2C_ADDRESS_ALTERNATE;
    bc_tag_temperature_set_event_handler(&temperature_tag_0_49, temperature_tag_event_handler, &temperature_tag_0_49_i2c);

    static bc_tag_humidity_t humidity_tag_r2_0_40;
    bc_tag_humidity_init(&humidity_tag_r2_0_40, BC_TAG_HUMIDITY_REVISION_R2, BC_I2C_I2C0, BC_TAG_HUMIDITY_I2C_ADDRESS_DEFAULT);
    bc_tag_humidity_set_update_interval(&humidity_tag_r2_0_40, UPDATE_INTERVAL);
    static uint8_t humidity_tag_r2_0_40_i2c = (BC_I2C_I2C0 << 7) | 0x40;
    bc_tag_humidity_set_event_handler(&humidity_tag_r2_0_40, humidity_tag_event_handler, &humidity_tag_r2_0_40_i2c);

    static bc_tag_lux_meter_t lux_meter_0_44;
    bc_tag_lux_meter_init(&lux_meter_0_44, BC_I2C_I2C0, BC_TAG_LUX_METER_I2C_ADDRESS_DEFAULT);
    bc_tag_lux_meter_set_update_interval(&lux_meter_0_44, UPDATE_INTERVAL);
    static uint8_t lux_meter_0_44_i2c = (BC_I2C_I2C0 << 7) | BC_TAG_LUX_METER_I2C_ADDRESS_DEFAULT;
    bc_tag_lux_meter_set_event_handler(&lux_meter_0_44, lux_meter_event_handler, &lux_meter_0_44_i2c);

    static bc_tag_barometer_t barometer_tag_0;
    bc_tag_barometer_init(&barometer_tag_0, BC_I2C_I2C0);
    bc_tag_barometer_set_update_interval(&barometer_tag_0, UPDATE_INTERVAL);
    static uint8_t barometer_tag_0_i2c = (BC_I2C_I2C0 << 7) | 0x60;
    bc_tag_barometer_set_event_handler(&barometer_tag_0, barometer_tag_event_handler, &barometer_tag_0_i2c);

}

void application_task(void)
{
    uint8_t buffer[] = {0x3C, 0x00, false};
    bc_radio_pub_buffer(buffer, sizeof(buffer));

    buffer[1] = 0x01;
    bc_radio_pub_buffer(buffer, sizeof(buffer));
}

static void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;
    if (event == BC_BUTTON_EVENT_CLICK)
    {
        static u_int16_t event_count = 0;
        bc_radio_pub_push_button(&event_count);
        event_count++;
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        bc_radio_enroll_to_gateway();

        bc_radio_enrollment_start();

        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
    }
}

static void radio_event_handler(bc_radio_event_t event, void *event_param)
{
    (void) event_param;

    if (event == BC_RADIO_EVENT_ATTACH)
    {
        bc_radio_enrollment_stop();

        bc_led_pulse(&led, 1000);

        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }
}

void bc_radio_on_push_button(uint64_t *peer_device_address, uint16_t *event_count)
{
    (void) peer_device_address;
    (void) event_count;

    bc_led_pulse(&led, 1000);
}

void bc_radio_on_buffer(uint64_t *peer_device_address, uint8_t *buffer, size_t *length)
{
    (void) peer_device_address;

    if (*length != 3)
    {
        return;
    }
    if (buffer[0] != 0x3B)
    {
        return;
    }

    bool state = buffer[2];

    if (buffer[1] == 0x00)
    {
        bc_module_relay_set_state(&relay_0_0, state);
        uint8_t buffer[] = {0x3C, 0x00, state};
        bc_radio_pub_buffer(buffer, sizeof(buffer));
    }
    else if (buffer[1] == 0x01)
    {
        bc_module_relay_set_state(&relay_0_1, state);
        uint8_t buffer[] = {0x3C, 0x01, state};
        bc_radio_pub_buffer(buffer, sizeof(buffer));
    }
    else if (buffer[1] == 0x02)
    {
        bc_module_power_relay_set_state(state);
        uint8_t buffer[] = {0x3C, 0x02, state};
        bc_radio_pub_buffer(buffer, sizeof(buffer));
    }
}

static void temperature_tag_event_handler(bc_tag_temperature_t *self, bc_tag_temperature_event_t event, void *event_param)
{
    float value;


    if (event != BC_TAG_TEMPERATURE_EVENT_UPDATE)
    {
        return;
    }

    if (bc_tag_temperature_get_temperature_celsius(self, &value))
    {
        bc_radio_pub_thermometer(*(uint8_t *)event_param, &value);
    }
}


void humidity_tag_event_handler(bc_tag_humidity_t *self, bc_tag_humidity_event_t event, void *event_param)
{
    float value;

    if (event != BC_TAG_HUMIDITY_EVENT_UPDATE)
    {
        return;
    }

    if (bc_tag_humidity_get_humidity_percentage(self, &value))
    {
        bc_radio_pub_humidity(*(uint8_t *)event_param, &value);
    }
}

void lux_meter_event_handler(bc_tag_lux_meter_t *self, bc_tag_lux_meter_event_t event, void *event_param)
{
    float value;

    if (event != BC_TAG_LUX_METER_EVENT_UPDATE)
    {
        return;
    }

    if (bc_tag_lux_meter_get_illuminance_lux(self, &value))
    {
        bc_radio_pub_luminosity(*(uint8_t *)event_param, &value);
    }
}

void barometer_tag_event_handler(bc_tag_barometer_t *self, bc_tag_barometer_event_t event, void *event_param)
{
    float pascal;
    float meter;

    if (event != BC_TAG_BAROMETER_EVENT_UPDATE)
    {
        return;
    }

    if (!bc_tag_barometer_get_pressure_pascal(self, &pascal))
    {
        return;
    }

    if (!bc_tag_barometer_get_altitude_meter(self, &meter))
    {
        return;
    }

    bc_radio_pub_barometer(*(uint8_t *)event_param, &pascal, &meter);

}


