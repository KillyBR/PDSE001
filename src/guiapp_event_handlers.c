
#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"
#include "guiapp_event_handlers.h"
#include "main_thread.h"

extern GX_WINDOW_ROOT *p_window_root;

static UINT show_window(GX_WINDOW *p_new, GX_WIDGET *p_widget, bool detach_old);
//static void update_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, GX_CONST GX_CHAR *string_id);
static void update_numeric(GX_WIDGET *p_widget, GX_RESOURCE_ID id, int numeric);

int set_point = 0;
int rpm = 0;
static int count = 0;

bool flag_activate = false;

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process (widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        case GX_EVENT_PEN_UP:
            show_window ((GX_WINDOW *) &window3, (GX_WIDGET *) widget, true);
        break;
        default:
            result = gx_window_event_process (widget, event_ptr);
        break;
    }
    return result;
}

UINT window2_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process (widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        case GX_SIGNAL(BTN_BACK, GX_EVENT_CLICKED):
            show_window ((GX_WINDOW *) &window3, (GX_WIDGET *) widget, true);
        break;
        default:
            result = gx_window_event_process (widget, event_ptr);
        break;
    }
    return result;
}

UINT window3_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process (widget, event_ptr);

    switch (event_ptr->gx_event_type)
    {
        case GX_SIGNAL(BTN_STOP, GX_EVENT_CLICKED):
            // show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true);
            flag_activate = false;
        break;
        case GX_SIGNAL(BTN_START, GX_EVENT_CLICKED):
            // show_window((GX_WINDOW*)&window2, (GX_WIDGET*)widget, true);
            flag_activate = true;
        break;
        case GX_SIGNAL(BTN_INC, GX_EVENT_CLICKED):
/*            set_point = set_point + 1;
            if (set_point > 100)
            {
                set_point = 100;
            }*/
            update_numeric (widget->gx_widget_parent, PR_SET_POINT, set_point);
        break;
        case GX_SIGNAL(BTN_DEC, GX_EVENT_CLICKED):
/*            set_point = set_point - 1;
            if (set_point < 0)
            {
                set_point = 0;
            }
*/
            update_numeric (widget->gx_widget_parent, PR_SET_POINT, set_point);
        break;
        case GX_SIGNAL(BTN_ABOUT, GX_EVENT_CLICKED):
            show_window ((GX_WINDOW *) &window2, (GX_WIDGET *) widget, true);
        break;
        default:
            result = gx_window_event_process (widget, event_ptr);
        break;
    }
    return result;
}

static UINT show_window(GX_WINDOW *p_new, GX_WIDGET *p_widget, bool detach_old)
{
    UINT err = GX_SUCCESS;

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

    GX_WIDGET *p_old = p_widget;
    if (p_old && detach_old)
    {
        if (p_old != (GX_WIDGET *) p_new)
        {
            gx_widget_detach(p_old);
        }
    }

    return err;
}

static void update_numeric(GX_WIDGET *p_widget, GX_RESOURCE_ID id, int numeric)
{
    GX_NUMERIC_PROMPT *p_prompt = NULL;

    ssp_err_t err = gx_widget_find(p_widget, (USHORT )id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET ** )&p_prompt);
    if (TX_SUCCESS == err)
    {
        _gxe_numeric_prompt_value_set (p_prompt, numeric);
    }
}

void update_duty_cycle(int duty_cycle)
{
    update_numeric ((GX_WIDGET *) &window3, PR_DUTY_CYCLE, duty_cycle);
}

void update_speed(int speed)
{
    //update_numeric ((GX_WIDGET *) &window3, PR_SPEED, speed);
    update_numeric ((GX_WIDGET *) &window3, PR_SPEED, speed * 60);
    rpm = speed;
}

int get_set_point()
{
    return set_point;
}

int get_speed()
{
    return rpm;
}

bool get_flag()
{
    return flag_activate;
}

void update_ventilador_2()
{
    update_ventilador ((GX_WIDGET *) &window3, PR_VENTILADOR);
}

void update_ventilador(GX_WIDGET *p_widget, GX_RESOURCE_ID id)
{
    //static int count = 0;
    if(flag_activate==true){
        count++;
    }
    GX_PIXELMAP_PROMPT *p_prompt = NULL;
    ssp_err_t err = gx_widget_find(p_widget, (USHORT )id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET ** )&p_prompt);
    if (TX_SUCCESS == err)
    {
        switch (count)
        {
            case 0:
                _gxe_pixelmap_prompt_pixelmap_set (p_prompt,
                GX_PIXELMAP_ID_VENTILADOR1,
                                                   GX_PIXELMAP_ID_VENTILADOR1,
                                                   GX_PIXELMAP_ID_VENTILADOR1,
                                                   GX_PIXELMAP_ID_VENTILADOR1,
                                                   GX_PIXELMAP_ID_VENTILADOR1,
                                                   GX_PIXELMAP_ID_VENTILADOR1);
            break;
            case 1:
                _gxe_pixelmap_prompt_pixelmap_set (p_prompt,
                GX_PIXELMAP_ID_VENTILADOR2,
                                                   GX_PIXELMAP_ID_VENTILADOR2,
                                                   GX_PIXELMAP_ID_VENTILADOR2,
                                                   GX_PIXELMAP_ID_VENTILADOR2,
                                                   GX_PIXELMAP_ID_VENTILADOR2,
                                                   GX_PIXELMAP_ID_VENTILADOR2);
            break;
            case 2:
                _gxe_pixelmap_prompt_pixelmap_set (p_prompt,
                GX_PIXELMAP_ID_VENTILADOR3,
                                                   GX_PIXELMAP_ID_VENTILADOR3,
                                                   GX_PIXELMAP_ID_VENTILADOR3,
                                                   GX_PIXELMAP_ID_VENTILADOR3,
                                                   GX_PIXELMAP_ID_VENTILADOR3,
                                                   GX_PIXELMAP_ID_VENTILADOR3);
                count = -1;
            break;
            default:
            break;

        }
    }
}

//kll>
void set_flag_true()
{
    flag_activate = true;
}
void set_flag_false()
{
    flag_activate = false;
}
void set_value_adc(uint16_t u16SetPoint, uint16_t u16PwmPercent)
{
    set_point = u16SetPoint;
    update_numeric ((GX_WIDGET *) &window3, PR_SET_POINT, set_point);
    update_duty_cycle(u16PwmPercent);
}
//kll<
