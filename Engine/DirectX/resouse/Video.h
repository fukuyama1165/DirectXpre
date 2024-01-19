#pragma once
#include "DirectXInit.h"
#include <mfapi.h>
#include <mfplay.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <wmcodecdsp.h>

class InternalData {

public:
    IMFSourceResolver* pSourceResolver = NULL;
    IUnknown* uSource = NULL;
    IMFMediaSource* mediaFileSource = NULL;
    IMFAttributes* pVideoReaderAttributes = NULL;
    IMFSourceReader* pSourceReader = NULL;
    IMFMediaType* pReaderOutputType = NULL, * pFirstOutputType = NULL;
    MF_OBJECT_TYPE     ObjectType = MF_OBJECT_INVALID;

    // Start processing frames.
    IMFSample* pVideoSample = NULL;
    DWORD      streamIndex = 0, flags = 0;
    LONGLONG   llVideoTimeStamp = 0, llSampleDuration = 0;
    int        sampleCount = 0;
    DWORD      sampleFlags = 0;

    std::string target_texture = "";
    uint32_t  width = 0;
    uint32_t  height = 0;
    float     fps = 0.0f;

    float     clock_time = 0.0f;
    LONGLONG  video_time = 0;
    bool      finished = false;
    bool      paused = false;
    bool      autoloop = true;

    std::string fileName_ = "";

    ~InternalData();


    bool readOutputMediaFormat();

public:

    bool open(std::string filename);

    void update(float elapsed);

};

struct VideoTexture {

	static bool createAPI();
	static void destroyAPI();

	std::unique_ptr<InternalData> internal_data = nullptr;

	bool create(std::string);
	void destroy();
	bool update(float dt);

	void pause();
	void resume();
	bool hasFinished();
	std::string getTextureName();
	float getAspectRatio() const;

private:

	
};