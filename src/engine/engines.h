//
// Created by leixing on 2025/6/8.
//

#pragma once

#include "engine/device_engine/DeviceEngine.h"
#include "engine/device_engine/DeviceEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "engine/device_engine/AndroidDeviceEngineBuilder.h"
#endif


#include "engine/graphic_pipeline_engine/GraphicPipelineEngine.h"
#include "engine/graphic_pipeline_engine/GraphicPipelineEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "engine/graphic_pipeline_engine/AndroidGraphicPipelineEngineBuilder.h"
#endif


#include "engine/compute_graphic_engine/ComputeGraphicEngine.h"
#include "engine/compute_graphic_engine/ComputeGraphicEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "engine/compute_graphic_engine/AndroidComputeGraphicEngineBuilder.h"
#endif
