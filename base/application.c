#include <application.h>
#include <usb_talk.h>
#include <bc_scheduler.h>

#define PREFIX_REMOTE "fireworks-launcher-remote"
#define PREFIX_BASE "fireworks-launcher-base"
#define UPDATE_INTERVAL 5000

static bc_led_t led;
static bc_scheduler_task_id_t enroll_to_gateway_task_id;

void enroll_to_gateway_task(void *param);
static void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param);
static void radio_event_handler(bc_radio_event_t event, void *event_param);
static void send_relay_state_set(usb_talk_payload_t *payload, void *param);

void application_init(void)
{
    usb_talk_init();

    bc_led_init(&led, BC_GPIO_LED, false, false);

    static bc_button_t button;
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize radio
    bc_radio_init();
    bc_radio_set_event_handler(radio_event_handler, NULL);
    bc_radio_listen();

    static uint8_t relay0_0 = 0x00;
    usb_talk_sub(PREFIX_REMOTE "/relay/0:0/state/set", send_relay_state_set, &relay0_0);
    static uint8_t relay_0_1 = 0x01;
    usb_talk_sub(PREFIX_REMOTE "/relay/0:1/state/set", send_relay_state_set, &relay_0_1);
    static uint8_t power_relay = 0x02;
    usb_talk_sub(PREFIX_REMOTE "/relay/-/state/set", send_relay_state_set, &power_relay);

    enroll_to_gateway_task_id = bc_scheduler_register(enroll_to_gateway_task, NULL, BC_TICK_INFINITY);
}

void enroll_to_gateway_task(void *param)
{
    (void) param;

    bc_radio_enroll_to_gateway();
}

static void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_BUTTON_EVENT_CLICK)
    {
        static u_int16_t event_count = 0;
        bc_radio_pub_push_button(&event_count);
        usb_talk_publish_push_button(PREFIX_BASE, &event_count);
        event_count++;
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK_FAST);
        bc_radio_enrollment_start();
    }
}

static void radio_event_handler(bc_radio_event_t event, void *event_param)
{
    (void) event_param;

    if (event == BC_RADIO_EVENT_ATTACH)
    {

        bc_led_pulse(&led, 1000);

        bc_led_set_mode(&led, BC_LED_MODE_OFF);

        bc_scheduler_plan_relative(enroll_to_gateway_task_id, 1000);
    }
}

static void send_relay_state_set(usb_talk_payload_t *payload, void *param)
{

    bool state;

    if (!usb_talk_payload_get_bool(payload, &state))
    {
        return;
    }

    uint8_t buffer[] = {0x3B, *(uint8_t *)param, state};
    bc_radio_pub_buffer(buffer, sizeof(buffer));
}

void bc_radio_on_push_button(uint64_t *peer_device_address, uint16_t *event_count)
{
    (void) peer_device_address;

    bc_led_pulse(&led, 1000);

    usb_talk_publish_push_button(PREFIX_REMOTE, event_count);
}

void bc_radio_on_thermometer(uint64_t *peer_device_address, uint8_t *i2c, float *temperature)
{
    (void) peer_device_address;

    usb_talk_publish_thermometer(PREFIX_REMOTE, i2c, temperature);
}

void bc_radio_on_humidity(uint64_t *peer_device_address, uint8_t *i2c, float *percentage)
{
    (void) peer_device_address;

    usb_talk_publish_humidity_sensor(PREFIX_REMOTE, i2c, percentage);
}

void bc_radio_on_lux_meter(uint64_t *peer_device_address, uint8_t *i2c, float *illuminance)
{
    (void) peer_device_address;

    usb_talk_publish_lux_meter(PREFIX_REMOTE, i2c, illuminance);
}

void bc_radio_on_barometer(uint64_t *peer_device_address, uint8_t *i2c, float *pressure, float *altitude)
{
    (void) peer_device_address;

    usb_talk_publish_barometer(PREFIX_REMOTE, i2c, pressure, altitude);
}

void bc_radio_on_buffer(uint64_t *peer_device_address, uint8_t *buffer, size_t *length)
{
    (void) peer_device_address;

    if (*length != 3)
    {
        return;
    }
    if (buffer[0] != 0x3C)
    {
        return;
    }

    uint8_t number = buffer[1];

    if (number == 0x02)
    {
        bool state = buffer[2];
        usb_talk_publish_relay(PREFIX_REMOTE, &state);
    }
    else
    {
        bc_module_relay_state_t state = (bool)buffer[2] == true ? BC_MODULE_RELAY_STATE_TRUE : BC_MODULE_RELAY_STATE_FALSE;
        usb_talk_publish_module_relay(PREFIX_REMOTE, &number, &state);
    }
}

