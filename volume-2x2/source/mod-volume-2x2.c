#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define PLUGIN_URI "http://moddevices.com/plugins/mod-devel/mod-volume-2x2"


typedef enum {
    L_INPUT_1   = 0,
    L_INPUT_2   = 1,
    L_OUTPUT_1  = 2,
    L_OUTPUT_2  = 3,
    L_MODE      = 4,
    L_LEVEL     = 5,
    L_ENABLE    = 6
} PortIndex;


typedef struct {
    float* input_1;
    float* input_2;
    float* output_1;
    float* output_2;
    float* mode;
    float* level;
    float* plugin_enabled;
    double a0;
    double b1;
    double z1;
    float old_range_min;
    float old_range_max;
} Volume;


static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
        double                    rate,
        const char*               bundle_path,
        const LV2_Feature* const* features)
{
    Volume* self = (Volume*)malloc(sizeof(Volume));

    self->z1 = 0.0;
    double frequency = 20.0 / rate;
    self->b1 = exp(-2.0 * M_PI * frequency);
    self->a0 = 1.0 - self->b1;

    return (LV2_Handle)self;
}


static void
connect_port(LV2_Handle instance,
        uint32_t   port,
        void*      data)
{
    Volume* self = (Volume*)instance;

    switch ((PortIndex)port) {
        case L_INPUT_1:
            self->input_1 = (float*)data;
            break;
        case L_INPUT_2:
            self->input_2 = (float*)data;
            break;
        case L_OUTPUT_1:
            self->output_1 = (float*)data;
            break;
        case L_OUTPUT_2:
            self->output_2 = (float*)data;
            break;
        case L_MODE:
            self->mode = (float*)data;
            break;
        case L_LEVEL:
            self->level = (float*)data;
            break;
        case L_ENABLE:
            self->plugin_enabled = (float*)data;
            break;
    }
}


static void
activate(LV2_Handle instance)
{
}


static double
lowPassProcess(Volume* self, float input)
{
    return self->z1 = input * self->a0 + self->z1 * self->b1;
}


static void
run(LV2_Handle instance, uint32_t n_samples)
{
    Volume* self = (Volume*) instance;
    float coef;

    if ((int)*self->mode == 0) {
        coef = pow(10.0f, (1 - *self->level) * -45 / 20.0f);
    } else {
        coef = *self->level;
    }

    if (*self->level == 0.0f) {
        coef = 0.0;
    }

    if (*self->plugin_enabled == 0.0f) {
        coef = 1.0;
    }


    for ( uint32_t i = 0; i < n_samples; i++)
    {
        float smooth_coef = lowPassProcess(self, coef);
        self->output_1[i] = self->input_1[i] * smooth_coef;
        self->output_2[i] = self->input_2[i] * smooth_coef;
    }
}


static void
deactivate(LV2_Handle instance)
{
}


static void
cleanup(LV2_Handle instance)
{
    free(instance);
}


static const void*
extension_data(const char* uri)
{
    return NULL;
}


static const LV2_Descriptor descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};


LV2_SYMBOL_EXPORT
    const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    switch (index) {
        case 0:  return &descriptor;
        default: return NULL;
    }
}
