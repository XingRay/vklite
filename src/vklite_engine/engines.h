//
// Created by leixing on 2025/6/8.
//

#pragma once

#include "vklite_engine/device_engine/DeviceEngine.h"
#include "vklite_engine/device_engine/DeviceEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "vklite_engine/device_engine/AndroidDeviceEngineBuilder.h"
#endif


#include "vklite_engine/graphic_pipeline_engine/GraphicPipelineEngine.h"
#include "vklite_engine/graphic_pipeline_engine/GraphicPipelineEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "vklite_engine/graphic_pipeline_engine/AndroidGraphicPipelineEngineBuilder.h"
#endif


#include "vklite_engine/compute_graphic_engine/ComputeGraphicEngine.h"
#include "vklite_engine/compute_graphic_engine/ComputeGraphicEngineBuilder.h"
#ifdef VKLITE_ANDROID
#include "vklite_engine/compute_graphic_engine/AndroidComputeGraphicEngineBuilder.h"
#endif
