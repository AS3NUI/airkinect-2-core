/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef KinectExtension_AKMSSDKInteractionClient_h
#define KinectExtension_AKMSSDKInteractionClient_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

class AKMSSDKInteractionClient : public INuiInteractionClient
{
public:

	AKMSSDKInteractionClient() {}
	~AKMSSDKInteractionClient(){}

	STDMETHODIMP_(ULONG)    AddRef()                                    { return 2;     }
    STDMETHODIMP_(ULONG)    Release()                                   { return 1;     }
    STDMETHODIMP            QueryInterface(REFIID riid, void **ppv)     { return S_OK;  }
    
	STDMETHOD(GetInteractionInfoAtLocation)(THIS_ DWORD skeletonTrackingId, NUI_HAND_TYPE handType, FLOAT x, FLOAT y, _Out_ NUI_INTERACTION_INFO *pInteractionInfo)
	{
        if(pInteractionInfo)
        {
            pInteractionInfo->IsPressTarget         = false;
            pInteractionInfo->PressTargetControlId  = 0;
            pInteractionInfo->PressAttractionPointX = 0.f;
            pInteractionInfo->PressAttractionPointY = 0.f;
            pInteractionInfo->IsGripTarget          = true;
            return S_OK;
        }
        return E_POINTER;
    }

};

#endif
#endif