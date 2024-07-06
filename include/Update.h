#ifndef UPDATE_H
#define UPDATE_H

#define CGLM_ALL_UNALIGNED
#include <cglm/cglm.h>
#include <pcg/pcg_basic.h>

#include <App.h>
#include <Utils.h>

double lastUpdate;

void updateSetup(App* app);
void update(App* app);
void createSSBOs(App* app);
void mapBuffers(App* app);

#endif